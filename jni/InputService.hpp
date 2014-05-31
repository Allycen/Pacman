#ifndef _INPUTSERVICE_HPP_
#define _INPUTSERVICE_HPP_

#include "InputHandler.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>


    class InputService : public InputHandler {
    public:
        InputService(android_app* pApplication,
					 const int32_t& pWidth,
					 const int32_t& pHeight);
        ~InputService();

        status start();				// запускаем сервис

        float getTouchX();			// получаем коорд. x прикосновения в текущий момент
        float getTouchY();			// получаем коорд. y прикосновения в текущий момент
        float getPreviousTouchX();	// получаем коорд. x прикосновения прошедшей итерации
        float getPreviousTouchY();	// получаем коорд. y прикосновения прошедшей итерации
        int32_t getTouchAction();   // получаем тип прикосновения (нажатие, перемещение, отпускание)

        bool menuBtnPressed();		// нажата ли кнопка menu в текущий момент
        bool backBtnPressed();		// нажата ли кнопка back в текущий момент

        void update();             // очистка информации для последующей итерации
        void setParams(float pX, float pY);

        void closeApplication();	// завершаем работу приложения
    public:
        bool onTouchEvent(AInputEvent* pEvent);
        bool onKeyEvent(AInputEvent* pEvent);

        static const int32_t TOUCH_DOWN = 0;
        static const int32_t TOUCH_UP = 1;
        static const int32_t TOUCH_DRAGGED = 2;

    private:

        android_app* mApplication;
        const int32_t &mWidth,
					  &mHeight;

        float mX,
			  mY,
			  mPreviousX,
			  mPreviousY;

        int32_t mTouchAction;

        bool mIsTouchedUp;
        bool mIsBtnMenuPressed;	// нажата ли кнопка Menu
        bool mIsBtnBackPressed; // нажата ли кнопка Back
    };

#endif
