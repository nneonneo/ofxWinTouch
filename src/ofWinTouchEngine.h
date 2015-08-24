#pragma once

#include "ofBaseTouchApp.h"

class ofWinTouchEngine{
    
public:
    ofWinTouchEngine(ofBaseTouchApp* ofAppPtr);
    ~ofWinTouchEngine(void);
    
	virtual void OnTouchDown(ofTouchEventArgs touch);
	virtual void OnTouchMove(ofTouchEventArgs touch);
	virtual void OnTouchUp(ofTouchEventArgs touch);
    
	LRESULT processTouch(HWND hWnd, WPARAM wParam, LPARAM lParam);
    
	void setAppPointer(ofBaseTouchApp * ofAppPtr);
	
private:
   ofBaseTouchApp * _ofAppPtr;
};
