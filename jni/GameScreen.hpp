#ifndef _GAMESCREEN_HPP_
#define _GAMESCREEN_HPP_

#include "Log.hpp"
#include "Screen.hpp"
#include "Rectangle.hpp"
#include "Vector2D.hpp"
#include "GraphicsService.hpp"
#include "TimeService.hpp"
#include "InputService.hpp"
#include "SoundService.hpp"
#include "ActivityHandler.hpp"
#include "PacmanActivity.hpp"
#include "Types.hpp"
#include "SpriteBatcher.hpp"
#include "TextureRegion.hpp"
#include "Texture.hpp"
#include "OverlapTester.hpp"
#include "WorldRenderer.hpp"
#include "World.hpp"
#include <cmath>


	/*
	 *	  GameScreen - класс, объекты которого отвечают за представление
	 *	 игрового экрана. Игровой экран может быть в различных состояниях ( представлять окошко
	 *	 паузы, настроек, отображать игровой процесс и т.д) и так же реагировать на взяимодействие
	 *	 с пользователем.
	 */

	class GameScreen : public Screen {
	public:
		~GameScreen();
		void init(ActivityHandler* pPackmanActivity,
				  GraphicsService* pGraphicsService,
				  TimeService* pTimeService,
				  InputService* pInputService,
				  SoundService* pSoundService);

		void present();								// представление(отрисовка объектов)
		void update(float pDeltaTime);				// обновление(проверка нажатия на игровые области, кнопки)
		void setConfigs();							// установка основных конфигураций

	private:
		void presentInit();							// представление создания
		void presentReady();						// предстовление состояния "Ready?"
		void presentMoveToWorld();					// представление перехода к миру
		void presentRunning();						// представление игрового процесса
		void presentSettings();						// представление меню настроеек
		void presentPause();						// представление меню паузы

		void updateInit(float pDeltaTime);			// обновление создания
		void updateReady(float pDeltaTime);			// обновление состояния "Ready?"
		void updateMoveToWorld(float pDeltaTime);	// обновление перехода к миру
		void updateRunning(float pDeltaTime);		// обновление игрового процесса
		void updateSettings(float pDeltaTime);		// обновление меню настроек
		void updatePause(float pDeltaTime);			// обновление меню паузы

	private:
		ActivityHandler* mActivityHandler;			// Pacman Activity(нужен при переходе к др. экрану)

		GraphicsService* mGraphicsService; 			// работа с графикой
		TimeService*     mTimeService;     			// работа с синхронизацией
		InputService*	 mInputService;	   			// работа с внешним взаимодействием
		SoundService*    mSoundService;				// работа со звуком

		World mWorld;								// игровой мир
		WorldRenderer* mWorldRenderer;				// визуализатор игрового мира

		SpriteBatcher* mSpriteBatcher;   			// им рисуем
		OverlapTester* mOverlapTester;			    // им проверяем соприкосновение
		TextureRegion* mTextReg;		   		    // какаую часть тектстуры рисуем

		int32_t mState,							    // состояние экрана
				mPreviousState;					    // предыдущее состояние экрана

		float mStateTime,							// продолжительность текущего состояния
			  mPreviousStateTime,					// продолжительность прошлого состояния
			  mReadyAngle;							// угол отрисовки "Ready"

		// границы игровых объектов
		Rectangle mResumeBounds,
				  mQuiteBounds,
				  mSettingsBounds,
				  mPauseBounds,
				  mMusicOnOffBounds,
				  mSoundOnOffBounds,
				  mBackgroundBounds,
				  mLabyrinthBounds,
				  mReadyBounds,
				  mWindowBounds,
				  mSettingsWBounds,
				  mMusicWBounds,
				  mSoundWBounds;

		// коэф. масштабирования
		float mk_x,
			  mk_y;

		// разрешение экрана
		int32_t mScreenWidth,
				mScreenHeight;



		struct { // Кнопка
			int32_t Resume,
					Quite,
					Settings,
					Pause,
					MusicOnOff,
					SoundOnOff,
					Start;

			static const int32_t STATE_ABLE = 0;
			static const int32_t STATE_DISABLE = 1;
			static const int32_t STATE_CLAMPED = 2;
		} mButton;

		struct { // камера
			float FrustrumWidth,
				  FrustrumHeight,
				  Zoom_Velocity,
				  CenterX,
				  CenterY,
				  OffsetX,
				  OffsetY;
			bool focused;
		} mCameraInfo;

	public:
		static const int32_t STATE_INIT = 0;
		static const int32_t STATE_READY = 1;
		static const int32_t STATE_MOVETOWORLD = 2;
		static const int32_t STATE_RUNNING = 3;
		static const int32_t STATE_SETTINGS = 4;
		static const int32_t STATE_PAUSE = 5;
		static const float DARK_VELOCITY = 3.5f;
		static const float ANGLE_READY_VELOCITY = 70.0f;
	};



#endif
