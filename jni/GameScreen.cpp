 #include "GameScreen.hpp"

	GameScreen::~GameScreen() {
		// удаляем созданный в динамической памяти объект
		delete mWorldRenderer;
		mWorldRenderer = NULL;
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

	void GameScreen::init(ActivityHandler* pPacmanActivity,
						  GraphicsService* pGraphicsService,
						  TimeService* pTimeService,
						  InputService* pInputService,
						  SoundService* pSoundService)
	{

		mActivityHandler = pPacmanActivity;
		mGraphicsService = pGraphicsService;
		mTimeService = pTimeService;
		mInputService = pInputService;
		mSoundService = pSoundService;

		mSpriteBatcher = ((PacmanActivity *)mActivityHandler)->getSpriteBatcher();
		mOverlapTester = ((PacmanActivity *)mActivityHandler)->getOverlapTester();

		mWorld.setInfo(pInputService, pSoundService);
		mWorldRenderer = new WorldRenderer(23.529f * float(mGraphicsService->getWidth()) / 800.0f,
										   14.117f * float(mGraphicsService->getHeight()) / 480.0f,
										   &mWorld,
										   mGraphicsService);

	}

	void GameScreen::setConfigs() {
		mState = STATE_INIT;
		mStateTime = 0.0f;

		mPreviousState = STATE_INIT;
		mPreviousStateTime = 0.0f;

		mButton.Resume = mButton.STATE_ABLE;
		mButton.Quite = mButton.STATE_ABLE;
		mButton.Settings = mButton.STATE_ABLE;
		mButton.Pause = mButton.STATE_ABLE;
		mButton.MusicOnOff = mButton.STATE_ABLE;
		mButton.SoundOnOff = mButton.STATE_ABLE;
		mButton.Start = mButton.STATE_ABLE;

		mReadyAngle = 0;

		// коэффицунты васштабирования по x и y
		float lk_x = float(mGraphicsService->getWidth()) / 800.0f;
		float lk_y = float(mGraphicsService->getHeight()) / 480.0f;

		mResumeBounds.setInfo	  (400.0f * lk_x, 180.0f * lk_y, 315.0f * lk_x, 70.0f  * lk_x);
		mQuiteBounds.setInfo	  (400.0f * lk_x, 300.0f * lk_y, 280.0f * lk_x, 70.0f  * lk_x);
		mSettingsBounds.setInfo   (665.0f * lk_x, 55.0f  * lk_y, 55.0f  * lk_x, 55.0f  * lk_x);
		mPauseBounds.setInfo	  (745.0f * lk_x, 55.0f  * lk_y, 50.0f  * lk_x, 50.0f  * lk_x);
		mMusicOnOffBounds.setInfo (510.0f * lk_x, 239.0f * lk_y, 117.0f * lk_x, 45.0f  * lk_x);
		mSoundOnOffBounds.setInfo (510.0f * lk_x, 309.0f * lk_y, 117.0f * lk_x, 45.0f  * lk_x);
		mBackgroundBounds.setInfo (400.0f * lk_x, 240.0f * lk_y, 800.0f * lk_x, 480.0f * lk_y);
		mLabyrinthBounds.setInfo  (400.0f * lk_x, 240.0f * lk_y, 595.0f * lk_x, 459.0f * lk_x);
		mReadyBounds.setInfo      (400.0f * lk_x, 240.0f * lk_y, 450.0f * lk_x, 100.0f * lk_x);
		mWindowBounds.setInfo     (400.0f * lk_x, 240.0f * lk_y, 450.0f * lk_x, 270.0f * lk_x);
		mSettingsWBounds.setInfo  (400.0f * lk_x, 160.0f * lk_y, 300.0f * lk_x, 50.0f  * lk_x);
		mMusicWBounds.setInfo     (315.0f * lk_x, 240.0f * lk_y, 180.0f * lk_x, 45.0f  * lk_x);
		mSoundWBounds.setInfo     (315.0f * lk_x, 310.0f * lk_y, 202.5f * lk_x, 45.0f  * lk_x);

		mScreenWidth = 800.0f * lk_x;
		mScreenHeight = 480.0f * lk_y;

		mWorld.refresh();

		mCameraInfo.FrustrumWidth = 400.0f * lk_x;
		mCameraInfo.FrustrumHeight = 240.0f * lk_y;
		mCameraInfo.Zoom_Velocity = 0.4f;
		mCameraInfo.CenterX = 400.0f * lk_x/2;
		mCameraInfo.CenterY = 240.0f * lk_y/2;
		mCameraInfo.OffsetX = -8.5f * lk_x;
		mCameraInfo.OffsetY = 51.0f * lk_y;
		mCameraInfo.focused = false;
	}

	void GameScreen::present() {
		glClear(GL_COLOR_BUFFER_BIT);

		switch(mState) {
			case STATE_INIT:
				presentInit();
				break;
			case STATE_READY:
				presentReady();
				break;
			case STATE_MOVETOWORLD:
				presentMoveToWorld();
				break;
		    case STATE_RUNNING:
			    presentRunning();
			    break;
		    case STATE_PAUSE:
		    	presentPause();
		    	break;
		    case STATE_SETTINGS:
		    	presentSettings();
		    	break;
		}
	}

	void GameScreen::presentInit() {
		this->presentReady();

	// рисуем затемнение
		mSpriteBatcher->draw(mGraphicsService->getWidth()/2,
							 mGraphicsService->getHeight()/2,
							 mGraphicsService->getWidth() - DARK_VELOCITY * mStateTime * mGraphicsService->getWidth(),
							 mGraphicsService->getHeight() - DARK_VELOCITY * mStateTime * mGraphicsService->getHeight(),
							 mGraphicsService->getAssets()->mBlackColor);
	}

	void GameScreen::presentReady() {



		// рисуем задний фон
			mSpriteBatcher->draw(mBackgroundBounds.getX(),
								 mBackgroundBounds.getY(),
								 mBackgroundBounds.getWidth(),
								 mBackgroundBounds.getHeight(),
								 mGraphicsService->getAssets()->mBackground);
		// рисуем лабиринт
			mSpriteBatcher->draw(mLabyrinthBounds.getX(),
								 mLabyrinthBounds.getY(),
								 mLabyrinthBounds.getWidth(),
								 mLabyrinthBounds.getHeight(),
								 mGraphicsService->getAssets()->mLabyrinth);

		// рисуем кнопку настроеек
			mSpriteBatcher->draw(mSettingsBounds.getX(),
								 mScreenHeight - mSettingsBounds.getY(),
								 mSettingsBounds.getWidth(),
								 mSettingsBounds.getHeight(),
								 mGraphicsService->getAssets()->mSettingsIcon);

		// рисуем кнопку паузы
			mSpriteBatcher->draw(mPauseBounds.getX(),
								 mScreenHeight - mPauseBounds.getY(),
								 mPauseBounds.getWidth(),
								 mPauseBounds.getHeight(),
								 mGraphicsService->getAssets()->mPauseIcon);

		// рисуем ready
			if(mReadyAngle < 180) {
				mSpriteBatcher->draw(mReadyBounds.getX(),
									 mReadyBounds.getY(),
									 mReadyBounds.getWidth() / (mReadyAngle/50),
									 mReadyBounds.getHeight() / (mReadyAngle/50),
									 mReadyAngle,
									 mGraphicsService->getAssets()->mReady);
			} else {
				mSpriteBatcher->draw(mReadyBounds.getX(),
									 mReadyBounds.getY(),
									 mReadyBounds.getWidth() / ((360-mReadyAngle)/50),
									 mReadyBounds.getHeight() / ((360-mReadyAngle)/50), mReadyAngle,
									 mGraphicsService->getAssets()->mReady);
			}

	}

	void GameScreen::presentMoveToWorld() {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if(mCameraInfo.FrustrumWidth < mScreenWidth - mScreenWidth*mStateTime*mCameraInfo.Zoom_Velocity) {
			mCameraInfo.CenterX = (mScreenWidth - mScreenWidth*mStateTime*mCameraInfo.Zoom_Velocity) / 2;
			mCameraInfo.CenterY = (mScreenHeight - mScreenHeight*mStateTime*mCameraInfo.Zoom_Velocity) / 2;
			glOrthof(0, mCameraInfo.CenterX*2, 0, mCameraInfo.CenterY*2, 1, 0);
		} else {
			glOrthof(0, mCameraInfo.CenterX*2, 0, mCameraInfo.CenterY*2, 1, 0);
			if(mStateTime*3>1) mCameraInfo.focused = true;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	// рисуем лабиринт
		if(mStateTime*3<1) {
			mSpriteBatcher->draw(mCameraInfo.CenterX+mCameraInfo.OffsetX*mStateTime*3,
								 mCameraInfo.CenterY+mCameraInfo.OffsetY*mStateTime*3,
								 mLabyrinthBounds.getWidth(),
								 mLabyrinthBounds.getHeight(),
								 mGraphicsService->getAssets()->mLabyrinth);
		} else {
			mSpriteBatcher->draw(mCameraInfo.CenterX+mCameraInfo.OffsetX,
								 mCameraInfo.CenterY+mCameraInfo.OffsetY,
								 mLabyrinthBounds.getWidth(),
								 mLabyrinthBounds.getHeight(),
								 mGraphicsService->getAssets()->mLabyrinth);
		}
	}

	void GameScreen::presentRunning() {
		// рисуем мир
		mWorldRenderer->render();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(0, mScreenWidth, 0, mScreenHeight, 1, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// рисуем кнопку настроеек
			mSpriteBatcher->draw(mSettingsBounds.getX(),
								 mScreenHeight - mSettingsBounds.getY(),
								 mSettingsBounds.getWidth(),
								 mSettingsBounds.getHeight(),
								 mGraphicsService->getAssets()->mSettingsIcon);
		// рисуем кнопку паузы
			mSpriteBatcher->draw(mPauseBounds.getX(),
								 mScreenHeight - mPauseBounds.getY(),
								 mPauseBounds.getWidth(),
								 mPauseBounds.getHeight(),
								 mGraphicsService->getAssets()->mPauseIcon);
	}

	void GameScreen::presentPause() {

	// в зависимости от того из какого состояния вызывалась пауза
	// отображаем соответствующий задний фон
		switch(mPreviousState) {
			case STATE_READY:
				presentReady();
				break;
			case STATE_RUNNING:
				presentRunning();
				break;
		}


		// рисуем окошко выхода
			mSpriteBatcher->draw(mWindowBounds.getX(),
								 mScreenHeight - mWindowBounds.getY(),
								 mWindowBounds.getWidth(),
								 mWindowBounds.getHeight(),
								 mGraphicsService->getAssets()->mConfigMenu);

		// рисуем кнопку Resume
			if(mButton.Resume == mButton.STATE_ABLE) {
				mSpriteBatcher->draw(mResumeBounds.getX(),
									 mScreenHeight - mResumeBounds.getY(),
									 mResumeBounds.getWidth(),
									 mResumeBounds.getHeight(),
									 mGraphicsService->getAssets()->mResume);
			} else {
				mSpriteBatcher->draw(mResumeBounds.getX(),
									 mScreenHeight - mResumeBounds.getY(),
									 mResumeBounds.getWidth(),
									 mResumeBounds.getHeight(),
									 mGraphicsService->getAssets()->mResume_clamped);
			}

		// рисуем кнопку Quite
			if(mButton.Quite == mButton.STATE_ABLE) {
				mSpriteBatcher->draw(mQuiteBounds.getX(),
									 mScreenHeight - mQuiteBounds.getY(),
									 mQuiteBounds.getWidth(),
									 mQuiteBounds.getHeight(),
									 mGraphicsService->getAssets()->mQuite);
			} else {
				mSpriteBatcher->draw(mQuiteBounds.getX(),
									 mScreenHeight - mQuiteBounds.getY(),
									 mQuiteBounds.getWidth(),
									 mQuiteBounds.getHeight(),
									 mGraphicsService->getAssets()->mQuite_clamped);
			}

	}

	void GameScreen::presentSettings() {

	// в зависимости от того из какого состояния вызывались настройки
	// отображаем соответствующий задний фон
		switch(mPreviousState) {
			case STATE_READY:
				presentReady();
				break;
			case STATE_RUNNING:
				presentRunning();
				break;
		}


		// рисуем окошко настроек
			mSpriteBatcher->draw(mWindowBounds.getX(),
								 mScreenHeight - mWindowBounds.getY(),
								 mWindowBounds.getWidth(),
								 mWindowBounds.getHeight(),
								 mGraphicsService->getAssets()->mConfigMenu);
		// рисуем settings
			mSpriteBatcher->draw(mSettingsWBounds.getX(),
								 mScreenHeight - mSettingsWBounds.getY(),
								 mSettingsWBounds.getWidth(),
								 mSettingsWBounds.getHeight(),
								 mGraphicsService->getAssets()->mSettings);
		// рисуем music
			mSpriteBatcher->draw(mMusicWBounds.getX(),
								 mScreenHeight - mMusicWBounds.getY(),
								 mMusicWBounds.getWidth(),
								 mMusicWBounds.getHeight(),
								 mGraphicsService->getAssets()->mMusic);
		// рисуем sound
			mSpriteBatcher->draw(mSoundWBounds.getX(),
								 mScreenHeight - mSoundWBounds.getY(),
								 mSoundWBounds.getWidth(),
								 mSoundWBounds.getHeight(),
								 mGraphicsService->getAssets()->mSound);


		// рисуем состояние music
			if(mSoundService->musicEnabled()) {
				mSpriteBatcher->draw(mMusicOnOffBounds.getX(),
									 mScreenHeight - mMusicOnOffBounds.getY(),
									 mMusicOnOffBounds.getWidth(),
									 mMusicOnOffBounds.getHeight(),
									 mGraphicsService->getAssets()->mOn);
			} else {
				mSpriteBatcher->draw(mMusicOnOffBounds.getX(),
									 mScreenHeight - mMusicOnOffBounds.getY(),
									 mMusicOnOffBounds.getWidth(),
									 mMusicOnOffBounds.getHeight(),
									 mGraphicsService->getAssets()->mOff);
			}

		// рисуем состояние sound
			if(mSoundService->soundEnabled()) {
				mSpriteBatcher->draw(mSoundOnOffBounds.getX(),
									 mScreenHeight - mSoundOnOffBounds.getY(),
									 mSoundOnOffBounds.getWidth(),
									 mSoundOnOffBounds.getHeight(),
									 mGraphicsService->getAssets()->mOn);
			} else {
				mSpriteBatcher->draw(mSoundOnOffBounds.getX(),
									 mScreenHeight - mSoundOnOffBounds.getY(),
									 mSoundOnOffBounds.getWidth(),
									 mSoundOnOffBounds.getHeight(),
									 mGraphicsService->getAssets()->mOff);
			}
	}

	void GameScreen::update(float pDeltaTime) {


		switch(mState) {
				case STATE_INIT:
					updateInit(pDeltaTime);
					break;
				case STATE_READY:
					updateReady(pDeltaTime);
					break;
				case STATE_MOVETOWORLD:
					updateMoveToWorld(pDeltaTime);
					break;
				case STATE_RUNNING:
					updateRunning(pDeltaTime);
					break;
				case STATE_PAUSE:
					updatePause(pDeltaTime);
					break;
				case STATE_SETTINGS:
					updateSettings(pDeltaTime);
					break;
			}

			this->present();
			mStateTime += pDeltaTime;
	}

	void GameScreen::updateInit(float pDeltaTime) {

		if(mStateTime > 1/DARK_VELOCITY) {

			mState = STATE_READY;
			mPreviousState = STATE_INIT;
			mPreviousStateTime = mStateTime;
			mStateTime = 0.0f;
		}
	}

	void GameScreen::updateReady(float pDeltaTime) {

		mReadyAngle += ANGLE_READY_VELOCITY * pDeltaTime;
		if(mReadyAngle > 360) {
			mReadyAngle = 0;
		}
	// проверяем нажатие на кнопку меню
		if((mInputService->menuBtnPressed() || mInputService->backBtnPressed()) && mStateTime > 0.1f) {
			mState = STATE_PAUSE;
			mPreviousState = STATE_READY;
			mPreviousStateTime = mStateTime;
			mStateTime = 0.0f;
		// если звук включен, воспроизводим клик
			if(mSoundService->soundEnabled()) {
				mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
			}
		}

		if(mInputService->getTouchAction() == InputService::TOUCH_DOWN) {
			// проверям на нажатие кнопки Pause
				if(mOverlapTester->pointInRectangle(&mPauseBounds, mInputService->getTouchX(),
																  mInputService->getTouchY())) {
					mButton.Pause = mButton.STATE_CLAMPED;
				}
			// проверям нажатие кнопки Settings
				if(mOverlapTester->pointInRectangle(&mSettingsBounds, mInputService->getTouchX(),
																     mInputService->getTouchY())) {
					mButton.Settings = mButton.STATE_CLAMPED;
				}
			// проверяем нажатие на любую область кроме Settings и Pause
				if(!mOverlapTester->pointInRectangle(&mPauseBounds, mInputService->getTouchX(),
													 mInputService->getTouchY()) &&
				   !mOverlapTester->pointInRectangle(&mSettingsBounds, mInputService->getTouchX(),
													 mInputService->getTouchY())) {
					mButton.Start = mButton.STATE_CLAMPED;
				}
			}


			if(mInputService->getTouchAction() == InputService::TOUCH_DRAGGED) {
			// проверяем перемещение пальца по экрана в рамках кнопки Pause
				if(!mOverlapTester->pointInRectangle(&mPauseBounds, mInputService->getTouchX(),
																   mInputService->getTouchY())) {
					mButton.Pause = mButton.STATE_ABLE;
				}
			// проверяем перемещение пальца по экрана в рамках кнопки Settings
				if(!mOverlapTester->pointInRectangle(&mSettingsBounds, mInputService->getTouchX(),
																      mInputService->getTouchY())) {
					mButton.Settings = mButton.STATE_ABLE;
				}
			// проверяем перемещение пальца по экрану в рамках любой области кроме Pause и Settings
				if(mOverlapTester->pointInRectangle(&mPauseBounds, mInputService->getTouchX(),
																  mInputService->getTouchY()) ||
				   mOverlapTester->pointInRectangle(&mSettingsBounds, mInputService->getTouchX(),
																     mInputService->getTouchY())) {
					mButton.Start = mButton.STATE_ABLE;
				}
			}

				if(mInputService->getTouchAction() == InputService::TOUCH_UP) {

				// по отпусканию пальца от кнопки Pause переходим к паузе
					if(mButton.Pause == mButton.STATE_CLAMPED) {
						mState = STATE_PAUSE;
						mPreviousState = STATE_READY;
						mPreviousStateTime = mStateTime;
						mStateTime = 0.0f;
					// если звук включен, воспроизводим клик
						if(mSoundService->soundEnabled()) {
							mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
						}
					}

				// по отпусканию пальца от кнопки Settings переходим к настройкам
					if(mButton.Settings == mButton.STATE_CLAMPED) {
						mState = STATE_SETTINGS;
						mPreviousState = STATE_READY;
						mPreviousStateTime = mStateTime;
						mStateTime = 0.0f;
					// если звук включен, воспроизводим клик
						if(mSoundService->soundEnabled()) {
							mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
						}
					}

				// по отпусканию пальца от игровой области переходим к игре
					if(mButton.Start == mButton.STATE_CLAMPED) {
						mState = STATE_MOVETOWORLD;
						mPreviousState = STATE_READY;
						mPreviousStateTime = mStateTime;
						mStateTime = 0.0f;
					// если звук включен, воспроизводим клик
						if(mSoundService->soundEnabled()) {
							mSoundService->playSound(mSoundService->getAssets()->mSoundGong);
						}
					}

				// по отпусканию пальца делаем все кнопки активными
					mButton.Pause = mButton.STATE_ABLE;
					mButton.Settings = mButton.STATE_ABLE;
					mButton.Start = mButton.STATE_ABLE;
				}
	}

	void GameScreen::updateMoveToWorld(float pDeltaTime) {
	// если камера сфокусировалась на нужном месте переходим в режим игры
		if(mCameraInfo.focused) {
			mState = STATE_RUNNING;
			mPreviousState = STATE_MOVETOWORLD;
			mPreviousStateTime = mStateTime;
			mStateTime = 0.0f;
		}
	}

	void GameScreen::updateRunning(float pDeltaTime) {
	// проверяем нажатие на кнопку меню
		if((mInputService->menuBtnPressed() || mInputService->backBtnPressed()) && mStateTime > 0.1f) {
			mState = STATE_PAUSE;
			mPreviousState = STATE_RUNNING;
			mPreviousStateTime = mStateTime;
			mStateTime = 0.0f;
		// если звук включен, воспроизводим клик
			if(mSoundService->soundEnabled()) {
				mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
			}
		}

	// обновляем игровой мир
		//mWorld->update(pDeltaTime);
		mWorld.update(pDeltaTime);

	// проверяем на конец игры
		if(mWorld.mState == World::STATE_EXIT) {
			((PacmanActivity *)mActivityHandler)->setMainMenuScreen();
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DOWN) {
			// проверям на нажатие кнопки Pause
				if(mOverlapTester->pointInRectangle(&mPauseBounds, mInputService->getTouchX(),
																  mInputService->getTouchY())) {
					mButton.Pause = mButton.STATE_CLAMPED;
				}
			// проверям нажатие кнопки Settings
				if(mOverlapTester->pointInRectangle(&mSettingsBounds, mInputService->getTouchX(),
																     mInputService->getTouchY())) {
					mButton.Settings = mButton.STATE_CLAMPED;
				}
			}


			if(mInputService->getTouchAction() == InputService::TOUCH_DRAGGED) {
			// проверяем перемещение пальца по экрана в рамках кнопки Pause
				if(!mOverlapTester->pointInRectangle(&mPauseBounds, mInputService->getTouchX(),
																   mInputService->getTouchY())) {
					mButton.Pause = mButton.STATE_ABLE;
				}
			// проверяем перемещение пальца по экрана в рамках кнопки Settings
				if(!mOverlapTester->pointInRectangle(&mSettingsBounds, mInputService->getTouchX(),
																      mInputService->getTouchY())) {
					mButton.Settings = mButton.STATE_ABLE;
				}
			}

				if(mInputService->getTouchAction() == InputService::TOUCH_UP) {

				// по отпусканию пальца от кнопки Pause переходим к паузе
					if(mButton.Pause == mButton.STATE_CLAMPED) {
						mState = STATE_PAUSE;
						mPreviousState = STATE_RUNNING;
						mPreviousStateTime = mStateTime;
						mStateTime = 0.0f;
					// если звук включен, воспроизводим клик
						if(mSoundService->soundEnabled()) {
							mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
						}
					}

				// по отпусканию пальца от кнопки Settings переходим к настройкам
					if(mButton.Settings == mButton.STATE_CLAMPED) {
						mState = STATE_SETTINGS;
						mPreviousState = STATE_RUNNING;
						mPreviousStateTime = mStateTime;
						mStateTime = 0.0f;
					// если звук включен, воспроизводим клик
						if(mSoundService->soundEnabled()) {
							mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
						}
					}

				// по отпусканию пальца делаем все кнопки активными
					mButton.Pause = mButton.STATE_ABLE;
					mButton.Settings = mButton.STATE_ABLE;
					mButton.Start = mButton.STATE_ABLE;
				}
	}

	void GameScreen::updatePause(float pDeltaTime) {
	// проверяем нажатие на кнопку меню
		if((mInputService->menuBtnPressed() || mInputService->backBtnPressed()) && mStateTime > 0.1f) {
			mState = mPreviousState;
			mPreviousState = STATE_PAUSE;
			float pStateTime = mStateTime;
			mStateTime = mPreviousStateTime;
			mPreviousStateTime = pStateTime;
		// если звук включен, воспроизводим клик
			if(mSoundService->soundEnabled()) {
				mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DOWN) {
		// проверям на нажатие кнопки Resume
			if(mOverlapTester->pointInRectangle(&mResumeBounds, mInputService->getTouchX(),
															   mInputService->getTouchY())) {
				mButton.Resume = mButton.STATE_CLAMPED;
			}
		// проверям нажатие кнопки No
			if(mOverlapTester->pointInRectangle(&mQuiteBounds, mInputService->getTouchX(),
															  mInputService->getTouchY())) {
				mButton.Quite = mButton.STATE_CLAMPED;
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DRAGGED) {
		// проверяем перемещение пальца по экрана в рамках кнопки Resume
			if(!mOverlapTester->pointInRectangle(&mResumeBounds, mInputService->getTouchX(),
															    mInputService->getTouchY())) {
				mButton.Resume = mButton.STATE_ABLE;
			}
		// проверяем перемещение пальца по экрана в рамках кнопки Quite
			if(!mOverlapTester->pointInRectangle(&mQuiteBounds, mInputService->getTouchX(),
															   mInputService->getTouchY())) {
				mButton.Quite = mButton.STATE_ABLE;
			}
		}


			if(mInputService->getTouchAction() == InputService::TOUCH_UP) {

			// по отпусканию пальца от кнопки Resume возвращаемся к игре
				if(mButton.Resume == mButton.STATE_CLAMPED) {
					mState = mPreviousState;
					mPreviousState = STATE_PAUSE;
					float pStateTime = mStateTime;
					mStateTime = mPreviousStateTime;
					mPreviousStateTime = pStateTime;
				// если звук включен, воспроизводим клик
					if(mSoundService->soundEnabled()) {
						mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
					}
				}

			// по отпусканию пальца от кнопки Quite возвращаемся в главное меню
				if(mButton.Quite == mButton.STATE_CLAMPED) {
					((PacmanActivity *)mActivityHandler)->setMainMenuScreen();
					// если звук включен, воспроизводим клик
					if(mSoundService->soundEnabled()) {
						mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
					}
				}

			// по отпусканию пальца делаем все кнопки активными
				mButton.Resume = mButton.STATE_ABLE;
				mButton.Quite = mButton.STATE_ABLE;
			}
	}

	void GameScreen::updateSettings(float pDeltaTime) {
	// проверяем нажатие на кнопку меню
		if((mInputService->menuBtnPressed() || mInputService->backBtnPressed()) && mStateTime > 0.1f) {
			mState = mPreviousState;
			mPreviousState = STATE_SETTINGS;
			float pStateTime = mStateTime;
			mStateTime = mPreviousStateTime;
			mPreviousStateTime = pStateTime;
		// если звук включен, воспроизводим клик
			if(mSoundService->soundEnabled()) {
				mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DOWN) {
		// проверям нажатие кнопки Music On/Off
			if(mOverlapTester->pointInRectangle(&mMusicOnOffBounds, mInputService->getTouchX(),
															       mInputService->getTouchY())) {
				mButton.MusicOnOff = mButton.STATE_CLAMPED;
			}
		// проверяем нажатие кнопки Sound On/Off
			if(mOverlapTester->pointInRectangle(&mSoundOnOffBounds, mInputService->getTouchX(),
																   mInputService->getTouchY())) {
				mButton.SoundOnOff = mButton.STATE_CLAMPED;
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_DRAGGED) {
		// проверяем перемещение пальца по экрану в рамках кнопки Music On/Off
			if(!mOverlapTester->pointInRectangle(&mMusicOnOffBounds, mInputService->getTouchX(),
															        mInputService->getTouchY())) {
				mButton.MusicOnOff = mButton.STATE_ABLE;
			}
		// проверяем перемещение пальца по экрану в рамках кнопки Sound On/Off
			if(mOverlapTester->pointInRectangle(&mSoundOnOffBounds, mInputService->getTouchX(),
																   mInputService->getTouchY())) {
				mButton.SoundOnOff = mButton.STATE_ABLE;
			}
		}


		if(mInputService->getTouchAction() == InputService::TOUCH_UP) {

		// по отпусканию пальца от кнопки Music On/Off меняем состояния музыки
			if(mButton.MusicOnOff == mButton.STATE_CLAMPED) {
				if(mSoundService->soundEnabled()){
					mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
				}
				if(mSoundService->musicEnabled()){
					mSoundService->setMusicState(false);
					mSoundService->stopBGM();
				} else {
					mSoundService->setMusicState(true);
					mSoundService->playBGM("BGM.mp3");
				}
			}

		// по отпусканию пальца от кнопки Sound On/Off меняем состояния музыки
			if(mButton.SoundOnOff == mButton.STATE_CLAMPED) {
				if(mSoundService->soundEnabled()){
					mSoundService->playSound(mSoundService->getAssets()->mSoundBtnClick);
					mSoundService->setSoundState(false);
				} else {
					mSoundService->setSoundState(true);
				}
			}


		// по отпусканию пальца делаем все кнопки активными
			mButton.MusicOnOff = mButton.STATE_ABLE;
			mButton.SoundOnOff = mButton.STATE_ABLE;
		}
	}



