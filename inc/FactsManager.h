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
#ifndef FACTSMANAGER_H_
#define FACTSMANAGER_H_

#include <FBase.h>

using namespace Osp::Base;

class FactsManager {
public:
	static void Initialize(const String &lang);

	static String GetRandomQuote(void);

private:
	static String GetString(const String &res);

	static String __lang;
};

#endif
