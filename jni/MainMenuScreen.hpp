#ifndef _MAINMENUSCREEN_HPP_
#define _MAINMENUSCREEN_HPP_

#include "Log.hpp"
#include "Screen.hpp"
#include "Rectangle.hpp"
#include "Vector2D.hpp"
#include "GraphicsService.hpp"
#include "TimeService.hpp"
#include "InputService.hpp"
#include "SoundService.hpp"
#include "PacmanActivity.hpp"
#include "GameScreen.hpp"
#include "Types.hpp"
#include "SpriteBatcher.hpp"
#include "TextureRegion.hpp"
#include "Texture.hpp"
#include "OverlapTester.hpp"
#include "ActivityHandler.hpp"
#include <cmath>

	/*
	 *	  MainMenuScreen - класс, объекты которого отвечают за представление
	 *	 главного меню. Главное меню может быть в различных состояниях ( представлять окошко
	 *	 выхода из игры, завершения т.д) и так же реагировать на взяимодействие
	 *	 с пользователем.
	 */

	class MainMenuScreen : public Screen {
	public:
		~MainMenuScreen();
		void init(ActivityHandler* pPackmanActivity,
				  GraphicsService* pGraphicsService,
				  TimeService* pTimeService,
				  InputService* pInputService,
				  SoundService* pSoundService);

		void present();						 // представление(отрисовка объектов)
		void update(float pDeltaTime);		 // обновление(проверка нажатия на игровые области, кнопки)
		void setConfigs();					 // установка основных конфигураций

	private:
		void presentInit();					 // представление создания
		void presentMenu();					 // представление меню
		void presentExit();					 // представление выхода
		void presentUninit();				 // представление перехода к др. экрану

		void updateInit(float pDeltaTime);	 // обновление создания
		void updateMenu(float pDeltaTime);	 // обновление меню
		void updateExit(float pDeltaTime);	 // обновление выхода
		void updateUninit(float pDeltaTime); // обновления перехода к др. экрану


	private:
		ActivityHandler* mActivityHandler;   // Pacman Activity(нужен при выходе или перехода к др. экрану)

		GraphicsService* mGraphicsService; 	 // работа с графикой
		TimeService*     mTimeService;     	 // работа с синхронизацией
		InputService*	 mInputService;	   	 // работа с внешним взаимодействием
		SoundService*    mSoundService;		 // работа со звуком

		SpriteBatcher* mSpriteBatcher;   	 // им рисуем
		OverlapTester* mOverlapTester;		 // проверка коллизии
		TextureRegion* mTextReg;		   	 // какаую часть тектстуры рисуем

		int32_t mState;						 // состояние экрана
		float mStateTime;					 // время состояния экрана

		// границы игровых объектов
		Rectangle mPlayBounds,
				  mAboutBounds,
				  mMusicBounds,
				  mSoundBounds,
				  mAmNyamBounds,
				  mYesBounds,
				  mNoBounds,
				  mBackgroundBounds,
				  mLogoBounds,
				  mRopeBounds,
				  mCandyBounds,
				  mExitBounds,
				  mWindowBounds,
				  mMindBounds;

		// коэф. масштабирования
		float mk_x,
			  mk_y;

		// разрешение экрана
		int32_t mScreenWidth,
				mScreenHeight;

		struct {  // Ам Ням
			int32_t state;
			int32_t jumpDirection;
			float posY;

			static const int32_t STATE_GAZING = 1;
			static const int32_t STATE_INJUMP = 2;
			static const float   VELOCITY_JUMP = 100.0f;
			static const int32_t DIRECTION_UP = 1;
			static const int32_t DIRECTION_DOWN = 2;
		} mAmNyam;

		struct { // Кнопка
			int32_t Play,
					About,
					Yes,
					No,
					Music,
					Sound,
					AmNyam;

			static const int32_t STATE_ABLE = 0;
			static const int32_t STATE_DISABLE = 1;
			static const int32_t STATE_CLAMPED = 2;
		} mButton;

	public:
		static const int32_t STATE_INIT = 0;
		static const int32_t STATE_MENU = 1;
		static const int32_t STATE_EXIT = 2;
		static const int32_t STATE_UNINIT = 3;
		static const float DARK_VELOCITY = 3.5f;
	};

#endif
