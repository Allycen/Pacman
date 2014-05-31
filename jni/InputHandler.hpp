#ifndef _INPUTHANDLER_HPP_
#define _INPUTHANDLER_HPP_

#include <android/input.h>

	// абстрактный класс
    class InputHandler {
    public:
        virtual ~InputHandler() {};

        virtual bool onTouchEvent(AInputEvent* pEvent) = 0;
        virtual bool onKeyEvent(AInputEvent* pEvent) = 0;  // !

    };

#endif
