#include "PacmanActivity.hpp"
#include "Log.hpp"


	  PacmanActivity::PacmanActivity( android_app* pApplication,
									   GraphicsService* pGraphicsService,
									   TimeService* pTimeService,
									   SoundService* pSoundService,
									   InputService* pInputService)
	  : mAssets(pApplication ,"texture.png")
    {
        Log::info("Creating Pacman Activity");
        mGraphicsService = pGraphicsService;
        mGraphicsService->setAssets(&mAssets);
        mTimeService = pTimeService;
        mSoundService = pSoundService;
        mSoundService->setAssets(&mAssets);
        mInputService = pInputService;

        mMainMenuScreen = new MainMenuScreen;	// создаем главный экран
        mGameScreen = new GameScreen;			// создаем игровой экран
        mCurrentScreen = mMainMenuScreen;		// текущий экран - главное меню

        mTimeFPS = 0.0f;
        mFPS = 0;
    }


    status PacmanActivity::onActivate() {
        Log::info("Activating Packman Activity");


        if (mGraphicsService->start() != STATUS_OK){
        	return STATUS_KO;
        }

        if (mInputService->start() != STATUS_OK) {
        return STATUS_KO;
        }

        if (mSoundService->start() != STATUS_OK) {
            return STATUS_KO;
       }

        mMainMenuScreen->init(this, mGraphicsService,
									mTimeService,
							        mInputService,
							        mSoundService);
        mGameScreen->init(this, mGraphicsService,
								mTimeService,
							    mInputService,
							    mSoundService);
        mCurrentScreen->setConfigs();

        mTimeService->reset();
        return STATUS_OK;
    }

    void PacmanActivity::onDeactivate() {
        Log::info("Deactivating Packman Activity");
        mGraphicsService->stop();
        mSoundService->stop();
    }

    status PacmanActivity::onStep() {

    	// FPS Counter
    	if(mTimeFPS > 1) {
    		Log::info("FPS= %d", mFPS);
    		mTimeFPS = 0.0f;
    		mFPS = 0;
    	} else {
    		mFPS++;
    		mTimeFPS += mTimeService->elapsed();
    	}

    	mTimeService->update();							 // обновляем синхронизацию
    	mGraphicsService->updateGlConfigs();			 // обновляем OpenGl конфигурацию
    	mCurrentScreen->update(mTimeService->elapsed()); // обновляем содержание экрана
    	mInputService->update();						 // обновляем события взаимодействия
    	if (mGraphicsService->update() != STATUS_OK) {   // обновляем контекст OpenGl
    	return STATUS_KO;
    	}
    	return STATUS_OK;
    }

    void PacmanActivity::onStart() {
        Log::info("onStart");
    }

    void PacmanActivity::onResume() {
        Log::info("onResume");
    }

    void PacmanActivity::onPause() {
        Log::info("onPause");
    }

    void PacmanActivity::onStop() {
        Log::info("onStop");
    }

    void PacmanActivity::onDestroy() {
        Log::info("onDestroy");

        // Освобождаем память выделенную на объекты
        // ответственные за отображение экрана
        delete mMainMenuScreen,
			   mGameScreen;
        mMainMenuScreen = NULL;
        mGameScreen = NULL;

        // Обнуляем указатели
        mGraphicsService = NULL;
        mTimeService = NULL;
        mSoundService = NULL;
        mInputService = NULL;
        mCurrentScreen = NULL;
    }

    void PacmanActivity::onSaveState(void** pData, size_t* pSize) {
        Log::info("onSaveInstanceState");
    }

    void PacmanActivity::onConfigurationChanged() {
        Log::info("onConfigurationChanged");
    }

    void PacmanActivity::onLowMemory() {
        Log::info("onLowMemory");
    }

    void PacmanActivity::onCreateWindow() {
        Log::info("onCreateWindow");
    }

    void PacmanActivity::onDestroyWindow() {
        Log::info("onDestroyWindow");
    }

    void PacmanActivity::onGainFocus() {
        Log::info("onGainFocus");
    }

    void PacmanActivity::onLostFocus() {
        Log::info("onLostFocus");
    }


    SpriteBatcher* PacmanActivity::getSpriteBatcher() {
    	return &mSpriteBatcher;
    }

    OverlapTester* PacmanActivity::getOverlapTester() {
    	return &mOverlapTester;
    }

    void PacmanActivity::setMainMenuScreen() {
    	mCurrentScreen = mMainMenuScreen;
    	mCurrentScreen->setConfigs();
    }

    void PacmanActivity::setGameScreen() {
    	mCurrentScreen = mGameScreen;
    	mCurrentScreen->setConfigs();
    }

