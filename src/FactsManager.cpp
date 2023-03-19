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
#include "FactsManager.h"
#include <FSystem.h>
#include <FIo.h>
#include <FText.h>
#include <FApp.h>

using namespace Osp::App;
using namespace Osp::System;
using namespace Osp::Base::Utility;
using namespace Osp::Io;
using namespace Osp::Text;

extern String FactsManager::__lang;

void FactsManager::Initialize(const String &lang) {
	__lang = lang;

	long long ticks;
	SystemTime::GetTicks(ticks);//milliseconds

	Math::Srand((int)ticks);
}

String FactsManager::GetRandomQuote(void) {
	File *file = new File;
	result res = file->Construct(L"/Res/data_" + __lang, L"r");
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to open the file for reading", GetErrorMessage(res));
		delete file;
		return GetString(L"IDS_PARSE_FACTS_ERROR");
	}

	file->Seek(FILESEEKPOSITION_END, 0);
	ByteBuffer *buf = new ByteBuffer;
	buf->Construct(file->Tell());
	file->Seek(FILESEEKPOSITION_BEGIN, 0);

	file->Read(*buf);
	delete file;

	buf->SetPosition(0);

	int count = 0;
	buf->GetInt(count);
	if (count > 0) {
		int rndIndex = Math::Rand();
		if (rndIndex >= count) {
			rndIndex = rndIndex % count;
		}

		int index = 0;
		int offset = 4;
		while (offset < buf->GetLimit() && index < rndIndex) {
			byte val = 0xFF;
			buf->GetByte(offset, val);
			if (val == 0) {//terminator
				index++;
			}
			offset++;
		}

		char *ptr = ((char*)buf->GetPointer()) + offset;

		String str;
		res = StringUtil::Utf8ToString(ptr, str);
		delete buf;

		if (IsFailed(res)) {
			AppLogDebug("[%s]: failed to parse string", GetErrorMessage(res));
			return GetString(L"IDS_PARSE_FACTS_ERROR");
		} else {
			return str;
		}
	} else {
		delete buf;
		return L"";
	}
}

String FactsManager::GetString(const String &res) {
	String str = res;
	AppResource *resource = Application::GetInstance()->GetAppResource();
	if (resource) {
		resource->GetString(res, str);
	} else {
		AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(GetLastResult()));
	}
	return str;
}
