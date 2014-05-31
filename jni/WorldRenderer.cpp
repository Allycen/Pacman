#include "WorldRenderer.hpp"

	WorldRenderer::WorldRenderer(float pFrustumWidth, float pFrustumHeight, World* pWorld, GraphicsService* pGraphicsService)
	: mCamera(pFrustumWidth, pFrustumHeight)
	{
		FRUSTUM_WIDTH = pFrustumWidth;
		FRUSTUM_HEIGHT = pFrustumHeight;
		mWorld = pWorld;
		mGraphicsService = pGraphicsService;
		mCamera.setFrustumResolution(FRUSTUM_WIDTH, FRUSTUM_HEIGHT);
	}

	WorldRenderer:: ~WorldRenderer() {
		/*
		 *  Обнуляем указатели. Удаление объектов займется
		 * класс, который их создал.
		 */
		mGraphicsService = NULL;
		mWorld = NULL;
	}

	void WorldRenderer::render() {

		mCamera.setCoords(mWorld->mPacman.mCoords.getX(), mWorld->mPacman.mCoords.getY());
		mCamera.setViewportAndMatrices();

		renderBackground();
		renderLabyrinth();
		renderPacman();
		renderStars();
		renderCandies();
		renderMonsters();
		renderPacmanLives();
		renderTime();
		renderScore();

		if(mWorld->mState == World::STATE_TIMEISUP) {
			renderTimeIsUp();
		}
		if(mWorld->mState == World::STATE_GAMEOVER) {
			renderGameOver();
		}
		if(mWorld->mState == World::STATE_SHOWENDSCORE) {
			renderFinalScore();
		}
	}

	void WorldRenderer::renderPacmanLives() {
		int32_t lLives = mWorld->mPacmanLives;

		while(lLives != 0) {
			mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+1.3f,
								mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-4.5f-1.5*(World::PACMAN_LIVES-lLives),
								1.2f, 1.2f, mGraphicsService->getAssets()->mAmNyam_state1);
			lLives--;
		}
	}

	void WorldRenderer::renderFinalScore() {
		// рисуем Score
			mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
								mWorld->mPacman.mCoords.getY() + 1.0f,
								9.0f, 2.0f, mGraphicsService->getAssets()->mScore);


		// узнаем кол-во символов из которого состоит число
			int32_t lLength = 1;
			while(mWorld->mScore/(pow(10,lLength-1)) >= 10) {
				lLength++;
			}

		// рисуем значение
			char lScore[lLength];
			sprintf(lScore, "%d", mWorld->mScore);
			float lOffset;
			float lOffsetY = 1.0f;
			float lWidth = 1.33f;
			float lHeight = 1.80f;

			lOffset = lLength - 1;

			if(mWorld->mScore > 0) {
			for(int i=0; i<10; i++) {
				switch(lScore[i]) {
					case '0':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum0);
						break;
					case '1':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum1);
						break;
					case '2':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum2);
						break;
					case '3':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum3);
						break;
					case '4':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum4);
						break;
					case '5':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum5);
						break;
					case '6':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum6);
						break;
					case '7':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum7);
						break;
					case '8':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum8);
						break;
					case '9':
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset+i*1.5,
											mWorld->mPacman.mCoords.getY()- lOffsetY,
											lWidth, lHeight, mGraphicsService->getAssets()->mNum9);
						break;
				}
			}
			} else {
				mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-lOffset,
									mWorld->mPacman.mCoords.getY() - lOffsetY,
									lWidth, lHeight, mGraphicsService->getAssets()->mNum0);
			}
	}

	void WorldRenderer::renderTimeIsUp() {
		mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
							mWorld->mPacman.mCoords.getY(),
							8, 4.4f, mGraphicsService->getAssets()->mTimeIsUp);
	}

	void WorldRenderer::renderGameOver() {
		mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
							mWorld->mPacman.mCoords.getY(),
							8, 4.4f, mGraphicsService->getAssets()->mGameOver);
	}

	void WorldRenderer::renderTime() {
	// рисуем "time"
		mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f,
							mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
							4, 1, mGraphicsService->getAssets()->mTime);

	// рисуем значение
		int32_t lTimeLeft = mWorld->mTimeLeft;
		char lTime[3];
		sprintf(lTime, "%d", lTimeLeft);

		float lOffset = 4.2f;
		float lWidth = 0.66f;
		float lHeight = 0.90f;

		if(mWorld->mTimeLeft > 0) {
		for(int i=0; i<2; i++) {
			switch(lTime[i]) {
				case '0':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum0);
					break;
				case '1':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum1);
					break;
				case '2':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum2);
					break;
				case '3':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum3);
					break;
				case '4':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum4);
					break;
				case '5':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum5);
					break;
				case '6':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum6);
					break;
				case '7':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum7);
					break;
				case '8':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum8);
					break;
				case '9':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum9);
					break;
			}
		}
		} else {
			mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+2.2f+lOffset,
								mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-2.3f,
								lWidth, lHeight, mGraphicsService->getAssets()->mNum0);
		}
	}

	void WorldRenderer::renderScore() {
	// рисуем Score
		mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f,
							mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
							4.5, 1, mGraphicsService->getAssets()->mScore);



	// узнаем кол-во символов из которого состоит число
		int32_t lLength = 1;
		while(mWorld->mScore/(pow(10,lLength-1)) >= 10) {
			lLength++;
		}
	// рисуем значение
		char lScore[lLength];
		sprintf(lScore, "%d", mWorld->mScore);
		float lOffset = 3.5f;
		float lWidth = 0.66f;
		float lHeight = 0.90f;


		if(mWorld->mScore > 0) {
		for(int32_t i=0; i<lLength; i++) {
			switch(lScore[i]) {
				case '0':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum0);
					break;
				case '1':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum1);
					break;
				case '2':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum2);
					break;
				case '3':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum3);
					break;
				case '4':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum4);
					break;
				case '5':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum5);
					break;
				case '6':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum6);
					break;
				case '7':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum7);
					break;
				case '8':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum8);
					break;
				case '9':
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset+i,
										mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
										lWidth, lHeight, mGraphicsService->getAssets()->mNum9);
					break;
			}
		}
		} else {
			mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX()-FRUSTUM_WIDTH/2+3.0f+lOffset,
								mWorld->mPacman.mCoords.getY()+FRUSTUM_HEIGHT/2-1.0f,
								lWidth, lHeight, mGraphicsService->getAssets()->mNum0);
		}
	}

	void WorldRenderer::renderBackground() {
		mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
							mWorld->mPacman.mCoords.getY(),
							FRUSTUM_WIDTH, FRUSTUM_HEIGHT, mGraphicsService->getAssets()->mBackground);
	}

	void WorldRenderer::renderLabyrinth() {
		mSpriteBatcher.draw(mWorld->WIDTH/2+0.5f, mWorld->HEIGHT/2+0.5f,
							mWorld->WIDTH,     mWorld->HEIGHT,
							mGraphicsService->getAssets()->mLabyrinth);
	}

	void WorldRenderer::renderMonsters() {
		for(int i=0; i<4;i++) {
		switch(mWorld->mMonsters[i].mAnimFrame) {
		case 0:
			switch(mWorld->mMonsters[i].mType) {
			case Monster::TYPE_RED :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									 mGraphicsService->getAssets()->mMonsterRed_state2);
				break;
			case Monster::TYPE_PINK :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterPink_state2);
				break;
			case Monster::TYPE_CYAN :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterCyan_state2);
				break;
			case Monster::TYPE_BROWN :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterBrown_state2);
				break;
			}
			break;
		case 1:
			switch(mWorld->mMonsters[i].mType) {
			case Monster::TYPE_RED :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									 mGraphicsService->getAssets()->mMonsterRed_state1);
				break;
			case Monster::TYPE_PINK :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterPink_state1);
				break;
			case Monster::TYPE_CYAN :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterCyan_state1);
				break;
			case Monster::TYPE_BROWN :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterBrown_state1);
				break;
			}
			break;
		case 2:
			switch(mWorld->mMonsters[i].mType) {
			case Monster::TYPE_RED :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									 mGraphicsService->getAssets()->mMonsterRed_state3);
				break;
			case Monster::TYPE_PINK :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterPink_state3);
				break;
			case Monster::TYPE_CYAN :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterCyan_state3);
				break;
			case Monster::TYPE_BROWN :
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									mWorld->mMonsters[i].WIDTH,
									mWorld->mMonsters[i].HEIGHT,
									mGraphicsService->getAssets()->mMonsterBrown_state3);
				break;
			}
			break;
		case 3:
			switch(mWorld->mMonsters[i].mType) {
			case Monster::TYPE_RED :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										 mGraphicsService->getAssets()->mMonsterRed_state2_scared);
					break;
			case Monster::TYPE_PINK :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterPink_state2_scared);
					break;
			case Monster::TYPE_CYAN :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterCyan_state2_scared);
					break;
			case Monster::TYPE_BROWN :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterBrown_state2_scared);
					break;
				}
				break;
			case 4:
				switch(mWorld->mMonsters[i].mType) {
				case Monster::TYPE_RED :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										 mGraphicsService->getAssets()->mMonsterRed_state1_scared);
					break;
				case Monster::TYPE_PINK :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterPink_state1_scared);
					break;
				case Monster::TYPE_CYAN :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterCyan_state1_scared);
					break;
				case Monster::TYPE_BROWN :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterBrown_state1_scared);
					break;
				}
				break;
			case 5:
				switch(mWorld->mMonsters[i].mType) {
				case Monster::TYPE_RED :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										 mGraphicsService->getAssets()->mMonsterRed_state3_scared);
					break;
				case Monster::TYPE_PINK :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterPink_state3_scared);
					break;
				case Monster::TYPE_CYAN :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterCyan_state3_scared);
					break;
				case Monster::TYPE_BROWN :
					mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
										mWorld->mMonsters[i].mCoords.getY(),
										mWorld->mMonsters[i].WIDTH,
										mWorld->mMonsters[i].HEIGHT,
										mGraphicsService->getAssets()->mMonsterBrown_state3_scared);
					break;
				}
				break;
			case 6:
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX(),
									mWorld->mMonsters[i].mCoords.getY(),
									0.8f, 0.52,
									mGraphicsService->getAssets()->mMonsterEyes);
				break;
		}
	// если Pacman близко и монстр в страхе - пишем "не ешь меня!"
		if(mWorld->mMonsters[i].mState == Monster::STATE_FRIGHTENED) {
			if(mWorld->mMonsters[i].mPacmanIsNear) {
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX() - 1.4f,
									mWorld->mMonsters[i].mCoords.getY() + 1.4f,
									2.72f, 1.7f,
									mGraphicsService->getAssets()->mDontEatMe);
			}
	// если Pacman монстр скоро выйдет из страха пишем "time to kill!"
			if(mWorld->mMonsters[i].mTimeToKill) {
				mSpriteBatcher.draw(mWorld->mMonsters[i].mCoords.getX() - 1.4f,
									mWorld->mMonsters[i].mCoords.getY() + 1.4f,
									2.72f, 1.7f,
									mGraphicsService->getAssets()->mTimeToKill);
			}
		}
		}
	}

	void WorldRenderer::renderPacman() {

		switch(mWorld->mPacman.mDirection) {
		case Pacman::DIRECTION_UP :
				switch(mWorld->mPacman.mAnimFrame) {
					case 0:
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
											mWorld->mPacman.mCoords.getY(),
											mWorld->mPacman.WIDTH,
											mWorld->mPacman.HEIGHT,
											-90.0f, mGraphicsService->getAssets()->mAmNyam_state1);
						break;
					case 1:
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
											mWorld->mPacman.mCoords.getY(),
											mWorld->mPacman.WIDTH,
											mWorld->mPacman.HEIGHT,
											-90.0f, mGraphicsService->getAssets()->mAmNyam_state2);
						break;
					case 2:
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
											mWorld->mPacman.mCoords.getY(),
											mWorld->mPacman.WIDTH,
											mWorld->mPacman.HEIGHT,
											-90.0f, mGraphicsService->getAssets()->mAmNyam_state3);
						break;
					case 3:
						mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
											mWorld->mPacman.mCoords.getY(),
											mWorld->mPacman.WIDTH,
											mWorld->mPacman.HEIGHT,
											-90.0f, mGraphicsService->getAssets()->mAmNyam_state4);
						break;
				}

				break;
				case Pacman::DIRECTION_DOWN :
				switch(mWorld->mPacman.mAnimFrame) {
				case 0:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										90.0f, mGraphicsService->getAssets()->mAmNyam_state1);
					break;
				case 1:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										90.0f, mGraphicsService->getAssets()->mAmNyam_state2);
					break;
				case 2:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										90.0f, mGraphicsService->getAssets()->mAmNyam_state3);
					break;
				case 3:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										90.0f, mGraphicsService->getAssets()->mAmNyam_state4);
					break;
				}
				break;
				case Pacman::DIRECTION_LEFT :
				switch(mWorld->mPacman.mAnimFrame) {
				case 0:
					mSpriteBatcher.drawInverted(mWorld->mPacman.mCoords.getX(),
												mWorld->mPacman.mCoords.getY(),
												mWorld->mPacman.WIDTH,
												mWorld->mPacman.HEIGHT,
												mGraphicsService->getAssets()->mAmNyam_state1);
					break;
				case 1:
					mSpriteBatcher.drawInverted(mWorld->mPacman.mCoords.getX(),
												mWorld->mPacman.mCoords.getY(),
												mWorld->mPacman.WIDTH,
												mWorld->mPacman.HEIGHT,
												mGraphicsService->getAssets()->mAmNyam_state2);
					break;
				case 2:
					mSpriteBatcher.drawInverted(mWorld->mPacman.mCoords.getX(),
												mWorld->mPacman.mCoords.getY(),
												mWorld->mPacman.WIDTH,
												mWorld->mPacman.HEIGHT,
												mGraphicsService->getAssets()->mAmNyam_state3);
					break;
				case 3:
					mSpriteBatcher.drawInverted(mWorld->mPacman.mCoords.getX(),
												mWorld->mPacman.mCoords.getY(),
												mWorld->mPacman.WIDTH,
												mWorld->mPacman.HEIGHT,
												mGraphicsService->getAssets()->mAmNyam_state4);
					break;
				}
				break;
				case Pacman::DIRECTION_RIGHT :
				switch(mWorld->mPacman.mAnimFrame) {
				case 0:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										mGraphicsService->getAssets()->mAmNyam_state1);
					break;
				case 1:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										mGraphicsService->getAssets()->mAmNyam_state2);
					break;
				case 2:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										mGraphicsService->getAssets()->mAmNyam_state3);
					break;
				case 3:
					mSpriteBatcher.draw(mWorld->mPacman.mCoords.getX(),
										mWorld->mPacman.mCoords.getY(),
										mWorld->mPacman.WIDTH,
										mWorld->mPacman.HEIGHT,
										mGraphicsService->getAssets()->mAmNyam_state4);
					break;
				}

				break;
		}
	}

	void WorldRenderer::renderStars() {


		for(int i=0;i<119;i++) {
			if(mWorld->mStars[i].mState != Star::STATE_NONE) {
		switch(mWorld->mStars[i].mAnimFrame) {
			case 0:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStar_state1);
				break;
			case 1:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStar_state2);
				break;
			case 2:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStar_state3);
				break;
			case 3:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStar_state4);
				break;
			case 4:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStar_state4);
				break;
			case 5:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStar10grad);
				break;
			case 6:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStar20grad);
				break;
			case 7:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStarMinus10grad);
				break;
			case 8:
				mSpriteBatcher.draw(mWorld->mStars[i].mCoords.getX(),
									mWorld->mStars[i].mCoords.getY(),
									mWorld->mStars[i].WIDTH,
									mWorld->mStars[i].HEIGHT,
									mGraphicsService->getAssets()->mStarMinus20grad);
				break;
		}
		}
		}
	}

	void WorldRenderer::renderCandies() {
		for(int i=0; i<2; i++) {
		    if(mWorld->mCandies[i].mState == Candy::STATE_EXIST) {
			mSpriteBatcher.draw(mWorld->mCandies[i].mCoords.getX(),
								mWorld->mCandies[i].mCoords.getY(),
								mWorld->mCandies[i].WIDTH,
								mWorld->mCandies[i].HEIGHT,
								mWorld->mCandies[i].mAngle,
								mGraphicsService->getAssets()->mCandyCircle);
		}
		}

	// рисуем усиливающие карамельки
		for(int i=0; i<4; i++) {
			if(mWorld->mCandiesStrong[i].mState == CandyStrongMaker::STATE_EXIST) {
			mSpriteBatcher.draw(mWorld->mCandiesStrong[i].mCoords.getX()+mWorld->mCandiesStrong[i].mOffset,
								mWorld->mCandiesStrong[i].mCoords.getY(),
								mWorld->mCandiesStrong[i].WIDTH,
								mWorld->mCandiesStrong[i].HEIGHT,
								mGraphicsService->getAssets()->mCandyHook);
		}
		}
	}
