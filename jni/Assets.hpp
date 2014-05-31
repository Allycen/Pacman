#ifndef _ASSETS_HPP_
#define _ASSETS_HPP_

#include "Texture.hpp"
#include "TextureRegion.hpp"
#include "Sound.hpp"

#include <android_native_app_glue.h>

	/*
	 *    Assets - класс, объекты которого, отвечают за определение игровых текстур
	 *   и звуков, необходимых в игре.
	 */

	class Assets {
	public:
		Assets(android_app* pApplication, const char* pPath);

		void loadTextures(android_app* pApplication);
		void loadSounds(SoundService* pSoundService);
		void disposeTextures();
		void disposeSounds();

	public:
			// текстура, содержащая изображения всех игровых объектов
		Texture mGameObjects;

			// игровые объекты
		TextureRegion *mMonsterRed_state1,			// красный монстр
					  *mMonsterRed_state2,
					  *mMonsterRed_state3,
					  *mMonsterRed_state1_scared,
					  *mMonsterRed_state2_scared,
					  *mMonsterRed_state3_scared, // +

					  *mMonsterPink_state1,			// розовый монстр
					  *mMonsterPink_state2,
					  *mMonsterPink_state3,
					  *mMonsterPink_state1_scared,
					  *mMonsterPink_state2_scared,
					  *mMonsterPink_state3_scared, // +

					  *mMonsterCyan_state1,			// голубой монстр
					  *mMonsterCyan_state2,
					  *mMonsterCyan_state3,
					  *mMonsterCyan_state1_scared,
					  *mMonsterCyan_state2_scared,
					  *mMonsterCyan_state3_scared, // +

					  *mMonsterBrown_state1,			// коричневый монстр
					  *mMonsterBrown_state2,
					  *mMonsterBrown_state3,
					  *mMonsterBrown_state1_scared,
					  *mMonsterBrown_state2_scared,
					  *mMonsterBrown_state3_scared, // +

					  *mMonsterEyes,					// глаза монстра

					  *mAmNyam_state1,				// ам-ням
					  *mAmNyam_state2,
					  *mAmNyam_state3,
					  *mAmNyam_state4, // +

					  *mCandyCircle,				// карамелька
					  *mCandyHook,					// +

					  *mPauseIcon,					// иконка паузы +
					  *mSettingsIcon,				// иконка настроек +

					  *mRope,						// веревка

					  *mSoundOn,					// иконка включенного звука
					  *mSoundOff,					// иконка выключенного звука
					  *mMusicOn,					// иконка включенной музыки
					  *mMusicOff,					// иконка выключенной музыки

					  *mBackground,					// задний фон
					  *mConfigMenu,					// окошко уведомлений(при выходе и т.п)
					  *mBlackColor,

					  *mStar10grad,
					  *mStar20grad,
					  *mStarMinus10grad,
					  *mStarMinus20grad,
					  *mStar_state1,
					  *mStar_state2,
					  *mStar_state3,
					  *mStar_state4,

					  *mMind,

					  *mLabyrinth,

					  *mDontEatMe,
					  *mTimeToKill;

			// слова, числа и т.п
		TextureRegion *mLogo,

					  *mPlay,
					  *mPlay_clamped,
					  *mAbout,
					  *mAbout_clamped,

					  *mExit,

					  *mYes,
					  *mYes_clamped,
					  *mNo,
					  *mNo_clamped,

					  *mSettings,

					  *mResume,
					  *mResume_clamped,
					  *mQuite,
					  *mQuite_clamped,

					  *mSound,
					  *mMusic,

					  *mOn,
					  *mOn_clamped,
					  *mOff,
					  *mOff_clamped,

					  *mReady,

					  *mNum0,
					  *mNum1,
					  *mNum2,
					  *mNum3,
					  *mNum4,
					  *mNum5,
					  *mNum6,
					  *mNum7,
					  *mNum8,
					  *mNum9,

					  *mGameOver,
					  *mTimeIsUp,
					  *mScore,
					  *mTime;

		Sound		  *mSoundBtnClick,
					  *mSoundPacmanEat,
					  *mSoundGong,
					  *mSoundSadness,
					  *mSoundGameOver,
					  *mSoundGainStar,
					  *mSoundGainCandy;

	};

#endif

