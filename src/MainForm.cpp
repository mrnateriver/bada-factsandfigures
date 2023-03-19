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
#include "MainForm.h"
#include "FactsManager.h"
#include <FSystem.h>
#include <FApp.h>
#include <FIo.h>

using namespace Osp::System;
using namespace Osp::App;
using namespace Osp::Base::Utility;
using namespace Osp::Base::Collection;
using namespace Osp::Io;

MainForm::MainForm(void) {
	__pTextArea = null;
	__pCopyMessage = null;
	__copyAnimForward = true;

	__pFontBuffer = null;
	__pFont = null;

	__pCurrentText = null;
	__currentString = L"";
}

MainForm::~MainForm(void) {
	if (__pFontBuffer) {
		delete __pFontBuffer;
	}
	if (__pFont) {
		delete __pFont;
	}
	if (__pCurrentText) {
		delete __pCurrentText;
	}
}

bool MainForm::Initialize() {
	result res = Construct(FORM_STYLE_INDICATOR);
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to construct underlying form", GetErrorMessage(res));
		return false;
	}
	SetBackgroundColor(Color(0x242424, false));
	SetName(L"MAIN_FORM");
	AddTouchEventListener(*this);

	Label *header = new Label;
	res = header->Construct(Rectangle(0, 0, 480, 170), L"");
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to construct header label", GetErrorMessage(res));
		delete header;
	} else {
		AppResource *resource = Application::GetInstance()->GetAppResource();
		if (resource) {
			String filename = L"";
			resource->GetString(L"IDS_MAINFORM_HEADER_FILENAME", filename);

			Bitmap *bmp = resource->GetBitmapN(filename);
			if (bmp) {
				header->SetBackgroundBitmap(*bmp);
				delete bmp;
			}

			header->AddTouchEventListener(*this);
			AddControl(*header);
		} else {
			AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(res));
			delete header;
		}
	}

	int fontSize = 24;
	__pFont = new Font;
	File *file = new File;
	res = file->Construct(L"/Res/main_fnt.ttf", L"r", false);
	if (!IsFailed(res)) {
		__pFontBuffer = new ByteBuffer;
		__pFontBuffer->Construct(135 * 1024);

		if (!IsFailed(file->Read(*__pFontBuffer))) {
			__pFont->Construct(*__pFontBuffer, FONT_STYLE_PLAIN, fontSize);
		} else {
			delete __pFontBuffer;
			__pFontBuffer = null;

			__pFont->Construct(FONT_STYLE_PLAIN, fontSize);
		}
	} else {
		AppLogDebug("[%s]: failed to read main font file", GetErrorMessage(res));
		__pFont->Construct(FONT_STYLE_PLAIN, fontSize);
	}
	delete file;

	__pTextArea = new Label;
	res = __pTextArea->Construct(Rectangle(20, 180, 440, 528), L"");
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to construct main text area", GetErrorMessage(res));
		delete __pTextArea;
		return false;
	}
	__pTextArea->SetName(L"TEXT_AREA");
	__pTextArea->AddTouchEventListener(*this);
	AddControl(*__pTextArea);

	Button *refreshButton = new Button;
	res = refreshButton->Construct(Rectangle(13,708,48,51), L"");
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to construct refresh button", GetErrorMessage(res));
		delete refreshButton;
	} else {
		refreshButton->SetActionId(ACTION_REFRESH);
		refreshButton->AddActionEventListener(*this);

		AppResource *resource = Application::GetInstance()->GetAppResource();
		if (resource) {
			Bitmap *bmp = resource->GetBitmapN(L"refresh_button_normal.png");
			if (bmp) {
				refreshButton->SetNormalBackgroundBitmap(*bmp);
				delete bmp;
			}
			bmp = resource->GetBitmapN(L"refresh_button_pressed.png");
			if (bmp) {
				refreshButton->SetPressedBackgroundBitmap(*bmp);
				delete bmp;
			}
			AddControl(*refreshButton);
		} else {
			AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(res));
			delete refreshButton;
		}
	}

	Button *wikiButton = new Button;
	res = wikiButton->Construct(Rectangle(61,708,48,51), L"");
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to construct wiki button", GetErrorMessage(res));
		delete wikiButton;
	} else {
		wikiButton->SetActionId(ACTION_WIKI);
		wikiButton->AddActionEventListener(*this);

		AppResource *resource = Application::GetInstance()->GetAppResource();
		if (resource) {
			Bitmap *bmp = resource->GetBitmapN(L"wiki_button_normal.png");
			if (bmp) {
				wikiButton->SetNormalBackgroundBitmap(*bmp);
				delete bmp;
			}
			bmp = resource->GetBitmapN(L"wiki_button_pressed.png");
			if (bmp) {
				wikiButton->SetPressedBackgroundBitmap(*bmp);
				delete bmp;
			}
			AddControl(*wikiButton);
		} else {
			AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(res));
			delete wikiButton;
		}
	}

	Button *copyButton = new Button;
	res = copyButton->Construct(Rectangle(109,708,48,51), L"");
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to construct copy button", GetErrorMessage(res));
		delete copyButton;
	} else {
		copyButton->SetActionId(ACTION_COPY);
		copyButton->AddActionEventListener(*this);

		AppResource *resource = Application::GetInstance()->GetAppResource();
		if (resource) {
			Bitmap *bmp = resource->GetBitmapN(L"copy_button_normal.png");
			if (bmp) {
				copyButton->SetNormalBackgroundBitmap(*bmp);
				delete bmp;
			}
			bmp = resource->GetBitmapN(L"copy_button_pressed.png");
			if (bmp) {
				copyButton->SetPressedBackgroundBitmap(*bmp);
				delete bmp;
			}
			AddControl(*copyButton);
		} else {
			AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(res));
			delete copyButton;
		}
	}

	__pCopyMessage = new Label;
	res = __pCopyMessage->Construct(Rectangle(159,708,321,51), L"");
	if (IsFailed(res)) {
		AppLogDebug("[%s]: failed to construct copy message label", GetErrorMessage(res));

		delete __pCopyMessage;
		__pCopyMessage = null;
	} else {
		AppResource *resource = Application::GetInstance()->GetAppResource();
		if (resource) {
			String msg = L"Text copied.";
			resource->GetString(L"IDS_TEXT_COPIED_MSG", msg);

			__pCopyMessage->SetName(L"COPY_MSG");
			__pCopyMessage->SetText(msg);
			__pCopyMessage->SetTextColor(Color::COLOR_WHITE);
			__pCopyMessage->SetTextConfig(25, LABEL_TEXT_STYLE_NORMAL);
			__pCopyMessage->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
			__pCopyMessage->SetTextVerticalAlignment(ALIGNMENT_MIDDLE);

			AddControl(*__pCopyMessage);
			__pCopyMessage->SetShowState(false);
		} else {
			AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(res));

			delete __pCopyMessage;
			__pCopyMessage = null;
		}
	}

	String lang = L"en";
	AppResource *resource = Application::GetInstance()->GetAppResource();
	if (resource) {
		resource->GetString(L"IDS_LANG_IDENT", lang);
	} else {
		AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(GetLastResult()));
	}
	FactsManager::Initialize(lang);

	return !IsFailed(res);
}

