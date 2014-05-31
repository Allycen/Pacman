#include "EventLoop.hpp"
#include "Log.hpp"



    EventLoop::EventLoop(android_app* pApplication) {
        mEnabled = false; 						 // приложение не активно
        mQuit = false;							 // обработчик не выходит из цикла
        mApplication = pApplication;
        mActivityHandler = NULL;
        mInputHandler = NULL;
        mApplication->userData = this;
        mApplication->onAppCmd = callback_event; // onAppCmd - код ответа в момент
												 // когда происходит действие(приложение не
												 // не активно, потеряло фокус и т.д)
        mApplication->onInputEvent = callback_input;
    }

    void EventLoop::run(ActivityHandler* pActivityHandler,
						InputHandler* pInputHandler) {
        int32_t lResult;
        int32_t lEvents;
        android_poll_source* lSource; // android_poll_source - описывает наличие
									  // или отсутствие в данный момент события

        app_dummy();
        mActivityHandler = pActivityHandler;
        mInputHandler = pInputHandler;

        Log::info("Starting event loop");
        while (true) {// while(true) - безконечный цикл, выход из которого состоится
					  // когда пользователь выйдет из приложения

            while ((lResult = ALooper_pollAll(mEnabled ? 0 : -1,
                         NULL, &lEvents, (void**) &lSource)) >= 0) {
            	// while(lResult..) - "ожидающий событий цикл"
                if (lSource != NULL) {
                  //  Log::info("Processing an event");
                    lSource->process(mApplication, lSource);
                }
                if (mApplication->destroyRequested) {
                    Log::info("Exiting event loop");
                    return;
                }
            }

            if ((mEnabled) && (!mQuit)) {
                if (mActivityHandler->onStep() != STATUS_OK) {
                    mQuit = true;
                    ANativeActivity_finish(mApplication->activity);
                }
            }
        }
    }

    void EventLoop::activate() {
        if ((!mEnabled) && (mApplication->window != NULL)) {
            mQuit = false; mEnabled = true;
            if (mActivityHandler->onActivate() != STATUS_OK) {
                mQuit = true;
                deactivate();
                ANativeActivity_finish(mApplication->activity);
            }
        }
    }

    void EventLoop::deactivate() {
        if (mEnabled) {
            mActivityHandler->onDeactivate();
            mEnabled = false;
        }
    }

    void EventLoop::processAppEvent(int32_t pCommand) {
        switch (pCommand) {
        case APP_CMD_CONFIG_CHANGED:
            mActivityHandler->onConfigurationChanged();
            break;
        case APP_CMD_INIT_WINDOW:
            mActivityHandler->onCreateWindow();
            break;
        case APP_CMD_DESTROY:
            mActivityHandler->onDestroy();
            break;
        case APP_CMD_GAINED_FOCUS:
            activate();
            mActivityHandler->onGainFocus();
            break;
        case APP_CMD_LOST_FOCUS:
            mActivityHandler->onLostFocus();
            deactivate();
            break;
        case APP_CMD_LOW_MEMORY:
            mActivityHandler->onLowMemory();
            break;
        case APP_CMD_PAUSE:
            mActivityHandler->onPause();
            deactivate();
            break;
        case APP_CMD_RESUME:
            mActivityHandler->onResume();
            break;
        case APP_CMD_SAVE_STATE:
            mActivityHandler->onSaveState(&mApplication->savedState,
                &mApplication->savedStateSize);
            break;
        case APP_CMD_START:
            mActivityHandler->onStart();
            break;
        case APP_CMD_STOP:
            mActivityHandler->onStop();
            break;
        case APP_CMD_TERM_WINDOW:
            mActivityHandler->onDestroyWindow();
            deactivate();
            break;
        default:
            break;
        }
    }

    void EventLoop::callback_event(android_app* pApplication,
        int32_t pCommand) {
        EventLoop& lEventLoop = *(EventLoop*) pApplication->userData;
        lEventLoop.processAppEvent(pCommand);
    }

    int32_t EventLoop::processInputEvent(AInputEvent* pEvent) {

        int32_t lEventType = AInputEvent_getType(pEvent);
        switch (lEventType) {
        case AINPUT_EVENT_TYPE_MOTION:
            switch (AInputEvent_getSource(pEvent)) {
            case AINPUT_SOURCE_TOUCHSCREEN:
                return mInputHandler->onTouchEvent(pEvent);
                break;
            }
            break;

        case AINPUT_EVENT_TYPE_KEY:
             return mInputHandler->onKeyEvent(pEvent);
             break;
        }
        return 0;
    }

    int32_t EventLoop::callback_input(android_app* pApplication,
        AInputEvent* pEvent) {
        EventLoop& lEventLoop = *(EventLoop*) pApplication->userData;
        return lEventLoop.processInputEvent(pEvent);
    }

