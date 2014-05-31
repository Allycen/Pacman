#include "InputService.hpp"
#include "Log.hpp"

#include <android_native_app_glue.h>
#include <cmath>


    InputService::InputService(android_app* pApplication,
        const int32_t& pWidth, const int32_t& pHeight) :
        mWidth(pWidth), mHeight(pHeight)
    {
        mApplication = pApplication;
        mX = -1;
        mY = -1;
        mPreviousX = -1;
        mPreviousY = -1;
        mTouchAction = -1;
        mIsTouchedUp = true;
        mIsBtnMenuPressed = false;
    }

    InputService::~InputService() {
    	mApplication = NULL;
    }

    status InputService::start() {
       // Log::info("Starting InputService.");
        if ((mWidth == 0) || (mHeight == 0)) return STATUS_KO;
        return STATUS_OK;
    }

    bool InputService::onTouchEvent(AInputEvent* pEvent) {

    	mX = AMotionEvent_getX(pEvent,0);
    	mY = AMotionEvent_getY(pEvent,0);
    	mTouchAction = AMotionEvent_getAction(pEvent);

    	if(AMotionEvent_getAction(pEvent) == 1) {
    		mIsTouchedUp = true;
    	} else {
    		mIsTouchedUp = false;
    	}

    	return true;
    }

    bool InputService::onKeyEvent(AInputEvent* pEvent) {
    	 if (AKeyEvent_getAction(pEvent) == AKEY_EVENT_ACTION_DOWN) {

    		 switch (AKeyEvent_getKeyCode(pEvent)) {
    	         case AKEYCODE_VOLUME_UP:
					 return false;
    	         case AKEYCODE_VOLUME_DOWN:
    	             return false;
    	     }
    	 } else {
    		 switch (AKeyEvent_getKeyCode(pEvent)) {

    	         case AKEYCODE_MENU:
    	              mIsBtnMenuPressed = true;
    	              break;
				 case AKEYCODE_BACK:
					 mIsBtnBackPressed = true;
					 break;
			     case AKEYCODE_VOLUME_UP:
			    	 return false;
    	         case AKEYCODE_VOLUME_DOWN:
    	        	 return false;
    	            }
    	        }

    	return true;
    }


    void InputService::update() {
    	if(mIsTouchedUp) {
    		mPreviousX = mX;
    		mPreviousY = mY;
    		mX = -1; mY = -1;
    		mTouchAction = -1;
    	} else {
    		mPreviousX = mX;
    		mPreviousY = mY;
    	}

    	mIsBtnMenuPressed = false;
    	mIsBtnBackPressed = false;
    }

    float InputService::getTouchX() {
    	return mX;
    }

    float InputService::getTouchY() {
    	return mY;
    }

    float InputService::getPreviousTouchX() {
    	return mPreviousX;
    }

    float InputService::getPreviousTouchY() {
    	return mPreviousY;
    }

    int32_t InputService::getTouchAction() {
    	return mTouchAction;
    }

    bool InputService::menuBtnPressed() {
    	return mIsBtnMenuPressed;
    }

    bool InputService::backBtnPressed() {
    	return mIsBtnBackPressed;
    }

    void InputService::setParams(float pX, float pY) {
    	mX = pX;
    	mY = pY;
    }

    void InputService::closeApplication() {
    	ANativeActivity_finish(mApplication->activity);
    }

    /* События input service

     * касание экрана
         AMotionEvent_getAction=%d,        AMotionEvent_getAction(pEvent)
         AMotionEvent_getFlags=%d, 		   AMotionEvent_getFlags(pEvent)
         AMotionEvent_getMetaState=%d, 	   AMotionEvent_getMetaState(pEvent)
         AMotionEvent_getEdgeFlags=%d, 	   AMotionEvent_getEdgeFlags(pEvent)
         AMotionEvent_getDownTime=%lld,    AMotionEvent_getDownTime(pEvent)
         AMotionEvent_getEventTime=%lld,   AMotionEvent_getEventTime(pEvent)
         AMotionEvent_getXOffset=%f,       AMotionEvent_getXOffset(pEvent)
         AMotionEvent_getYOffset=%f,       AMotionEvent_getYOffset(pEvent)
         AMotionEvent_getXPrecision=%f,    AMotionEvent_getXPrecision(pEvent)
         AMotionEvent_getYPrecision=%f,    AMotionEvent_getYPrecision(pEvent)
         AMotionEvent_getPointerCount=%d,  AMotionEvent_getPointerCount(pEvent)
         AMotionEvent_getRawX=%f,          AMotionEvent_getRawX(pEvent, 0)
         AMotionEvent_getRawY=%f,          AMotionEvent_getRawY(pEvent, 0)
         AMotionEvent_getX=%f,             AMotionEvent_getX(pEvent, 0)
         AMotionEvent_getY=%f,             AMotionEvent_getY(pEvent, 0)
         AMotionEvent_getPressure=%f,      AMotionEvent_getPressure(pEvent, 0)
         AMotionEvent_getSize=%f,          AMotionEvent_getSize(pEvent, 0)
         AMotionEvent_getOrientation=%f,   AMotionEvent_getOrientation(pEvent, 0)
         AMotionEvent_getTouchMajor=%f,    AMotionEvent_getTouchMajor(pEvent, 0)
         AMotionEvent_getTouchMinor=%f,    AMotionEvent_getTouchMinor(pEvent, 0)

      * нажатие на кнопку
         AKeyEvent_getAction=%d, 		   AKeyEvent_getAction(pEvent)
         AKeyEvent_getFlags=%d, 		   AKeyEvent_getFlags(pEvent)
         AKeyEvent_getKeyCode=%d, 		   AKeyEvent_getKeyCode(pEvent)
         AKeyEvent_getScanCode=%d, 		   AKeyEvent_getScanCode(pEvent)
         AKeyEvent_getMetaState=%d, 	   AKeyEvent_getMetaState(pEvent)
         AKeyEvent_getRepeatCount=%d, 	   AKeyEvent_getRepeatCount(pEvent)
         AKeyEvent_getDownTime=%lld, 	   AKeyEvent_getDownTime(pEvent)
         AKeyEvent_getEventTime=%lld, 	   AKeyEvent_getEventTime(pEvent)
     */
