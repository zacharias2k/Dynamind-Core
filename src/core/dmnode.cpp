/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @author  Michael Mair <abroxos@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaMite
 *
 * Copyright (C) 2011  Christian Urich, Michael Mair, Markus Sengthaler

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <dmcomponent.h>
#include <dmnode.h>
#include <dmedge.h>
#include <cstdlib>
#include <math.h>

#include <dmdbconnector.h>
#include <QSqlQuery>
#include "dmlogger.h"

using namespace DM;

Node::Node( double x, double y, double z) : 
	Component(true),
	vector(x,y,z)
{
	isInserted = false;
	connectedEdges = 0;
}

Node::Node() : 
	Component(true)
{
	isInserted = false;
	connectedEdges = 0;
}

Node::Node(const Node& n) : 
	Component(n, true),
	vector(n.vector)
{
	isInserted = false;
	connectedEdges = 0;
	if(n.connectedEdges)
		foreach(Edge* e, *n.connectedEdges)
		this->addEdge(e);
}
Node::~Node()
{
	if(connectedEdges)	delete connectedEdges;
}
void Node::SetOwner(Component *owner)
{
	QMutexLocker ml(mutex);

	currentSys = owner->getCurrentSystem();

	mforeach(Attribute* a, ownedattributes)
		a->setOwner(this);
}

DM::Components Node::getType() const
{
	return DM::NODE;
}
QString Node::getTableName()
{
	return "nodes";	// also give in precache
}
double Node::getX() const
{
	return vector.x;
}

double Node::getY() const
{
	return vector.y;
}

double Node::getZ() const
{
	return vector.z;
}

void Node::get(double *vector) const
{
	memcpy(vector, &this->vector, sizeof(this->vector));
}

const double Node::get(unsigned int i) const {
	if(i==0)		return getX();
	else if(i==1)	return getY();
	else if(i==2)	return getZ();
	else			return 0;
}

std::vector<Edge*> Node::getEdges() const
{
	if(!connectedEdges)
		return std::vector<Edge*>();

	std::vector<Edge*> edges;
	foreach(Edge* e,*connectedEdges)
		edges.push_back(e);
	return edges;
}

void Node::set(double x, double y, double z)
{
	QMutexLocker ml(mutex);
	vector.x = x;
	vector.y = y;
	vector.z = z;
}

void Node::setX(double x)
{
	vector.x = x;
}

void Node::setY(double y)
{
	vector.y = y;
}

void Node::setZ(double z)
{
	vector.z = z;
}

Component* Node::clone()
{
	return new Node(*this);
}

Node& Node::operator=(const Node& other)
{
	QMutexLocker ml(mutex);

	if(this != &other)
	{
		vector = other.vector;

		if(!connectedEdges && other.connectedEdges)
			foreach(Edge* e, *other.connectedEdges)
			this->addEdge(e);
	}
	return *this;
}

bool Node::operator ==(const Node & other) const 
{
	return 0 == memcmp(&vector, &other.vector, sizeof(vector));
}

const Node Node::operator -(const Node & other) const 
{
	return Node(vector.x - other.vector.x,
				vector.y - other.vector.y,
				vector.z - other.vector.z);
}

const Node Node::operator +(const Node & other) const 
{
	return Node(vector.x + other.vector.x,
				vector.y + other.vector.y,
				vector.z + other.vector.z);
}
const Node Node::operator *(const double &val) const
{
	return Node(vector.x * val,
				vector.y * val,
				vector.z * val);
}

const Node Node::operator /(const double &val) const
{
	return Node(vector.x / val,
				vector.y / val,
				vector.z / val);
}

bool Node::compare2d(const Node &other, double round ) const 
{
	return fabs(vector.x - other.vector.x) <= round 
		&& fabs(vector.y - other.vector.y) <= round;
}
bool Node::compare2d(const Node * other , double round ) const 
{
	return fabs(vector.x - other->vector.x) <= round 
		&& fabs(vector.y - other->vector.y) <= round;
}

void Node::addEdge(Edge* e)
{
	if(!connectedEdges)
		connectedEdges = new std::list<Edge*>();
	connectedEdges->push_back(e);
}
void Node::removeEdge(Edge* e)
{
	if(connectedEdges)
		connectedEdges->remove(e);
}
