/*
 * Copyright (c) 2016 Evgenii Dobrovidov
 * This file is part of "Facts and Figures".
 *
 * "Facts and Figures" is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * "Facts and Figures" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with "Facts and Figures".  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _FACTSANDFIGURES_H_
#define _FACTSANDFIGURES_H_

#include <FApp.h>
#include <FSystem.h>

using namespace Osp::App;
using namespace Osp::System;

class FactsAndFigures: public Application {
public:
	static Application *CreateInstance(void);

public:
	FactsAndFigures();
	~FactsAndFigures();

public:
	bool OnAppInitializing(AppRegistry &appRegistry);
	bool OnAppTerminating(AppRegistry &appRegistry, bool forcedTermination = false);
	void OnForeground(void);
	void OnBackground(void);
	void OnLowMemory(void);
	void OnBatteryLevelChanged(BatteryLevel batteryLevel);
};

#endif