result MainForm::OnInitializing(void) {
	AppRegistry *reg = Application::GetInstance()->GetAppRegistry();
	if (reg) {
		int val = 0;
		if (reg->Get(L"HAS_SHOWN_TUTORIAL", val) == E_KEY_NOT_FOUND) {
			reg->Add(L"HAS_SHOWN_TUTORIAL", 1);
		}
		if (val) {
			SetText(FactsManager::GetRandomQuote());
		} else {
			AppResource *resource = Application::GetInstance()->GetAppResource();
			if (resource) {
				String msg = L"Click anywhere on the screen to switch to next fact. You can also do that from the buttons below, as well as open Wikipedia and copy current text. And don\'t forget to click on <link=http://en.wikipedia.org/wiki/Hyperlink>links</link> in the text!";
				resource->GetString(L"IDS_TUTORIAL_MESSAGE", msg);

				SetText(msg);
			} else {
				AppLogDebug("failed to acquire application resources");
				SetText(FactsManager::GetRandomQuote());
			}
		}
	} else {
		AppLogDebug("failed to acquire application registry");
		SetText(FactsManager::GetRandomQuote());
	}
	return E_SUCCESS;
}

result MainForm::OnTerminating(void) {
	return E_SUCCESS;
}

void MainForm::OnActionPerformed(const Control &source, int actionId) {
	switch (actionId) {
	case ACTION_WIKI: {
		String lang = L"en";
		AppResource *resource = Application::GetInstance()->GetAppResource();
		if (resource) {
			resource->GetString(L"IDS_LANG_IDENT", lang);
		} else {
			AppLogDebug("[%s]: failed to acquire application resources", GetErrorMessage(GetLastResult()));
		}

		ArrayList* pDataList = new ArrayList();
		pDataList->Construct();

		String* pData = new String(L"url:" + lang + L".wikipedia.org");
		pDataList->Add(*pData);

		AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_PROVIDER_BROWSER, APPCONTROL_OPERATION_VIEW);
		if (pAc) {
			pAc->Start(pDataList, null);
			delete pAc;
		}
		pDataList->RemoveAll(true);
		delete pDataList;
		break;
	}
	case ACTION_COPY: {
		ClipboardItem item;
		item.Construct(CLIPBOARD_DATA_TYPE_TEXT, __currentString);

		Clipboard* pClipboard = Clipboard::GetInstance();
		if (pClipboard) {
			pClipboard->CopyItem(item);
		}

		if (__pCopyMessage) {
			ControlAnimator *control = __pCopyMessage->GetControlAnimator();
			FloatAnimation anim(0.0, 1.0, 1500, ANIMATION_INTERPOLATOR_LINEAR);

			if (control->AddControlAnimatorEventListener(*this) == E_UNSUPPORTED_OPERATION) {
				__pCopyMessage->SetShowState(true);
				RequestRedraw(true);

				Timer *timer = new Timer;
				timer->Construct(*this);
				timer->Start(2000);
			} else {
				__copyAnimForward = true;
				control->StartUserAnimation(ANIMATION_TARGET_ALPHA, anim);
			}
		}
		break;
	}
	case ACTION_REFRESH: {
		SetText(FactsManager::GetRandomQuote());
		break;
	}
	default:
		break;
	}
}

