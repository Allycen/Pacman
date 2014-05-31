#include "MainMenuScreen.hpp"


	MainMenuScreen::~MainMenuScreen() {
		/*
		 *  Обнуляем указатели. Удаление объектов займется
		 * класс, который их создал.
		 */
		mActivityHandler = NULL;
		mGraphicsService = NULL;
		mTimeService = NULL;
		mInputService = NULL;
		mSoundService = NULL;
		mSpriteBatcher = NULL;
		mOverlapTester = NULL;
		mTextReg = NULL;
	}

	void MainMenuScreen::init(ActivityHandler* pPacmanActivity,
							  GraphicsService* pGraphicsService,
							  TimeService* pTimeService,
							  InputService* pInputService,
							  SoundService* pSoundService)
	{
		//Log::info("Initializating MainMenuScreen");

		mActivityHandler = pPacmanActivity;
		mGraphicsService = pGraphicsService;
		mTimeService = pTimeService;
		mInputService = pInputService;
		mSoundService = pSoundService;

		mSpriteBatcher = ((PacmanActivity *)mActivityHandler)->getSpriteBatcher();
		mOverlapTester = ((PacmanActivity *)mActivityHandler)->getOverlapTester();

		if(mSoundService->musicEnabled()) {
			 mSoundService->playBGM("BGM.mp3");
		}
	}

	void MainMenuScreen::setConfigs() {
		mState = STATE_INIT;
		mStateTime = 0.0f;

		mButton.Play  = mButton.STATE_ABLE;
		mButton.About = mButton.STATE_ABLE;
		mButton.Yes   = mButton.STATE_ABLE;
		mButton.No    = mButton.STATE_ABLE;
		mButton.Music = mButton.STATE_ABLE;
		mButton.Sound = mButton.STATE_ABLE;

		mAmNyam.state = mAmNyam.STATE_GAZING;
		mAmNyam.jumpDirection = mAmNyam.DIRECTION_UP;


		// коэффицунты васштабирования по x и y
		float lk_x = float(mGraphicsService->getWidth()) / 800.0f;
		float lk_y = float(mGraphicsService->getHeight()) / 480.0f;

		// начало отсчета от левого верхнего края!!!
				mPlayBounds.setInfo			(200.0f * lk_x, 300.0f * lk_y,  240.0f * lk_x, 60.0f    * lk_x);
				mMusicBounds.setInfo		(660.0f * lk_x, 355.0f * lk_y,  90.0f  * lk_x,  90.0f   * lk_x);
				mSoundBounds.setInfo		(680.0f * lk_x, 250.0f * lk_y,  90.0f  * lk_x,  90.0f   * lk_x);
				mAmNyamBounds.setInfo		(470.0f * lk_x, 395.0f * lk_y,  100.0f * lk_x, 100.0f   * lk_x);
				mYesBounds.setInfo			(310.0f * lk_x, 300.0f * lk_y,  175.0f * lk_x, 70.0f    * lk_x);
				mNoBounds.setInfo			(510.0f * lk_x, 300.0f * lk_y,  140.0f * lk_x, 70.0f    * lk_x);
				mBackgroundBounds.setInfo	(400.0f * lk_x, 240.0f * lk_y,  800.0f * lk_x, 480.0f   * lk_y);
				mLogoBounds.setInfo			(400.0f * lk_x, 100.0f * lk_y,  600.0f * lk_x, 100.0f   * lk_x);
				mRopeBounds.setInfo			(470.0f * lk_x, 182.0f * lk_y,  6.0f   * lk_x,   159.3f * lk_x);
				mCandyBounds.setInfo		(463.0f * lk_x, 100.35f* lk_y,  35.0f  * lk_x,  70.0f   * lk_x);
				mExitBounds.setInfo			(405.0f * lk_x, 175.0f * lk_y,  280.0f * lk_x, 70.0f    * lk_x);
				mWindowBounds.setInfo		(400.0f * lk_x, 240.0f * lk_y,  450.0f * lk_x, 270.0f   * lk_x);
				mMindBounds.setInfo			(400.0f * lk_x, 335.0f * lk_y,  85.0f  * lk_x,  85.0f   * lk_x);

				mAmNyam.posY = 85.0f   * lk_y;
				mScreenWidth = 800.0f  * lk_x;
				mScreenHeight = 480.0f * lk_y;

				mk_x = lk_x;
				mk_y = lk_y;
	}


	void MainMenuScreen::present() {
		glClear(GL_COLOR_BUFFER_BIT);

		switch(mState) {
			case STATE_INIT:
				presentInit();
				break;
			case STATE_MENU:
				presentMenu();
				break;
		    case STATE_EXIT:
			    presentExit();
			    break;
		    case STATE_UNINIT:
		    	presentUninit();
		    	break;
		}
	}

	void MainMenuScreen::presentMenu() {

	// рисуем background
		mSpriteBatcher->draw(mBackgroundBounds.getX(),
							 mBackgroundBounds.getY(),
							 mBackgroundBounds.getWidth(),
							 mBackgroundBounds.getHeight(),
					         mGraphicsService->getAssets()->mBackground);

	// рисуем логотит "Packman"
		mSpriteBatcher->draw(mLogoBounds.getX(),
							 mScreenHeight - mLogoBounds.getY(),
							 mLogoBounds.getWidth(),
							 mLogoBounds.getHeight(),
					         mGraphicsService->getAssets()->mLogo);

	// рисуем кнопку "Play"
		if(mButton.Play == mButton.STATE_CLAMPED) {
			mSpriteBatcher->draw(mPlayBounds.getX(),
								 mScreenHeight - mPlayBounds.getY(),
								 mPlayBounds.getWidth(),
								 mPlayBounds.getHeight(),
							     mGraphicsService->getAssets()->mPlay_clamped);
		} else {
			mSpriteBatcher->draw(mPlayBounds.getX(),
								 mScreenHeight - mPlayBounds.getY(),
								 mPlayBounds.getWidth(),
								 mPlayBounds.getHeight(),
							     mGraphicsService->getAssets()->mPlay);
		}

	// рисуем значок музыки
		if(mSoundService->musicEnabled()) {
			mSpriteBatcher->draw(mMusicBounds.getX(),
								 mScreenHeight - mMusicBounds.getY(),
								 mMusicBounds.getWidth(),
								 mMusicBounds.getHeight(),
					             mGraphicsService->getAssets()->mMusicOn);
		} else {
			mSpriteBatcher->draw(mMusicBounds.getX(),
								 mScreenHeight - mMusicBounds.getY(),
								 mMusicBounds.getWidth(),
								 mMusicBounds.getHeight(),
					             mGraphicsService->getAssets()->mMusicOff);
		}

	// рисуем значок звука
		if(mSoundService->soundEnabled()) {
			mSpriteBatcher->draw(mSoundBounds.getX(),
								 mScreenHeight - mSoundBounds.getY(),
								 mSoundBounds.getWidth(),
								 mSoundBounds.getHeight(),
				                 mGraphicsService->getAssets()->mSoundOn);
		} else {
			mSpriteBatcher->draw(mSoundBounds.getX(),
								 mScreenHeight - mSoundBounds.getY(),
								 mSoundBounds.getWidth(),
								 mSoundBounds.getHeight(),
				                 mGraphicsService->getAssets()->mSoundOff);
		}

	// рисуем веревку
		mSpriteBatcher->draw(mRopeBounds.getX(),
							(mScreenHeight - mRopeBounds.getY()),
							 mRopeBounds.getWidth(),
							 mRopeBounds.getHeight(),
					         mGraphicsService->getAssets()->mRope);

	// рисуем карамельку
		 mSpriteBatcher->draw(mCandyBounds.getX(),
							  mScreenHeight - mCandyBounds.getY()-mRopeBounds.getHeight(),
							  mCandyBounds.getWidth(),
							  mCandyBounds.getHeight(),
							  mGraphicsService->getAssets()->mCandyHook);

	// рисуем ам няма

		if(mAmNyam.state == mAmNyam.STATE_GAZING) {
			mSpriteBatcher->draw(mAmNyamBounds.getX(),
								 mScreenHeight - mAmNyamBounds.getY(),
							     mAmNyamBounds.getWidth(),
							     mAmNyamBounds.getHeight(),
								 mGraphicsService->getAssets()->mAmNyam_state1);
		} else {
			mSpriteBatcher->draw(mAmNyamBounds.getX(), mAmNyam.posY,
								 mAmNyamBounds.getWidth(),
								 mAmNyamBounds.getHeight(),
								(mScreenHeight - mAmNyamBounds.getY() - mAmNyam.posY)/2,
								 mGraphicsService->getAssets()->mAmNyam_state1);
		}


	// рисуем просьбу ам няма
		if(mAmNyam.state == mAmNyam.STATE_GAZING) {
			mSpriteBatcher->draw(mMindBounds.getX(),
								 mScreenHeight - mMindBounds.getY(),
								 mMindBounds.getWidth(),
								 mMindBounds.getHeight(),
						         mGraphicsService->getAssets()->mMind);
		}


	// если надо, меняем направление прыжка ам няма
		if(mAmNyam.posY > (mScreenHeight - mAmNyamBounds.getY() + 50.0f*mk_y)) {
			mAmNyam.jumpDirection = mAmNyam.DIRECTION_DOWN;
		} else {
			if(mAmNyam.posY < (mScreenHeight - mAmNyamBounds.getY())) {
			   mAmNyam.state = mAmNyam.STATE_GAZING;
			   mAmNyam.posY = (mScreenHeight - mAmNyamBounds.getY());
			}
		}
	}

	void MainMenuScreen::presentExit() {
		this->presentMenu();

	// рисуем окошко выхода
		mSpriteBatcher->draw(mWindowBounds.getX(),
							 mScreenHeight - mWindowBounds.getY(),
							 mWindowBounds.getWidth(),
							 mWindowBounds.getHeight(),
							 mGraphicsService->getAssets()->mConfigMenu);
	// рисуем Exit
		mSpriteBatcher->draw(mExitBounds.getX(),
							 mScreenHeight - mExitBounds.getY(),
							 mExitBounds.getWidth(),
							 mExitBounds.getHeight(),
							 mGraphicsService->getAssets()->mExit);
	// рисуем кнопку Yes
		if(mButton.Yes == mButton.STATE_ABLE) {
			mSpriteBatcher->draw(mYesBounds.getX(),
								 mScreenHeight - mYesBounds.getY(),
								 mYesBounds.getWidth(),
								 mYesBounds.getHeight(),
								 mGraphicsService->getAssets()->mYes);
		} else {
			mSpriteBatcher->draw(mYesBounds.getX(),
								 mScreenHeight - mYesBounds.getY(),
								 mYesBounds.getWidth(),
								 mNoBounds.getHeight(),
								 mGraphicsService->getAssets()->mYes_clamped);
			}
	// рисуем кнопку No
		if(mButton.No == mButton.STATE_ABLE) {
			mSpriteBatcher->draw(mNoBounds.getX(),
								 mScreenHeight - mNoBounds.getY(),
								 mNoBounds.getWidth(),
								 mNoBounds.getHeight(),
								 mGraphicsService->getAssets()->mNo);
		} else {
			mSpriteBatcher->draw(mNoBounds.getX(),
								 mScreenHeight - mNoBounds.getY(),
								 mNoBounds.getWidth(),
								 mNoBounds.getHeight(),
								 mGraphicsService->getAssets()->mNo_clamped);
		}

	}

	void MainMenuScreen::presentInit() {
		this->presentMenu();

	// рисуем затемнение
		mSpriteBatcher->draw(mGraphicsService->getWidth()/2,
							 mGraphicsService->getHeight()/2,
							 mGraphicsService->getWidth() - DARK_VELOCITY * mStateTime * mGraphicsService->getWidth(),
							 mGraphicsService->getHeight() - DARK_VELOCITY * mStateTime * mGraphicsService->getHeight(),
							 mGraphicsService->getAssets()->mBlackColor);
	}

	void MainMenuScreen::presentUninit() {
		this->presentMenu();

	// рисуем затемнение
		mSpriteBatcher->draw(mGraphicsService->getWidth()/2,
							 mGraphicsService->getHeight()/2,
							 DARK_VELOCITY * mStateTime * mGraphicsService->getWidth(),
							 DARK_VELOCITY * mStateTime * mGraphicsService->getHeight(),
							 mGraphicsService->getAssets()->mBlackColor);
	}

	void MainMenuScreen::update(float pDeltaTime) {

		switch(mState) {
			case STATE_INIT:
				updateInit(pDeltaTime);
				break;
			case STATE_MENU:
				updateMenu(pDeltaTime);
				break;
			case STATE_EXIT:
				updateExit(pDeltaTime);
				break;
			case STATE_UNINIT:
				updateUninit(pDeltaTime);
				break;
		}
		this->present();
		mStateTime += pDeltaTime;
	}



	void MainMenuScreen::updateMenu(float pDeltaTime) {

	// проверяем нажатие на кнопку меню или back
		if((mInputService->menuBtnPressed() || mInputService->backBtnPressed()) && mStateTime > 0.1f) {
			mState = this->STATE_EXIT;
			mStateTime = 0.0f;
		// если звук включен, воспроизводим клик
			if(mSoundService->soundEnabled()) {
				mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
			}
		}


	// изменяем координаты Ам Няме если он в состоянии прыжка
		if(mAmNyam.state == mAmNyam.STATE_INJUMP) {
			if(mAmNyam.jumpDirection == mAmNyam.DIRECTION_UP) {
				mAmNyam.posY += pDeltaTime * mAmNyam.VELOCITY_JUMP;

			} else {
				mAmNyam.posY -= pDeltaTime * mAmNyam.VELOCITY_JUMP;
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DOWN) {

		   // проверям на нажатие кнопки Play
			if(mOverlapTester->pointInRectangle(&mPlayBounds, mInputService->getTouchX(),
															 mInputService->getTouchY())) {
				mButton.Play = mButton.STATE_CLAMPED;
			}
		   // проверям нажатие кнопки About
			if(mOverlapTester->pointInRectangle(&mAboutBounds, mInputService->getTouchX(),
															  mInputService->getTouchY())) {
				mButton.About = mButton.STATE_CLAMPED;
			}
		   // проверяем нажатие кнопки музыки
			if(mOverlapTester->pointInRectangle(&mMusicBounds, mInputService->getTouchX(),
															  mInputService->getTouchY())) {
				mButton.Music = mButton.STATE_CLAMPED;
			}
			// проверяем нажатие кнопки звука
			if(mOverlapTester->pointInRectangle(&mSoundBounds, mInputService->getTouchX(),
															  mInputService->getTouchY())) {
				mButton.Sound = mButton.STATE_CLAMPED;
			}
			// проверяем нажатие на ам няма
			if(mOverlapTester->pointInRectangle(&mAmNyamBounds, mInputService->getTouchX(),
															   mInputService->getTouchY())) {
				mButton.AmNyam = mButton.STATE_CLAMPED;
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DRAGGED) {

		   // проверяем перемещение пальца по экрана в рамках кнопки Play
			if(!mOverlapTester->pointInRectangle(&mPlayBounds, mInputService->getTouchX(),
															  mInputService->getTouchY())) {
				mButton.Play = mButton.STATE_ABLE;
			}
		   // проверяем перемещение пальца по экрана в рамках кнопки About
			if(!mOverlapTester->pointInRectangle(&mAboutBounds, mInputService->getTouchX(),
															   mInputService->getTouchY())) {
				//mBtnAboutClamped = false;
				mButton.About = mButton.STATE_ABLE;
			}
		   // проверяем перемещение пальца по экрана в рамках ам няма
			if(!mOverlapTester->pointInRectangle(&mAmNyamBounds, mInputService->getTouchX(),
															    mInputService->getTouchY())) {
				mButton.AmNyam = mButton.STATE_ABLE;
			}
		   // проверяем перемещение пальца по экрана в рамках кнопки музыки
			if(!mOverlapTester->pointInRectangle(&mMusicBounds, mInputService->getTouchX(),
															   mInputService->getTouchY())) {
				//mBtnMusicClamped = false;
				mButton.Music = mButton.STATE_ABLE;
			}
		   // проверяем перемещение пальца по экрана в рамках кнопки звука
			if(!mOverlapTester->pointInRectangle(&mSoundBounds, mInputService->getTouchX(),
															   mInputService->getTouchY())) {
				//mBtnSoundClamped = false;
				mButton.Sound = mButton.STATE_ABLE;
			}
		}



		if(mInputService->getTouchAction() == InputService::TOUCH_UP) {


		// по отпусканию пальца от кнопки Play переходим к игровому экрану
			if(mButton.Play == mButton.STATE_CLAMPED) {
			// если звук включен, воспроизводим клик
				if(mSoundService->soundEnabled()) {
					mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
				}
				mState = STATE_UNINIT;
				mStateTime = 0.0f;
			}

		// по отпусканию пальца от кнопки музыки делаем ее активной/не активной
			if(mButton.Music == mButton.STATE_CLAMPED) {
			// если звук включен, воспроизводим клик
				if(mSoundService->soundEnabled()) {
					mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
				}
				if(mSoundService->musicEnabled()) {
					mSoundService->setMusicState(false);
					mSoundService->stopBGM();

				} else {
					mSoundService->setMusicState(true);
					mSoundService->playBGM("BGM.mp3");
				}
			}

		// по отпусканию пальца от кнопки звука делаем его активным/не активным
			if(mButton.Sound == mButton.STATE_CLAMPED) {
				if(mSoundService->soundEnabled()) {
					mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
					mSoundService->setSoundState(false);
				} else {
					mSoundService->setSoundState(true);
				}
			}

		// по отпусканию пальца от Ам Няма переводим его в состояния прыжка
				if(mButton.AmNyam == mButton.STATE_CLAMPED && mAmNyam.state == mAmNyam.STATE_GAZING) {
				mAmNyam.state = mAmNyam.STATE_INJUMP;
				mAmNyam.jumpDirection = mAmNyam.DIRECTION_UP;
			}

		// по отпусканию пальца от экрана все кнопки делаем активными
			mButton.Play = mButton.STATE_ABLE;
			mButton.About = mButton.STATE_ABLE;
			mButton.Music = mButton.STATE_ABLE;
			mButton.Sound = mButton.STATE_ABLE;
			mButton.AmNyam = mButton.STATE_ABLE;
		}
	}

	void MainMenuScreen::updateExit(float pDeltaTime) {
	// проверяем нажатие на кнопку меню
		if((mInputService->menuBtnPressed() || mInputService->backBtnPressed()) && mStateTime > 0.1f) {
			mState = STATE_MENU;
			mStateTime = 0.0f;
		// если звук включен, воспроизводим клик
			if(mSoundService->soundEnabled()) {
				mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
			}
		}

		if(mInputService->getTouchAction() == InputService::TOUCH_DOWN) {

		// проверям на нажатие кнопки Yes
			if(mOverlapTester->pointInRectangle(&mYesBounds, mInputService->getTouchX(),
														    mInputService->getTouchY())) {
				mButton.Yes = mButton.STATE_CLAMPED;
			}
		// проверям нажатие кнопки No
			if(mOverlapTester->pointInRectangle(&mNoBounds, mInputService->getTouchX(),
														   mInputService->getTouchY())) {
				mButton.No = mButton.STATE_CLAMPED;
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DRAGGED) {

		// проверяем перемещение пальца по экрана в рамках кнопки Yes
			if(!mOverlapTester->pointInRectangle(&mYesBounds, mInputService->getTouchX(),
															 mInputService->getTouchY())) {
				mButton.Yes = mButton.STATE_ABLE;
			}
		// проверяем перемещение пальца по экрана в рамках кнопки No
			if(!mOverlapTester->pointInRectangle(&mNoBounds, mInputService->getTouchX(),
															mInputService->getTouchY())) {

				mButton.No = mButton.STATE_ABLE;
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_UP) {

		// по отпусканию пальца от кнопки No выходим в главное меню
			if(mButton.No == mButton.STATE_CLAMPED) {
				mState = STATE_MENU;
				mStateTime = 0.0f;
			// если звук включен, воспроизводим клик
				if(mSoundService->soundEnabled()) {
					mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
				}
			}

		// по отпусканию пальца от кнопки Yes завершаем работу приложения
			if(mButton.Yes == mButton.STATE_CLAMPED) {
			// если звук включен, воспроизводим клик
				if(mSoundService->soundEnabled()) {
					mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
				}
				mInputService->closeApplication();
			}

		// по отпусканию пальца делаем все кнопки активными
			mButton.Yes = mButton.STATE_ABLE;
			mButton.No = mButton.STATE_ABLE;
		}
	}

	void MainMenuScreen::updateUninit(float pDeltaTime) {

		if(mStateTime > 1/DARK_VELOCITY) {
			((PacmanActivity *)mActivityHandler)->setGameScreen();
		}
	}

	void MainMenuScreen::updateInit(float pDeltaTime) {
		if(mStateTime > 1/DARK_VELOCITY) {
			mState = STATE_MENU;
			mStateTime = 0.0f;
		}
	}
