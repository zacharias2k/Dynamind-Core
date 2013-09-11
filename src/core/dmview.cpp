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
#include <dmcomponent.h>

using namespace DM;

View::View(std::string name, int type, int accesstypeGeometry)
{
	this->name = name;
	this->type = type;
	this->accesstypeGeometry = accesstypeGeometry;

}

View::View() 
{
	this->name = "";
	this->type = -1;
}

View::View(const View& ref):
	name(ref.name), type(ref.type), 
	accesstypeGeometry(ref.accesstypeGeometry),
	ownedAttributes(ref.ownedAttributes), 
	attributeTypes(ref.attributeTypes),
	attributeLinks(ref.attributeLinks)
{
}

void View::addAttribute(std::string name) {
	this->ownedAttributes[name] = WRITE;
	this->attributeTypes[name] = Attribute::NOTYPE;
}

void View::getAttribute(std::string name) {
	this->ownedAttributes[name] = READ;
	this->attributeTypes[name] = Attribute::NOTYPE;
}
void View::modifyAttribute(std::string name) {
	this->ownedAttributes[name] = MODIFY;
	this->attributeTypes[name] = Attribute::NOTYPE;
}

std::vector<std::string> View::getWriteAttributes() const {
	std::vector<std::string> attrs;
	for (std::map<std::string, int>::const_iterator it = this->ownedAttributes.begin(); it != this->ownedAttributes.end(); ++it)
		if (it->second > READ)
			attrs.push_back(it->first);

	return attrs;
}

std::vector<std::string> View::getReadAttributes() const {
	std::vector<std::string> attrs;
	for (std::map<std::string, int>::const_iterator it = this->ownedAttributes.begin(); it != this->ownedAttributes.end(); ++it)
		if (it->second < WRITE)
			attrs.push_back(it->first);

	return attrs;
}

bool View::reads() const
{
	if (this->accesstypeGeometry < WRITE)
		return true;
	for (std::map<std::string, int>::const_iterator it = this->ownedAttributes.begin(); it != this->ownedAttributes.end(); ++it)
		if (it->second < WRITE)
			return true;

	return false;
}

bool View::writes() const
{
	if (this->accesstypeGeometry > READ)
		return true;
	for (std::map<std::string, int>::const_iterator it = this->ownedAttributes.begin(); it != this->ownedAttributes.end(); ++it)
		if (it->second > READ)
			return true;

	return false;
}

Attribute::AttributeType View::getAttributeType(std::string name) const
{
	std::map<std::string, Attribute::AttributeType>::const_iterator it = attributeTypes.find(name);
	if(it != attributeTypes.end())
		return it->second;
	return Attribute::NOTYPE;
}

ACCESS View::getAttributeAccessType(std::string name) const
{
	int a = ACCESS();
	map_contains(&ownedAttributes, name, a);
	return (ACCESS)a;
}

std::vector<std::string> View::getAllAttributes() const
{
	std::vector<std::string> names;
	for (std::map<std::string, int>::const_iterator it = this->ownedAttributes.begin(); it != this->ownedAttributes.end(); ++it)
		names.push_back(it->first);
	return names;
}

void View::setAttributeType(std::string name, Attribute::AttributeType type)
{
	this->attributeTypes[name] = type;
}

void View::addLinks(string name, View linkedView)
{
	addLinks(name, linkedView.getName());
}

void View::addLinks(string name, std::string linkedViewName)
{
	this->addAttribute(name);
	this->setAttributeType(name, Attribute::LINK);
	this->attributeLinks[name] = linkedViewName;
}

std::vector<std::string> View::getNamesOfLinks()
{
	std::vector<std::string> namesOfView;
	typedef std::map<std::string, std::string> stringmap;
	for (stringmap::const_iterator it = attributeLinks.begin(); it != attributeLinks.end(); ++it) {
		namesOfView.push_back(it->first);
	}
	return namesOfView;

}

std::string View::getNameOfLinkedView(string name)
{
	return this->attributeLinks[name];
}


