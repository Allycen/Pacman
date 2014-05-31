#ifndef _EVENTLOOP_HPP_
#define _EVENTLOOP_HPP_

#include "ActivityHandler.hpp"
#include "InputHandler.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h> // native_app_glue - API цель которого - объединять
									 // NativeActivity и native code  между собой


	/*   События, совершаемые в приложении
		должны обрабатываться в "цикле событий" (event loop)
	 */
    class EventLoop {
    public:
        EventLoop(android_app* pApplication);
        void run(ActivityHandler* pActivityHandler,
        		 InputHandler* pInputHandler);

    protected:
        void activate();
        void deactivate();

        void processAppEvent(int32_t pCommand);
        int32_t processInputEvent(AInputEvent* pEvent);

    private:
        static void callback_event(android_app* pApplication,
            int32_t pCommand);

        static int32_t callback_input(android_app* pApplication,
                    AInputEvent* pEvent);

    private:
        bool mEnabled; 			   // сохраняет состояние приложения когда оно активно/на паузе
        bool mQuit; 			   // показывает, хочет ли обработчик событий выйти из цикла
        android_app* mApplication; // android app - объект, содержащий информацию о
								   // native activity, его состоянии, окне, очереди событий и т.д
        ActivityHandler* mActivityHandler;
        InputHandler* mInputHandler;
    };

#endif