void MainForm::SetText(const String &txt) {
	if (__pCurrentText) {
		__pCurrentText->RemoveAll(true);
		delete __pCurrentText;
	}
	__currentString = L"";

	__pCurrentText = new EnrichedText;
	__pCurrentText->Construct(Dimension(440, 528));
	__pCurrentText->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	__pCurrentText->SetTextAbbreviationEnabled(false);
	__pCurrentText->SetHorizontalAlignment(TEXT_ALIGNMENT_CENTER);
	__pCurrentText->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);

	String curURL = L"";
	String curTag = L"<link=";
	int startIndex = 0, curIndex = 0;
	while (!IsFailed(txt.IndexOf(curTag, startIndex, curIndex))) {
		String normal;
		txt.SubString(startIndex, curIndex - startIndex, normal);

		if (!normal.IsEmpty()) {
			TextElement *elem = new TextElement;
			if (!curURL.IsEmpty()) {
				elem->Construct(normal, LINK_TYPE_URL, curURL);
				elem->SetTextColor(Color(0x83d7f6, false));
			} else {
				elem->Construct(normal);
				elem->SetTextColor(Color::COLOR_WHITE);
			}
			elem->SetFont(*__pFont);
			__pCurrentText->Add(*elem);

			__currentString.Append(normal);
		}

		startIndex = curIndex + curTag.GetLength();
		if (!curURL.IsEmpty()) {
			curURL = L"";
			curTag = L"<link=";
		} else {
			if (!IsFailed(txt.IndexOf(L">", startIndex, curIndex))) {
				txt.SubString(startIndex, curIndex - startIndex, curURL);
				curTag = L"</link>";
				startIndex = curIndex + 1;
			}
		}
	}
	if (startIndex < txt.GetLength()) {
		String normal;
		txt.SubString(startIndex, txt.GetLength() - startIndex, normal);

		TextElement *elem = new TextElement;
		elem->Construct(normal);
		elem->SetTextColor(Color::COLOR_WHITE);
		elem->SetFont(*__pFont);
		__pCurrentText->Add(*elem);

		__currentString.Append(normal);
	}

	Canvas *canv = new Canvas;
	result res = E_SUCCESS;
	res = canv->Construct(Rectangle(0, 0, 440, 528));
	if (!IsFailed(res)) {
		canv->SetFont(*__pFont);
		canv->DrawText(Point(0, 0), *__pCurrentText);

		Bitmap *prBmp = new Bitmap;
		res = prBmp->Construct(*canv, Rectangle(0,0,440,528));
		if (!IsFailed(res)) {
			__pTextArea->SetBackgroundBitmap(*prBmp);
		}
		delete prBmp;
	}
	delete canv;

	RequestRedraw(true);
}

void MainForm::OnTouchReleased(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo) {
	if (source.GetName().Equals(L"TEXT_AREA", true)) {
		if (__pCurrentText) {
			LinkInfo linkInfo;
			if (!IsFailed(__pCurrentText->GetLinkInfoFromPosition(currentPosition, linkInfo))) {
				ArrayList* pDataList = new ArrayList();
				pDataList->Construct();

				String* pData = new String(L"url:" + linkInfo.GetLink());
				pDataList->Add(*pData);

				AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_PROVIDER_BROWSER, APPCONTROL_OPERATION_VIEW);
				if (pAc) {
					pAc->Start(pDataList, null);
					delete pAc;
				}
				pDataList->RemoveAll(true);
				delete pDataList;
			} else {
				SetText(FactsManager::GetRandomQuote());
			}
		}
	} else {
		SetText(FactsManager::GetRandomQuote());
	}
}

void MainForm::OnControlAnimationFinished(ControlAnimator &source, Control &control) {
	if (__copyAnimForward && control.GetName().Equals(L"COPY_MSG", true)) {
		Timer *timer = new Timer;
		timer->Construct(*this);
		timer->Start(2000);
	}
}

void MainForm::OnTimerExpired(Timer &timer) {
	ControlAnimator *control = __pCopyMessage->GetControlAnimator();
	FloatAnimation anim(1.0, 0.0, 1500, ANIMATION_INTERPOLATOR_LINEAR);

	control->AddControlAnimatorEventListener(*this);

	__copyAnimForward = false;
	control->StartUserAnimation(ANIMATION_TARGET_ALPHA, anim);

	delete &timer;
}
