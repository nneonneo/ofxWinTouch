#include "ofWinTouchEngine.h"
#include "ofEvents.h"
#include "ofAppRunner.h"

ofWinTouchEngine::ofWinTouchEngine(ofBaseTouchApp * ofAppPtr){}

ofWinTouchEngine::~ofWinTouchEngine(void) {}

static ofTouchEventArgs winTouchToOFTouch(TOUCHINPUT &ti) {
	ofTouchEventArgs touch; // initialize touch values to zero

	touch.x = TOUCH_COORD_TO_PIXEL((float)ti.x)-ofGetWindowPositionX();
	touch.y = TOUCH_COORD_TO_PIXEL((float)ti.y)-ofGetWindowPositionY();
	touch.id = ti.dwID;
	if(ti.dwMask & TOUCHINPUTMASKF_CONTACTAREA) {
		touch.width = (float)ti.cxContact / 100;
		touch.height = (float)ti.cyContact / 100;
	} else {
		touch.width = 0;
		touch.height = 0;
	}
	touch.pressure = sqrt(touch.width * touch.height);
	touch.majoraxis = max(touch.width, touch.height);
	touch.minoraxis = min(touch.width, touch.height);

	return touch;
}

LRESULT ofWinTouchEngine::processTouch(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	BOOL bHandled = FALSE;
	UINT cInputs = LOWORD(wParam);
	PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
	if (pInputs){
		if (GetTouchInputInfo((HTOUCHINPUT)lParam, cInputs, pInputs, sizeof(TOUCHINPUT))){
			for (UINT i=0; i < cInputs; i++){
				TOUCHINPUT ti = pInputs[i];
				//do something with each touch input entry
				if (ti.dwFlags & TOUCHEVENTF_DOWN){
					OnTouchDown(winTouchToOFTouch(ti));
				}
				else if (ti.dwFlags & TOUCHEVENTF_MOVE){
					OnTouchMove(winTouchToOFTouch(ti));
				}
				if (ti.dwFlags & TOUCHEVENTF_UP){
					OnTouchUp(winTouchToOFTouch(ti));
				}
			}
			bHandled = TRUE;
		} else {
			/* handle the error here */
		}
		delete [] pInputs;
	} else {
		/* handle the error here, probably out of memory */
	}
	if (bHandled){
		// if you handled the message, close the touch input handle and return
		CloseTouchInputHandle((HTOUCHINPUT)lParam);
		return 0;
	} else {
		// if you didn't handle the message, let DefWindowProc handle it
		return DefWindowProc(hWnd, WM_TOUCH, wParam, lParam);
	}
}

void ofWinTouchEngine::OnTouchDown(ofTouchEventArgs touch) {
	if(_ofAppPtr) {
		touch.type = ofTouchEventArgs::Type::down;
		_ofAppPtr->touchDown(touch);
		ofNotifyEvent( ofEvents().touchDown, touch );
	}
}

void ofWinTouchEngine::OnTouchMove(ofTouchEventArgs touch) {
	if(_ofAppPtr) {
		touch.type = ofTouchEventArgs::Type::move;
		_ofAppPtr->touchMoved(touch);
		ofNotifyEvent( ofEvents().touchMoved, touch );
	}
}

void ofWinTouchEngine::OnTouchUp(ofTouchEventArgs touch) {
	if(_ofAppPtr) {
		touch.type = ofTouchEventArgs::Type::up;
		_ofAppPtr->touchUp(touch);
		ofNotifyEvent( ofEvents().touchUp, touch );
	}
}

void ofWinTouchEngine::setAppPointer(ofBaseTouchApp * ofAppPtr){
	_ofAppPtr = ofAppPtr;
}
