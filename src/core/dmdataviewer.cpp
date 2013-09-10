/**
 * @file
 * @author  Markus Sengthaler <m.sengthaler@gmail.com>
 * @version 1.0
 * @section LICENSE
 * This file is part of DynaVibe
 *
 * Copyright (C) 2013	Markus Sengthaler

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

#include "dmdataviewer.h"
#include "dmcomponent.h"
#include "dmsystem.h"
#include "dmlogger.h"

using namespace DM;

DataViewer::DataViewer(const View& view, const System* system):
	name(view.getName()), type(view.getType()), system(system)
{
	foreach(const std::string& attributeName, view.getAllAttributes())
		attributes.insert(attributeName);
}

std::vector<Component*>	DataViewer::getComponents(const DataFilter& filter) const
{
	return components;
}

/*std::vector<Attribute*>	DataViewer::getAttributes(const std::string name, const DataFilter& filter)
{
}*/

void DataViewer::addComponent(Component* component)
{
	components.push_back(component);
}

bool DataViewer::removeComponent(Component* component)
{
	std::vector<Component*>::iterator it = find(components.begin(), components.end(), component);
	if(it != components.end())
	{
		return true;
		components.erase(it);
	}
	return false;
}

void DataViewer::update(const View& view)
{
	if(view.getName() != name)
	{
		Logger(Warning) << "DataViewer update failed";
		return;
	}

	attributeAccessMap.clear();
	componentAccess = (ACCESS)view.getAccessType();
	foreach(const std::string attributeName, view.getAllAttributes())
		attributeAccessMap[name] = view.getAttributeAccessType(attributeName);
}