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

using namespace Osp::Base;
using namespace Osp::Base::Collection;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _PROFILE
extern void start_profile (void);
extern void end_profile (void);
#else
#define start_profile()
#define end_profile()
#endif

_EXPORT_ int OspMain(int argc, char *pArgv[]) {
	result r = E_SUCCESS;

	AppLog("Application started.");
	ArrayList* pArgs = new ArrayList();
	pArgs->Construct();
	for (int i = 0; i < argc; i++)
		pArgs->Add(*(new String(pArgv[i])));

	start_profile();
	r = Osp::App::Application::Execute(FactsAndFigures::CreateInstance, pArgs);
	if (IsFailed(r)) {
		AppLogException("Application execution failed-[%s].", GetErrorMessage(r));
		r &= 0x0000FFFF;
	} end_profile();

	pArgs->RemoveAll(true);
	delete pArgs;
	AppLog("Application finished.");

	return static_cast<int> (r);
}
#ifdef __cplusplus
}
#endif
