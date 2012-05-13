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

#ifndef DMCOMPONENT_H
#define DMCOMPONENT_H

#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <dmview.h>
#include <dmcompilersettings.h>

#ifdef SWIG
#define DM_HELPER_DLL_EXPORT
#endif

namespace DM {
/** @ingroup DynaMind-Core
  * Basic class that contains to store informations in DynaMind
  *
  * All Objects like Nodes, Edges and Faces are derived from the component class.
  * Ever component can be identified by a unique ID created by the QT framework.
  * Components also manages the attributes.
  */
class Attribute;

class DM_HELPER_DLL_EXPORT Component
{
    friend class System;
protected:
    std::string uuid;
    std::map<std::string,Component*> childsview;
    std::map<std::string,Attribute*> attributesview;
    std::map<std::string,Component*> ownedchilds;
    std::map<std::string,Attribute*> ownedattributes;
    std::set<std::string> inViews;

    void removeView(const DM::View & view);


public:
    Component();
    Component(const Component& s);
    virtual ~Component();
    void setUUID(std::string uuid);
    std::string getUUID();

    bool addAttribute(Attribute newattribute);
    /** @brief add attribute to the component. If the Attribute already exists changeAttribute is called */
    bool addAttribute(std::string, double val);
    bool changeAttribute(Attribute newattribute);
    bool changeAttribute(std::string, double val);
    bool removeAttribute(std::string name);
    Attribute* getAttribute(std::string name);

    /** @brief Returns a map of all Attributes */
    const std::map<std::string, Attribute*> & getAllAttributes() const;
    bool addChild(Component *newcomponent);
    bool changeChild(Component *newcomponent);
    bool removeChild(std::string name);
    Component* getChild(std::string name);
    std::map<std::string, Component*> getAllChilds();
    void setView(std::string view);
    void setView(const DM::View & view);
    void createNewUUID();
    std::set<std::string> const &  getInViews() const;
    bool isInView(DM::View view) const;
    virtual Component* clone()=0;

};
typedef std::map<std::string, DM::Component*> ComponentMap;
}
#endif // COMPONENT_H
