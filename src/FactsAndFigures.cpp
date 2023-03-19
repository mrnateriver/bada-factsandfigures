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
#include "FactsAndFigures.h"
#include "MainForm.h"

FactsAndFigures::FactsAndFigures() {
}

FactsAndFigures::~FactsAndFigures() {
}

Application *FactsAndFigures::CreateInstance(void) {
	return new FactsAndFigures();
}

bool FactsAndFigures::OnAppInitializing(AppRegistry &appRegistry) {
	MainForm *pMainForm = new MainForm();
	bool res = pMainForm->Initialize();
	if (!res) {
		delete pMainForm;
		return false;
	}

	Frame *pFrame = GetAppFrame()->GetFrame();
	pFrame->AddControl(*pMainForm);

	pFrame->SetCurrentForm(*pMainForm);

	pMainForm->Draw();
	pMainForm->Show();

	return true;
}

bool FactsAndFigures::OnAppTerminating(AppRegistry &appRegistry, bool forcedTermination) {
	return true;
}

void FactsAndFigures::OnForeground(void) {
}

void FactsAndFigures::OnBackground(void) {
}

void FactsAndFigures::OnLowMemory(void) {
}

void FactsAndFigures::OnBatteryLevelChanged(BatteryLevel batteryLevel) {
}
