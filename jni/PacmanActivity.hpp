#ifndef _PACMANACTIVITY_HPP_
#define _PACMANACTIVITY_HPP_

#include "ActivityHandler.hpp"
#include "GraphicsService.hpp"
#include "TimeService.hpp"
#include "Types.hpp"
#include "Screen.hpp"
#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "Sound.hpp"
#include "SoundService.hpp"
#include "InputService.hpp"
#include "SpriteBatcher.hpp"
#include "OverlapTester.hpp"

	/*
	 *    PacmanActivity - класс, объект которого - activity с
	 *   полноценным lifecycle. Для представления экрана и внешнего взаимодействия
	 *   использует объекты MainMenuScreen и GameScreen.
	 */

	class MainMenuScreen;
	class GameScreen;

    class PacmanActivity : public ActivityHandler {
    public:
    	  PacmanActivity(android_app* pApplication,
    			  GraphicsService* pGraphicsService,
					      TimeService* pTimeService,
					      SoundService* pSoundService,
					      InputService* pInputService);
    protected:
        status onActivate();
        void onDeactivate();
        status onStep();

        void onStart();
        void onResume();
        void onPause();
        void onStop();
        void onDestroy();

        void onSaveState(void** pData, size_t* pSize);
        void onConfigurationChanged();
        void onLowMemory();

        void onCreateWindow();
        void onDestroyWindow();
        void onGainFocus();
        void onLostFocus();

    public:
        SpriteBatcher* getSpriteBatcher();
        OverlapTester* getOverlapTester();
        void setMainMenuScreen();
        void setGameScreen();


    private:
        SpriteBatcher   mSpriteBatcher;
        OverlapTester   mOverlapTester;

        GraphicsService* mGraphicsService;
        TimeService*     mTimeService;
        SoundService*    mSoundService;
        InputService*    mInputService;
        Assets mAssets;

        Screen* mCurrentScreen;
        MainMenuScreen* mMainMenuScreen;
        GameScreen* mGameScreen;

        int32_t mFPS;
        float mTimeFPS;

    };

#endif
