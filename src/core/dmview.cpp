/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @author  Michael Mair <abroxos@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaMite
 *
 * Copyright (C) 2011  Christian Urich, Michael Mair

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

#include "dmview.h"
#include <dmmodule.h>
#include <dmattribute.h>
#include <dmdatafilter.h>

using namespace DM;

View::View(std::string name, Components type, ACCESS geometryAccess):
	name(name), type(type), geometryAccess(geometryAccess)
{
}

View::View():
	name(""), type(NOTYPE), geometryAccess(NOACCESS)
{
}

View::View(const View& ref):
	name(ref.name), type(ref.type), 
	geometryAccess(ref.geometryAccess),
	linkedAttributes(ref.linkedAttributes),
	linkedViews(ref.linkedViews),
	filters(ref.filters)
{
}

View::~View()
{
	foreach(DataFilter* f, filters)
		delete f;
}

void View::addAttribute(std::string name) 
{
	linkedAttributes[name] = TypeAccessPair(Attribute::NOTYPE, WRITE);
}

void View::getAttribute(std::string name) 
{
	linkedAttributes[name] = TypeAccessPair(Attribute::NOTYPE, READ);
}
void View::modifyAttribute(std::string name) 
{
	linkedAttributes[name] = TypeAccessPair(Attribute::NOTYPE, MODIFY);
}

std::vector<std::string> View::getWriteAttributes() const 
{
	std::vector<std::string> attrs;
	for(std::map<std::string, TypeAccessPair>::const_iterator it = linkedAttributes.cbegin();
		it != linkedAttributes.cend();	++it)
		if (it->second.second > READ)
			attrs.push_back(it->first);

	return attrs;
}

std::vector<std::string> View::getReadAttributes() const 
{
	std::vector<std::string> attrs;
	for(std::map<std::string, TypeAccessPair>::const_iterator it = linkedAttributes.cbegin();
		it != linkedAttributes.cend();	++it)
		if (it->second.second < WRITE)
			attrs.push_back(it->first);

	return attrs;
}

bool View::reads() const
{
	if (this->geometryAccess < WRITE)
		return true;
	for(std::map<std::string, TypeAccessPair>::const_iterator it = linkedAttributes.cbegin();
		it != linkedAttributes.cend();	++it)
		if (it->second.second < WRITE)
			return true;

	return false;
}

bool View::writes() const
{
	if (this->geometryAccess > READ)
		return true;
	for(std::map<std::string, TypeAccessPair>::const_iterator it = linkedAttributes.cbegin();
		it != linkedAttributes.cend();	++it)
		if (it->second.second > READ)
			return true;

	return false;
}

Attribute::AttributeType View::getAttributeType(std::string name) const
{
	std::map<std::string, TypeAccessPair>::const_iterator it = linkedAttributes.find(name);
	if(it != linkedAttributes.end())
		return it->second.first;
	return Attribute::NOTYPE;
}

ACCESS View::getAttributeAccessType(std::string name) const
{
	TypeAccessPair entry;
	map_contains(&linkedAttributes, name, entry);
	return entry.second;
}

std::vector<std::string> View::getAllAttributes() const
{
	std::vector<std::string> names;
	for(std::map<std::string, TypeAccessPair>::const_iterator it = linkedAttributes.cbegin();
		it != linkedAttributes.cend();	++it)
		names.push_back(it->first);
	return names;
}

void View::setAttributeType(std::string name, Attribute::AttributeType type)
{
	this->linkedAttributes[name].first = type;
}

void View::addLinks(string name, View linkedView)
{
	addLinks(name, linkedView.getName());
}

void View::addLinks(string name, std::string linkedViewName)
{
	this->addAttribute(name);
	this->setAttributeType(name, Attribute::LINK);
	linkedViews[name] = (linkedViewName);
}

std::vector<std::string> View::getNamesOfLinks()
{
	std::vector<std::string> namesOfView;
	typedef std::map<std::string, std::string> stringmap;
	for (stringmap::const_iterator it = linkedViews.begin(); it != linkedViews.end(); ++it)
		namesOfView.push_back(it->first);
	return namesOfView;

}

std::string View::getNameOfLinkedView(string name)
{
	return this->linkedViews[name];
}

void View::addFilter(const DataFilter& filter)
{
	filters.push_back(new DataFilter(filter));
}
