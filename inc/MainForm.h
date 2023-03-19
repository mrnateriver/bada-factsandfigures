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
#ifndef _MAINFORM_H_
#define _MAINFORM_H_

#include <FBase.h>
#include <FUi.h>
//workaround for strange bug in SDK
#include <FUiAnimations.h>

using namespace Osp::Base;
using namespace Osp::Base::Runtime;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Ui::Animations;
using namespace Osp::Graphics;

class MainForm: public Form, public IActionEventListener, public ITouchEventListener, public IControlAnimatorEventListener, public ITimerEventListener {
public:
	MainForm(void);
	virtual ~MainForm(void);

	bool Initialize(void);

private:
	void SetText(const String &txt);

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnActionPerformed(const Control &source, int actionId);

	virtual void OnTouchPressed(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo) { }
	virtual void OnTouchLongPressed(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo) { }
	virtual void OnTouchReleased(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo);
	virtual void OnTouchMoved(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo) { }
	virtual void OnTouchDoublePressed(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo) { }
	virtual void OnTouchFocusIn(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo) { }
	virtual void OnTouchFocusOut(const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo) { }

	virtual void OnControlAnimationStarted(ControlAnimator &source, Control &control) { }
	virtual void OnControlAnimationFinished(ControlAnimator &source, Control &control);
	virtual void OnControlAnimationStopped(ControlAnimator &source, Control &control) { }

	virtual void OnTimerExpired(Timer &timer);

	static const int ACTION_WIKI = 501;
	static const int ACTION_REFRESH = 502;
	static const int ACTION_COPY = 503;

	Label *__pTextArea;
	Label *__pCopyMessage;
	bool __copyAnimForward;

	ByteBuffer *__pFontBuffer;
	Font *__pFont;

	EnrichedText *__pCurrentText;
	String __currentString;
};

#endif
