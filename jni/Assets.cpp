#include "Assets.hpp"
#include "SoundService.hpp"

	 Assets::Assets(android_app* pApplication, const char* pPath)
	: mGameObjects(pApplication, pPath)
	{ }

	void Assets::loadTextures(android_app* pApplication)
	{
		mGameObjects.load();


		/*
		 *   Первоночально размер текстуры был 2210x2000, но текстуры не отображались на устройствах с API 4
		 *  Как оказалось, если размер текстуры не будет превышать 2000px, то не будет возникать подобных проблем.
		 *  В связи с чем я лишь сохраняя пропорции уменьшил изображение при этом немного проиграв в качестве
		 *  изображения.
		 */

		// коэфф. масштабирования
		float lk_X = 0.9049;
		float lk_Y = 0.9049;

		// начало отсчета с левого нижнего края!

		mMonsterRed_state1          = new TextureRegion(&mGameObjects, 0*   lk_X, 1007*lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterRed_state2          = new TextureRegion(&mGameObjects, 80*  lk_X, 1007*lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterRed_state3          = new TextureRegion(&mGameObjects, 160* lk_X, 1007*lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterRed_state1_scared   = new TextureRegion(&mGameObjects, 240* lk_X, 1007*lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterRed_state2_scared   = new TextureRegion(&mGameObjects, 320* lk_X, 1007*lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterRed_state3_scared   = new TextureRegion(&mGameObjects, 400* lk_X, 1007*lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterPink_state1		    = new TextureRegion(&mGameObjects, 0*   lk_X, 927* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterPink_state2		    = new TextureRegion(&mGameObjects, 80*  lk_X, 927* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterPink_state3		    = new TextureRegion(&mGameObjects, 160* lk_X, 927* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterPink_state1_scared  = new TextureRegion(&mGameObjects, 240* lk_X, 927* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterPink_state2_scared  = new TextureRegion(&mGameObjects, 320* lk_X, 927* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterPink_state3_scared  = new TextureRegion(&mGameObjects, 400* lk_X, 927* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterCyan_state1		    = new TextureRegion(&mGameObjects, 0*   lk_X, 847* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterCyan_state2		    = new TextureRegion(&mGameObjects, 80*  lk_X, 847* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterCyan_state3		    = new TextureRegion(&mGameObjects, 160* lk_X, 847* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterCyan_state1_scared  = new TextureRegion(&mGameObjects, 240* lk_X, 847* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterCyan_state2_scared  = new TextureRegion(&mGameObjects, 320* lk_X, 847* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterCyan_state3_scared  = new TextureRegion(&mGameObjects, 400* lk_X, 847* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterBrown_state1		= new TextureRegion(&mGameObjects, 0*   lk_X, 767* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterBrown_state2		= new TextureRegion(&mGameObjects, 80*  lk_X, 767* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterBrown_state3		= new TextureRegion(&mGameObjects, 160* lk_X, 767* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterBrown_state1_scared = new TextureRegion(&mGameObjects, 240* lk_X, 767* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterBrown_state2_scared = new TextureRegion(&mGameObjects, 320* lk_X, 767* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterBrown_state3_scared = new TextureRegion(&mGameObjects, 400* lk_X, 767* lk_Y, 77* lk_X, 80* lk_Y);
		mMonsterEyes				= new TextureRegion(&mGameObjects, 596* lk_X, 767* lk_Y, 40* lk_X, 26* lk_Y);
		mAmNyam_state1 				= new TextureRegion(&mGameObjects, 476* lk_X, 1007*lk_Y, 79* lk_X, 80* lk_Y);
		mAmNyam_state2				= new TextureRegion(&mGameObjects, 476* lk_X, 927* lk_Y, 79* lk_X, 80* lk_Y);
		mAmNyam_state3				= new TextureRegion(&mGameObjects, 476* lk_X, 847* lk_Y, 79* lk_X, 80* lk_Y);
		mAmNyam_state4				= new TextureRegion(&mGameObjects, 476* lk_X, 767* lk_Y, 79* lk_X, 80* lk_Y);
		mCandyCircle				= new TextureRegion(&mGameObjects, 556* lk_X, 847* lk_Y, 80* lk_X, 80* lk_Y);
		mCandyHook					= new TextureRegion(&mGameObjects, 555* lk_X, 767* lk_Y, 40* lk_X, 80* lk_Y);
	    mSettingsIcon				= new TextureRegion(&mGameObjects, 559* lk_X, 1007*lk_Y, 80* lk_X, 80* lk_Y);
	    mPauseIcon			    	= new TextureRegion(&mGameObjects, 559* lk_X, 927* lk_Y, 80* lk_X, 79* lk_Y);
		mRope						= new TextureRegion(&mGameObjects, 720* lk_X, 772* lk_Y, 12* lk_X, 315*lk_Y);
		mMusicOn					= new TextureRegion(&mGameObjects, 639* lk_X, 1007*lk_Y, 78* lk_X, 80* lk_Y);
		mMusicOff 					= new TextureRegion(&mGameObjects, 639* lk_X, 927* lk_Y, 78* lk_X, 80* lk_Y);
		mSoundOn					= new TextureRegion(&mGameObjects, 639* lk_X, 847* lk_Y, 78* lk_X, 80* lk_Y);
		mSoundOff					= new TextureRegion(&mGameObjects, 637* lk_X, 767* lk_Y, 78* lk_X, 80* lk_Y);
		mBackground					= new TextureRegion(&mGameObjects, 760* lk_X, 1017*lk_Y, 60* lk_X, 60* lk_Y);
		mConfigMenu					= new TextureRegion(&mGameObjects, 830* lk_X, 1007*lk_Y, 80* lk_X, 80* lk_Y);
		mBlackColor					= new TextureRegion(&mGameObjects, 770* lk_X, 722* lk_Y, 40* lk_X, 40* lk_Y);
		mStar10grad					= new TextureRegion(&mGameObjects, 0*   lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mStar20grad					= new TextureRegion(&mGameObjects, 65*  lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mStarMinus10grad            = new TextureRegion(&mGameObjects, 195* lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mStarMinus20grad			= new TextureRegion(&mGameObjects, 260* lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mStar_state1				= new TextureRegion(&mGameObjects, 325* lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mStar_state2				= new TextureRegion(&mGameObjects, 390* lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mStar_state3				= new TextureRegion(&mGameObjects, 455* lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mStar_state4				= new TextureRegion(&mGameObjects, 130* lk_X, 702* lk_Y, 65* lk_X, 65* lk_Y);
		mMind						= new TextureRegion(&mGameObjects, 830* lk_X, 702* lk_Y ,80* lk_X, 80* lk_Y);
		mLabyrinth 					= new TextureRegion(&mGameObjects, 0*   lk_X, 0*   lk_Y, 910*lk_X, 702*lk_Y);
		mDontEatMe					= new TextureRegion(&mGameObjects, 750* lk_X, 802* lk_Y ,160*lk_X, 100*lk_Y);
		mTimeToKill					= new TextureRegion(&mGameObjects, 750* lk_X, 912* lk_Y ,160*lk_X, 90* lk_Y);
		mLogo						= new TextureRegion(&mGameObjects, 910* lk_X, 947* lk_Y, 850*lk_X, 140*lk_Y);
		mPlay						= new TextureRegion(&mGameObjects, 910* lk_X, 847* lk_Y, 400*lk_X, 100*lk_Y);
		mPlay_clamped				= new TextureRegion(&mGameObjects, 910* lk_X, 747* lk_Y, 400*lk_X, 100*lk_Y);
		mAbout						= new TextureRegion(&mGameObjects, 1310*lk_X, 847* lk_Y, 450*lk_X, 100*lk_Y);
		mAbout_clamped				= new TextureRegion(&mGameObjects, 1310*lk_X, 747* lk_Y, 450*lk_X, 100*lk_Y);
		mExit						= new TextureRegion(&mGameObjects, 910* lk_X, 647* lk_Y, 400*lk_X, 100*lk_Y);
		mYes_clamped				= new TextureRegion(&mGameObjects, 1310*lk_X, 647* lk_Y, 250*lk_X, 100*lk_Y);
		mYes						= new TextureRegion(&mGameObjects, 1310*lk_X, 547* lk_Y, 250*lk_X, 100*lk_Y);
		mNo_clamped					= new TextureRegion(&mGameObjects, 1560*lk_X, 647* lk_Y, 200*lk_X, 100*lk_Y);
		mNo							= new TextureRegion(&mGameObjects, 1560*lk_X, 547* lk_Y, 200*lk_X, 100*lk_Y);
		mSettings					= new TextureRegion(&mGameObjects, 912* lk_X, 447* lk_Y, 598*lk_X, 100*lk_Y);
		mResume						= new TextureRegion(&mGameObjects, 912* lk_X, 347* lk_Y, 450*lk_X, 100*lk_Y);
		mResume_clamped				= new TextureRegion(&mGameObjects, 912* lk_X, 247* lk_Y, 450*lk_X, 100*lk_Y);
		mQuite						= new TextureRegion(&mGameObjects, 1360*lk_X, 347* lk_Y, 400*lk_X, 100*lk_Y);
		mQuite_clamped				= new TextureRegion(&mGameObjects, 1360*lk_X, 247* lk_Y, 400*lk_X, 100*lk_Y);
		mSound						= new TextureRegion(&mGameObjects, 912* lk_X, 147* lk_Y, 450*lk_X, 100*lk_Y);
		mMusic						= new TextureRegion(&mGameObjects, 1360*lk_X, 147* lk_Y, 400*lk_X, 100*lk_Y);
		mOn							= new TextureRegion(&mGameObjects, 912* lk_X, 47*  lk_Y, 148*lk_X, 100*lk_Y);
		mOn_clamped					= new TextureRegion(&mGameObjects, 1312*lk_X, 47*  lk_Y, 148*lk_X, 100*lk_Y);
		mOff						= new TextureRegion(&mGameObjects, 1070*lk_X, 47*  lk_Y, 240*lk_X, 100*lk_Y);
		mOff_clamped				= new TextureRegion(&mGameObjects, 1470*lk_X, 47*  lk_Y, 240*lk_X, 100*lk_Y);
		mReady						= new TextureRegion(&mGameObjects, 1760*lk_X, 987* lk_Y, 450*lk_X, 100*lk_Y);
		mNum0						= new TextureRegion(&mGameObjects, 1820*lk_X, 882* lk_Y, 78* lk_X, 105*lk_Y);
		mNum1						= new TextureRegion(&mGameObjects, 1898*lk_X, 882* lk_Y, 78* lk_X, 105*lk_Y);
		mNum2						= new TextureRegion(&mGameObjects, 1976*lk_X, 882* lk_Y, 78* lk_X, 105*lk_Y);
		mNum3 						= new TextureRegion(&mGameObjects, 2054*lk_X, 882* lk_Y, 78* lk_X, 105*lk_Y);
		mNum4						= new TextureRegion(&mGameObjects, 2132*lk_X, 882* lk_Y, 78* lk_X, 105*lk_Y);
		mNum5						= new TextureRegion(&mGameObjects, 1820*lk_X, 777* lk_Y, 78* lk_X, 105*lk_Y);
		mNum6						= new TextureRegion(&mGameObjects, 1900*lk_X, 777* lk_Y, 78* lk_X, 105*lk_Y);
		mNum7						= new TextureRegion(&mGameObjects, 1976*lk_X, 777* lk_Y, 78* lk_X, 105*lk_Y);
		mNum8						= new TextureRegion(&mGameObjects, 2054*lk_X, 777* lk_Y, 78* lk_X, 105*lk_Y);
		mNum9						= new TextureRegion(&mGameObjects, 2132*lk_X, 777* lk_Y, 78* lk_X, 105*lk_Y);
		mGameOver					= new TextureRegion(&mGameObjects, 1810*lk_X, 577* lk_Y, 400*lk_X, 200*lk_Y);
		mTimeIsUp					= new TextureRegion(&mGameObjects, 1810*lk_X, 357* lk_Y, 400*lk_X, 220*lk_Y);
		mScore						= new TextureRegion(&mGameObjects, 1760*lk_X, 257* lk_Y, 450*lk_X, 100*lk_Y);
		mTime						= new TextureRegion(&mGameObjects, 1810*lk_X, 157* lk_Y, 400*lk_X, 100*lk_Y);
	}


	 	void Assets::loadSounds(SoundService* pSoundService) {
		mSoundBtnClick   = pSoundService->registerSound("BtnClick.wav");
		mSoundPacmanEat  = pSoundService->registerSound("MonsterKill.wav");
		mSoundGong       = pSoundService->registerSound("Gong.wav");
		mSoundSadness    = pSoundService->registerSound("Sadness.wav");
	}




	  void Assets::disposeTextures() {
		  // освобождаем память выделенную на текстуры
			delete mMonsterRed_state1, mMonsterRed_state2, mMonsterRed_state3,
				   mMonsterPink_state1, mMonsterPink_state2, mMonsterPink_state3,
				   mMonsterCyan_state1, mMonsterCyan_state2, mMonsterCyan_state3,
				   mMonsterBrown_state1, mMonsterBrown_state2, mMonsterBrown_state3,
				   mMonsterRed_state1_scared, mMonsterRed_state2_scared, mMonsterRed_state3_scared;
				   mMonsterPink_state1_scared, mMonsterPink_state2_scared, mMonsterPink_state3_scared,
				   mMonsterCyan_state1_scared, mMonsterCyan_state2_scared, mMonsterCyan_state3_scared,
				   mMonsterBrown_state1_scared, mMonsterBrown_state2_scared, mMonsterBrown_state3_scared,
				   mMonsterEyes, mAmNyam_state1, mAmNyam_state2, mAmNyam_state3, mAmNyam_state4,
				   mCandyCircle, mCandyHook, mSettingsIcon, mPauseIcon, mRope, mMusicOn, mMusicOff,
				   mSoundOn, mSoundOff, mBackground, mConfigMenu, mBlackColor, mStar10grad,
				   mStar20grad, mStarMinus10grad, mStarMinus20grad,
				   mStar_state1, mStar_state2, mStar_state3, mStar_state4,
				   mMind, mLabyrinth, mDontEatMe, mTimeToKill, mLogo,
				   mPlay, mPlay_clamped, mAbout, mAbout_clamped, mExit,
				   mYes_clamped, mYes, mNo_clamped, mNo, mSettings,
				   mResume, mResume_clamped, mQuite, mQuite_clamped,
				   mSound, mMusic, mOn, mOn_clamped, mOff, mOff_clamped, mReady,
				   mNum0, mNum1, mNum2, mNum3, mNum4; mNum5, mNum6, mNum7, mNum8, mNum9,
				   mGameOver, mTimeIsUp, mScore, mTime;

	}

	  void Assets::disposeSounds() {
		  // освобождаем память выделенную на звуки
		  /*
		   *   Поскольку указатели на все звуковые объекты будет хранить
		   *  класс SoundService он и будет ответственен за освобождение
		   *  памяти для этих объектов.
		   */
	  }


