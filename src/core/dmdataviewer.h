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

#ifndef DMDATAVIEWER_H
#define DMDATAVIEWER_H

#include <string>
#include <vector>
#include "dmview.h"

namespace DM {
	
class Component;
class System;
class DerivedSystem;

class DM_HELPER_DLL_EXPORT DataViewer
{
public:
	DataViewer(const View& view, System* owningSystem);
	DataViewer(const DataViewer& ref, System* owningSystem);
	
	const View*	getCurrentViewDefinition();
	const std::vector<Component*>& getComponents();

	void	addComponent(Component* component);
	bool	removeComponent(Component* component);

	void	update(const View& view);
	void	migrateComponent(const Component* src, Component* dest);
	void	migrateAllComponents(DerivedSystem* targetSystem);
private:
	DataViewer(const DataViewer& ref){}	// prevent from copy without system init
	View	currentViewDefinition;
	System*	owningSystem;
	
	std::vector<Component*>	components;
	std::vector<Component*>	filteredComponents;
};

}

#endif // DMDATAVIEWER_H
