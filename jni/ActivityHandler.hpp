#ifndef _ACTIVITYHANDLER_HPP_
#define _ACTIVITYHANDLER_HPP_

#include "Types.hpp"

	/*
	 *   ActivityHandler - абстрактный класс, который предоставляет каркас для создания
	 *  классов реализующих activity с полноценным lifecycle.
	 */
    class ActivityHandler {
    public:
        virtual ~ActivityHandler() {};

        virtual status onActivate() = 0;
        virtual void onDeactivate() = 0;
        virtual status onStep() = 0;

        virtual void onStart() {};
        virtual void onResume() {};
        virtual void onPause() {};
        virtual void onStop() {};
        virtual void onDestroy() {};

        virtual void onSaveState(void** pData, size_t* pSize) {};
        virtual void onConfigurationChanged() {};
        virtual void onLowMemory() {};

        virtual void onCreateWindow() {};
        virtual void onDestroyWindow() {};
        virtual void onGainFocus() {};
        virtual void onLostFocus() {};
    };

#endif
