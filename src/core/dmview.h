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

#ifndef DMVIEW_H
#define DMVIEW_H

#include <map>
#include <vector>
#include <string>
#include <dmcompilersettings.h>
#include <dmattribute.h>

namespace DM {

enum ViewAccess
{
	READ,
	MODIFY,
	WRITE
};

/**
  * @class DM::View
  *
  * @brief Data in DynaMind are organised in views. A view provides a description of a
  * component and its attributes and how they are accessed in the module.
  *
  * @ingroup DynaMind-Core
  *
  *
  * A view is defined by:
  *
  * @section Type
  * Following Type are supported in DynaMind:
  *
  * -   NODE
  * -   EDGE
  * -   FACE
  * -   SUBSYSTEM
  * -   RASTERDATA
  *
  * @section AccessType
  * The AccessType defines the assecctype geometric component.
  * Following AccessTypes are supported:
  * - Read: Geometry stays the same and no new components are added to the view
  * - Write: Gemetroy of existing components can be changed and new components can be added to the view
  * - Modify: Gemeotry can be modified but no new components are added to the view
  *
  * @section Attributes
  * Every component can hold attributes. In the view the access to the attributes of the component is defined.
  * Following methods can be used to access attributes within a module
  *
  * - addAttribute(std::string name)
  * - getAttribute(std::string name)
  * - modifyAttribute(std::string name)
  *
  * @section Dummy Component
  * To every View a dummy component is created. The dummy component is used to track the changes made by the modules.
  * @author Christian Urich
  * @date 11.03.2012
  */
class DM_HELPER_DLL_EXPORT View
{
public:
	/** @brief Default constructor to create a new view */
	View(std::string name, int type, int accesstypeGeometry = READ);
	View();
	View(const View& ref);

	/** @brief add attributes that to write by added by the module*/
	void addAttribute(std::string name);

	/** @brief add attributes that to write by read by the module*/
	void getAttribute(std::string name);

	/** @brief add attributes that to write by modified by the module*/
	void modifyAttribute(std::string name);

	/** @brief Get name of the View*/
	std::string const & getName() const {return this->name;}

	/** @brief Set name of the view */
	void setName(std::string name){this->name = name;}

	/** @brief Return a list of attributes that are added to the component */
	std::vector<std::string>  getWriteAttributes  () const;

	/** @brief Return a list of attributes that are read */
	std::vector<std::string>  getReadAttributes  () const;

	/** @brief Set Type */
	void setType(int type) {this->type=type;}

	/** @brief Return Type */
	int const & getType() const {return type;}

	/** @brief Return AccessType of the Geometry */
	int const & getAccessType() const{return accesstypeGeometry;}

	/** @brief set AccessType of the Geometry */
	void setAccessType(int Type) {this->accesstypeGeometry = Type;}

	/** @brief Returns true if the accesstype of the geomtry or from one attribute is modify or read */
	bool reads() const;

	/** @brief Returns true if the accesstype of the geomtry or from one attribute is modify or write */
	bool writes() const;

	/** @brief Returns Attribute Type */
	Attribute::AttributeType getAttributeType(std::string name) const;

	/** @brief Returns Attribute Type */
	ViewAccess getAttributeAccessType(std::string name) const;

	/** @brief Sets Attribute Type */
	void setAttributeType(std::string name, Attribute::AttributeType type);

	/** @brief Adds link attribute */
	void addLinks(std::string name, View linkedView);	// backwards comp.
	void addLinks(std::string name, std::string linkedViewName);

	/** @brief Returns names of object that link to other views */
	std::vector<std::string> getNamesOfLinks();

	/** @brief Returns names of object the linked view */
	std::string getNameOfLinkedView(std::string name);

	std::vector<std::string> getAllAttributes() const;
private:
	int type;
	std::string name;
	int accesstypeGeometry;

	typedef std::pair<Attribute::AttributeType, ViewAccess> TypeAccessPair;
	std::map<std::string, TypeAccessPair> linkedAttributes;

	std::map<std::string, std::string>	linkedViews;
};
}


#endif // DMVIEW_H
