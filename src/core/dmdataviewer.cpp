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
#include "dmdatafilter.h"
#include "dmview.h"
#include "dmcomponent.h"
#include "dmsystem.h"
#include "dmlogger.h"
#include "dmderivedsystem.h"
#include <vector>
#include <dmnode.h>

using namespace DM;

DataViewer::DataViewer(const View& view):
	currentViewDefinition()
{
	currentViewDefinition = view;
	currentViewDefinition.clearFilters();
	update(view);
}

const std::vector<Component*>& DataViewer::getComponents() const
{
	return components;
}

void DataViewer::addComponent(Component* component)
{
	components.push_back(component);
}

bool DataViewer::removeComponent(Component* component)
{
	std::vector<Component*>::iterator it = find(components.begin(), components.end(), component);
	if(it != components.end())
	{
		components.erase(it);
		return true;
	}
	return false;
}

bool ApplyFilter(Component* c, DataFilter* filter)
{
	if(filter->attributeName.empty())
	{
		if(Node* n = dynamic_cast<Node*>(c))
		{
			double value = n->get(filter->coord);
			
			if(filter->op == DataFilter::GREATER)
				return value > filter->value;
			else if(filter->op == DataFilter::GREATEREQUAL)
				return value >= filter->value;
			else if(filter->op == DataFilter::LESS)
				return value < filter->value;
			else if(filter->op == DataFilter::LESSEQUAL)
				return value <= filter->value;
			else if(filter->op == DataFilter::EQUAL)
				return value == filter->value;
		}
	}
}

void ApplyFilters(std::vector<Component*>& componentList, std::vector<DataFilter*> filters)
{
	std::vector<Component*> newComponentList;
	foreach(Component* c, componentList)
	{
		bool excluded = false;
		foreach(DataFilter* filter, filters)
			if(!ApplyFilter(c, filter))
				excluded = true;

		if(!excluded)
			newComponentList.push_back(c);
	}

	componentList = newComponentList;
}

void DataViewer::update(const View& view)
{
	if(view.getName() != currentViewDefinition.getName())
	{
		Logger(Warning) << "DataViewer update failed";
		return;
	}
	// check if we need to completly renew filteredComponents
	bool renewFilteredComponents = false;
	foreach(DataFilter* newFilter, currentViewDefinition.getFilters())
	{
		bool isMissing = true;
		foreach(DataFilter* oldFilter, view.getFilters())
			if(*oldFilter == *newFilter)
			{
				isMissing = false;
				break;
			}
		if(isMissing)
		{
			renewFilteredComponents = true;
			break;
		}
	}

	if(renewFilteredComponents)
	{
		// recreate filteredComponents
		filteredComponents = components;
		ApplyFilters(filteredComponents, view.getFilters());
	}
	else
	{
		// adapt filteredComponents
		std::vector<DataFilter*> newFilters;
		foreach(DataFilter* newFilter, view.getFilters())
		{
			bool isNew = true;
			foreach(DataFilter* oldFilter, currentViewDefinition.getFilters())
				if(*oldFilter == *newFilter)
				{
					isNew = false;
					break;
				}

			if(isNew)
				newFilters.push_back(newFilter);
		}
		ApplyFilters(filteredComponents, newFilters);
	}
	currentViewDefinition = view;
}

void DataViewer::migrateComponent(const Component* src, Component* dest)
{
	if(src != dest && dest != NULL)
	{
		std::vector<Component*>::iterator it = find(components.begin(), components.end(), src);
		if(it != components.end())
			*it = dest;
	}
}

void DataViewer::migrateAllComponents(DerivedSystem* system)
{
	std::vector<Component*> predecComponents = components;
	foreach(Component* c, predecComponents)
		migrateComponent(c, system->getComponent(c->getUUID()));
}

const View* DataViewer::getCurrentViewDefinition()
{
	return &currentViewDefinition;
}
