#include "World.hpp"

	World::World()
	: mPacman(18.5f, 11.0f, Pacman::STATE_RUN, Pacman::DIRECTION_RIGHT)
	{}

	World::~World() {
		/*
		 *  Обнуляем указатели. Удаление объектов займется
		 * класс, который их создал.
		 */
	     mInputService = NULL;
	     mSoundService = NULL;
	}

	void World::setInfo(InputService* pInputService, SoundService* pSoundService) {
		mInputService = pInputService;
		mSoundService = pSoundService;
		mTimeLeft = TIME_LEVEL;
		mScore = 0;
		mState = STATE_RUNNING;
		mPacmanLives = PACMAN_LIVES;
		/*
		 *  По-хорошему информацию о лабиринте и всех его составляющих надо получать
		 *   из файла или базы данных. В силу небольшого объема информации заполнил всю нужную
		 *   информацию о лабиринте вручную
		 */
		fillCells();			// информация о ячейках лабиринта
		fillStarsInfo();		// информация о звездочках в лабиринте
		fillCandiesInfo();		// информация о карамельках в лабиринте
		fillMonstersInfo();		// информация о монстрах
	}

	void World::refresh() {
		mTimeLeft = TIME_LEVEL;
		mScore = 0;
		mState = STATE_RUNNING;
		mPacmanLives = PACMAN_LIVES;

		fillCells();			// информация о ячейках лабиринта
		fillStarsInfo();		// информация о звездочках в лабиринте
		fillCandiesInfo();		// информация о карамельках в лабиринте
		fillMonstersInfo();		// информация о монстрах

		mPacman.mCoords.set(18.5f, 11.0f);
		mPacman.mState = Pacman::STATE_RUN;
		mPacman.mStateTime = 0.0f;
		mPacman.mDirection = Pacman::DIRECTION_RIGHT;
	}

	void World::update(float pDeltaTime) {
		if(mState == STATE_RUNNING) {
			checkCollisions();
			checkTimeIsUp();
			checkGameOver();

			updatePacman(pDeltaTime);
			updateStars(pDeltaTime);
			updateCandies(pDeltaTime);
			updateMonsters(pDeltaTime);

			mTimeLeft -= pDeltaTime;
		}

		if(mState == STATE_TIMEISUP || mState == STATE_GAMEOVER || mState == STATE_SHOWENDSCORE) {
			checkTouchEvent();
		}

		if(mState == STATE_WAITING && mStateTime > TIME_WAIT) {
			mState = STATE_RUNNING;
			mStateTime = 0.0f;
		}
		mStateTime += pDeltaTime;
	}


	void World::updateCandies(float pDeltaTime) {
		for(int i=0; i<2; i++) {
			mCandies[i].update(pDeltaTime);
		}
		for(int i=0;i<4;i++) {
			mCandiesStrong[i].update(pDeltaTime);
		}
	}

	void World::updateStars(float pDeltaTime) {
		for(int i=0;i<119;i++) {
			mStars[i].update(pDeltaTime);
		}
	}

	void World::updateMonsters(float pDeltaTime) {

		// время, через которое монстры выходят из домика
		float lTimeInHome[] = { 3.0f, 5.0f, 7.0f, 9.0f };

		for(int i=0; i<4; i++) {

			/*
			 *   Алгоритм поведения монстра когда он в домике(PLACE_ATHOME)
			 *
			 *    1) Если он в (STATE_SCATTER или STATE_CHASE)
			 *  1. Бегаем в пределах домика вверх вниз с нормальной скоростью.
			 *  2. По истечению определенного времени начинаем выходить из домика.
			 *
			 *    2) Если он в (STATE_FRIGHTENED)
			 *  1. Бегаем в пределах домика вверх вниз с испуганной скоростью.
			 *  2. По истечению определенного времени начинаем выходить из домика.
			 */

			if(mMonsters[i].mPlace == Monster::PLACE_ATHOME) {

			// если надо - выходим из домика
				if(mMonsters[i].mPlaceTime > lTimeInHome[i]) {
					mMonsters[i].mPlace = Monster::PLACE_GETTINGFROMHOME;
					mMonsters[i].mPlaceTime = 0.0f;
				}
			// бегаем вверх - вниз
				if(mMonsters[i].mCoords.getY() > 15.0f) {
					mMonsters[i].mDirection = Monster::DIRECTION_DOWN;
				} else {
					if(mMonsters[i].mCoords.getY() < 13.0f) {
						mMonsters[i].mDirection = Monster::DIRECTION_UP;
					}
				}
			}



			/*
			 *   Алгоритм поведения монстра когда он выходит из домика(PLACE_GETTINGFROMHOME)
			 *
			 *      1) Если он в (STATE_SCATTER или STATE_CHASE)
			 *  1. Если монстр находится не ровно песередине относительно домика - смещаемся к центру.
			 *  2. Выводим монстра через "воротики", как только он встал на игровую клетку переходим
			 *    в состояние преследования.
			 *
			 *      2) Если он в (STATE_FRIGHTENED)
			 *  1. -//-/-, только с меньшей скоростью
			 *  2. -//-/-, только с меньшей скоростью
			 */

			if(mMonsters[i].mPlace == Monster::PLACE_GETTINGFROMHOME) {

				bool lInCenter = false;
				// проверка в центре ли домика монстр
				if(mMonsters[i].mCoords.getX() >= 18.5-0.1f && mMonsters[i].mCoords.getX() <= 18.5+0.1f) {
					lInCenter = true;
					mMonsters[i].mCoords.set(18.5f, mMonsters[i].mCoords.getY());
				}
			// если монстр не в центре домика
				if(!lInCenter) {
					// если монстр по левую сторону от центра, перемещаем его к центру
					if(mMonsters[i].mCoords.getX() < 18.5) {
						mMonsters[i].mDirection = Monster::DIRECTION_RIGHT;
					} else {
						if(mMonsters[i].mCoords.getX() > 18.5) {
							// если монстр по правую сторону от центра перемецаем его к центру
							mMonsters[i].mDirection = Monster::DIRECTION_LEFT;
						}
					}
				} else {
				// если монстр в центре домика
					// если монстр не вышел из домика
					if(mMonsters[i].mCoords.getY() < 17.0f) {
						mMonsters[i].mDirection = Monster::DIRECTION_UP;
					} else {
					// если монстр вышел на игровую клетку - говорим, что он в лабиринте
						mMonsters[i].mPlace = Monster::PLACE_INLABYRINTH;
						mMonsters[i].mPlaceTime = 0.0f;
						mMonsters[i].mCoords.set(mMonsters[i].mCoords.getX(), 17.0f);
						switch(i) {
						case 0:
							mMonsters[i].mDirection = Monster::DIRECTION_RIGHT;
							break;
						case 1:
							mMonsters[i].mDirection = Monster::DIRECTION_LEFT;
							break;
						case 2:
							mMonsters[i].mDirection = Monster::DIRECTION_RIGHT;
							break;
						case 3:
							mMonsters[i].mDirection = Monster::DIRECTION_LEFT;
							break;
						}

					}
				}

			}



			/*
			 *   Алгоритм поведения монстра когда он в лабиринте (PLACE_INLABYRINTH)
			 *
			 *      1) Когда он в (STATE_CHASE)
			 *  1. Находим текущую ячейку(ближайшую) монстра.
			 *  2. Находим ближайшие ячейки, к которым может направиться монстр.
			 *    Игнорируем при этом ячейку, которая находится сзади по отношению к движению.
			 *  3. Из найденых ячеек, к которым может направиться монстр, выбираем ту, которая
			 *    переменстит монстра ближе к Pacman'y.
			 *  4. Сравнивая с какой стороны от монстра находится целевая
			 *    ячейка, меняем направление движения монстра.
			 *  5. По истечении определенного времени переходим в режим разбегания.
			 *
			 *
			 *  	2) Когда он в (STATE_SCATTER)
			 *  1. -//-/-
			 *  2. -//-/-
			 *  3. Из найденых ячеек, к которым может направиться монстр, выбираем ту, которая
			 *    переменстит монстра ближе к одному из углов лабиринта.
			 *  4. -//-/-
			 *  5. По истечении определенного времени переходим в режим преследования.
			 *
			 *
			 *  	3) Когда он в (STATE_FRIGHTENED)
			 *  1. -//-/-
			 *  2. -//-/-
			 *  3. Из найденых ячеек, выбираем рандомно любую из них.
			 *  4. -//-/-
			 *  5. По истечении определенного времени переходим в режим преследования.
			 */


			if(mMonsters[i].mPlace == Monster::PLACE_INLABYRINTH) {
			// находим ближайшую клетку к монстру
				int32_t li, lj; // идентификаторы ближайшей ячейки

				if((mMonsters[i].mCoords.getX()-floor(mMonsters[i].mCoords.getX())) <=
					(ceil(mMonsters[i].mCoords.getX())-mMonsters[i].mCoords.getX())) {
					lj = floor(mMonsters[i].mCoords.getX())-1;
				} else {
					lj = ceil(mMonsters[i].mCoords.getX())-1;
				}
				if((mMonsters[i].mCoords.getY()-floor(mMonsters[i].mCoords.getY())) <=
					(ceil(mMonsters[i].mCoords.getY())-mMonsters[i].mCoords.getY())) {
					li = floor(mMonsters[i].mCoords.getY())-1;
				} else {
					li = ceil(mMonsters[i].mCoords.getY())-1;
				}

				int32_t lNexti[3];
				int lNextj[3];
				int lNextCellsPossibilities = 0;
				float lTURN_ERROR = 0.15f;

			// находим ячейки, к которым может направиться монстр
				switch(mMonsters[i].mDirection) {
					case 0: // Up
						// передняя ячейка (верхнее направление)
						if(mCells[li+1][lj].mState == Cell::STATE_EMPTY) {
							lNextCellsPossibilities ++;
							lNexti[0] = li+1; lNextj[0] = lj;
						}
						// правая ячейка (правое направление)
						if(mCells[li][lj+1].mState == Cell::STATE_EMPTY
						&&fabs(mCells[li][lj].mCoords.getX() - mMonsters[i].mCoords.getY()) < lTURN_ERROR
						) {
							lNexti[lNextCellsPossibilities] = li;
							lNextj[lNextCellsPossibilities] = lj+1;
							lNextCellsPossibilities++;
						}
						// левая ячейка (левое направление)
						if(mCells[li][lj-1].mState == Cell::STATE_EMPTY
						&&fabs(mCells[li][lj].mCoords.getX() - mMonsters[i].mCoords.getY()) < lTURN_ERROR
						) {
							lNexti[lNextCellsPossibilities] = li;
							lNextj[lNextCellsPossibilities] = lj-1;
							lNextCellsPossibilities++;
						}
						break;
					case 2: // Down
						// передняя ячейка (нижнее направление)
						if(mCells[li-1][lj].mState == Cell::STATE_EMPTY) {
							lNexti[lNextCellsPossibilities] = li-1;
							lNextj[lNextCellsPossibilities] = lj;
							lNextCellsPossibilities++;
						}
						// левая ячейка (правое направление)
						if(mCells[li][lj-1].mState == Cell::STATE_EMPTY
						&&fabs(mCells[li][lj].mCoords.getX() - mMonsters[i].mCoords.getY()) < lTURN_ERROR
						) {
							lNexti[lNextCellsPossibilities] = li;
							lNextj[lNextCellsPossibilities] = lj-1;
							lNextCellsPossibilities++;
						}
						// правая ячейка (левое направление)
						if(mCells[li][lj+1].mState == Cell::STATE_EMPTY
						&&fabs(mCells[li][lj].mCoords.getX() - mMonsters[i].mCoords.getY()) < lTURN_ERROR
						) {
							lNexti[lNextCellsPossibilities] = li;
							lNextj[lNextCellsPossibilities] = lj+1;
							lNextCellsPossibilities++;
						}
						break;
					case 3: // Left
						// передняя ячейка (левое направление)
						if(mCells[li][lj-1].mState == Cell::STATE_EMPTY) {
							lNexti[lNextCellsPossibilities] = li;
							lNextj[lNextCellsPossibilities] = lj-1;
							lNextCellsPossibilities++;
						}
						// правая ячейка (верхнее направление)
						if(mCells[li+1][lj].mState == Cell::STATE_EMPTY
								&& fabs(mCells[li][lj].mCoords.getY() - mMonsters[i].mCoords.getX()) < lTURN_ERROR
								) {
								lNexti[lNextCellsPossibilities] = li+1;
								lNextj[lNextCellsPossibilities] = lj;
								lNextCellsPossibilities++;
						}
						// левая ячейка (нижнее направление)
						if(mCells[li-1][lj].mState == Cell::STATE_EMPTY
								&& fabs(mCells[li][lj].mCoords.getY() - mMonsters[i].mCoords.getX()) < lTURN_ERROR
								) {
								lNexti[lNextCellsPossibilities] = li-1;
								lNextj[lNextCellsPossibilities] = lj;
								lNextCellsPossibilities++;
						}
						break;
					case 1: // Right
						// передняя ячейка (правое направление)
						if(mCells[li][lj+1].mState == Cell::STATE_EMPTY) {
							lNexti[lNextCellsPossibilities] = li;
							lNextj[lNextCellsPossibilities] = lj+1;
							lNextCellsPossibilities++;
						}
						// правая ячейка (нижнее направление)
						if(mCells[li-1][lj].mState == Cell::STATE_EMPTY
								&& fabs(mCells[li][lj].mCoords.getY() - mMonsters[i].mCoords.getX()) < lTURN_ERROR
								) {
								lNexti[lNextCellsPossibilities] = li-1;
								lNextj[lNextCellsPossibilities] = lj;
								lNextCellsPossibilities++;
						}
						// левая ячейка (верхнее направление)
						if(mCells[li+1][lj].mState == Cell::STATE_EMPTY
								&& fabs(mCells[li][lj].mCoords.getY() - mMonsters[i].mCoords.getX()) < lTURN_ERROR
								) {
								lNexti[lNextCellsPossibilities] = li+1;
								lNextj[lNextCellsPossibilities] = lj;
								lNextCellsPossibilities++;
						}
						break;
					}

				// ----------------------------------------------------------------------

				// определяем target ячейку
				int32_t lTargeti, lTargetj;
				int32_t lFlag;
				float lShortestDistance = 9999999999999;
				float lCurrDistance;

				if(mMonsters[i].mState == Monster::STATE_CHASE) {
					if(lNextCellsPossibilities != 0) {
						for(int j=0; j<lNextCellsPossibilities;j++) {
							lCurrDistance = mPacman.mCoords.distSquared(mCells[lNexti[j]][lNextj[j]].mCoords.getY(),
																		 mCells[lNexti[j]][lNextj[j]].mCoords.getX());
							if(lCurrDistance < lShortestDistance) {
								lTargeti = lNexti[j];
								lTargetj = lNextj[j];
								lShortestDistance = lCurrDistance;
							}
						}
						}
				}

				if(mMonsters[i].mState == Monster::STATE_SCATTER) {
					if(lNextCellsPossibilities != 0) {
						for(int j=0; j<lNextCellsPossibilities;j++) {
								switch(i) {
								case 0:
									lCurrDistance = mCells[0][0].mCoords.distSquared(mCells[lNexti[j]][lNextj[j]].mCoords.getY(),
																				     mCells[lNexti[j]][lNextj[j]].mCoords.getX());
									break;
								case 1:
									lCurrDistance = mCells[0][34].mCoords.distSquared(mCells[lNexti[j]][lNextj[j]].mCoords.getY(),
																				     mCells[lNexti[j]][lNextj[j]].mCoords.getX());
									break;
								case 2:
									lCurrDistance = mCells[26][0].mCoords.distSquared(mCells[lNexti[j]][lNextj[j]].mCoords.getY(),
																				     mCells[lNexti[j]][lNextj[j]].mCoords.getX());
									break;
								case 3:
									lCurrDistance = mCells[26][34].mCoords.distSquared(mCells[lNexti[j]][lNextj[j]].mCoords.getY(),
																				     mCells[lNexti[j]][lNextj[j]].mCoords.getX());
									break;
								}
							if(lCurrDistance < lShortestDistance) {
								lTargeti = lNexti[j];
								lTargetj = lNextj[j];
								lShortestDistance = lCurrDistance;
							}
						}
					}
				}

				if(mMonsters[i].mState == Monster::STATE_FRIGHTENED) {
					float lsinValue = sinf(mMonsters[i].mStateTime);
					int32_t lRandomValue;
					// целочисленное число от 0 до 2
					if(lNextCellsPossibilities == 1) {
						lRandomValue = 0;
					}
					if(lNextCellsPossibilities == 2) {
						if(lsinValue > 0)  lRandomValue = 0;
						if(lsinValue <= 0) lRandomValue = 1;

					}
					if(lNextCellsPossibilities == 3) {
						if(lsinValue < -0.33f) 						 lRandomValue = 0;
						if(lsinValue >= -0.33f && lsinValue < 0.33f) lRandomValue = 1;
						if(lsinValue >= 0.33f) 						 lRandomValue = 2;
					}
					// ранодомно назначаем "target" ячейку
					if(lNextCellsPossibilities != 0) {
						lTargeti = lNexti[lRandomValue];
						lTargetj = lNextj[lRandomValue];
					}
				}


				float lDeltaTimeChanges = 0.2f;
				/*
				 *  Чтобы монстр не мог изменить свое направление два раза в течение очень
				 *  короткого промежутка времени вводим lDeltaTimeChanges - это значение
				 *  покажет через какое время может быть осуществлен следующий поворот
				 */
				// меняем направление монстра в зависимости от target cell
				if(lNextCellsPossibilities != 0) {
					switch(li==lTargeti) {
					// находится на оси X
						case true:
							switch(lj>lTargetj) {
								// находится левее оси Y
								case true:
									if(mMonsters[i].mDeltaTimeChangeDirections > lDeltaTimeChanges) {
									mMonsters[i].mDirection = Monster::DIRECTION_LEFT;
									if(mMonsters[i].mPreviousDirection == Monster::DIRECTION_UP ||
									   mMonsters[i].mPreviousDirection == Monster::DIRECTION_DOWN
									   ) {
											mMonsters[i].mCoords.set(mMonsters[i].mCoords.getX(), mCells[li][lj].mCoords.getX());
											mMonsters[i].mDeltaTimeChangeDirections = 0.0f;
									}
									}
									break;
									// находится правее оси Y
								case false:
									if(mMonsters[i].mDeltaTimeChangeDirections > lDeltaTimeChanges) {
									mMonsters[i].mDirection = Monster::DIRECTION_RIGHT;
									if(mMonsters[i].mPreviousDirection == Monster::DIRECTION_UP ||
									   mMonsters[i].mPreviousDirection == Monster::DIRECTION_DOWN) {
											mMonsters[i].mCoords.set(mMonsters[i].mCoords.getX(), mCells[li][lj].mCoords.getX());
											mMonsters[i].mDeltaTimeChangeDirections = 0.0f;
									}
									}
									break;
								}
							break;
							// находится ниже или выше оси X
							case false:
								switch(li<lTargeti) {
									// находится выше оси X
									case true:
										if(mMonsters[i].mDeltaTimeChangeDirections > lDeltaTimeChanges) {
										mMonsters[i].mDirection = Monster::DIRECTION_UP;
										if(mMonsters[i].mPreviousDirection == Monster::DIRECTION_LEFT ||
										   mMonsters[i].mPreviousDirection == Monster::DIRECTION_RIGHT) {
												mMonsters[i].mCoords.set(mCells[li][lj].mCoords.getY(), mMonsters[i].mCoords.getY());
												mMonsters[i].mDeltaTimeChangeDirections = 0.0f;
										}
										}
										break;
										// находится ниже оси X
									case false:
										if(mMonsters[i].mDeltaTimeChangeDirections > lDeltaTimeChanges) {
										mMonsters[i].mDirection = Monster::DIRECTION_DOWN;
										if(mMonsters[i].mPreviousDirection == Monster::DIRECTION_LEFT ||
										   mMonsters[i].mPreviousDirection == Monster::DIRECTION_RIGHT) {
												mMonsters[i].mCoords.set(mCells[li][lj].mCoords.getY(), mMonsters[i].mCoords.getY());
												mMonsters[i].mDeltaTimeChangeDirections = 0.0f;
										}
										}
										break;
									}
								break;
					}

				} else {
					/*
					 *  В лабиринте может сложится такая ситуация, что монстр попадет в тупик.
					 *  В этом случае меняем его направление на противоположное.
					 */

					switch(mMonsters[i].mDirection) {
					case Monster::DIRECTION_UP:
						if(mMonsters[i].mCoords.getY()>mCells[li][lj].mCoords.getX()) {
							mMonsters[i].mDirection = Monster::DIRECTION_DOWN;
						}
						break;
					case Monster::DIRECTION_RIGHT:
						if(mMonsters[i].mCoords.getX()>mCells[li][lj].mCoords.getY()) {
							mMonsters[i].mDirection = Monster::DIRECTION_LEFT;
						}
						break;
					case Monster::DIRECTION_DOWN:
						if(mMonsters[i].mCoords.getY()<mCells[li][lj].mCoords.getX()) {
							mMonsters[i].mDirection = Monster::DIRECTION_UP;
						}
						break;
					case Monster::DIRECTION_LEFT:
						if(mMonsters[i].mCoords.getX()<mCells[li][lj].mCoords.getY()) {
							mMonsters[i].mDirection = Monster::DIRECTION_RIGHT;
						}
						break;
					}
				}

				if(mMonsters[i].mState == Monster::STATE_CHASE) {
					// если время преследования истекло - переходим в режим разбегания
					if(mMonsters[i].mStateTime > Monster::TIME_CHASE) {
						mMonsters[i].mState = Monster::STATE_SCATTER;
						mMonsters[i].mStateTime = 0.0f;
					}
				}

				if(mMonsters[i].mState == Monster::STATE_SCATTER) {
					// если время разбегания истекло - переходим в режим преследования
					if(mMonsters[i].mStateTime > Monster::TIME_SCATTER) {
						mMonsters[i].mState = Monster::STATE_CHASE;
						mMonsters[i].mStateTime = 0.0f;
					}
				}

				if(mMonsters[i].mState == Monster::STATE_FRIGHTENED) {
					// если пакман находится очень близко пишем "не убивай меня!"
					float lPacmanDistance = mPacman.mCoords.distSquared(mMonsters[i].mCoords.getX(),
																		mMonsters[i].mCoords.getY());
					if(lPacmanDistance < 20) {
						mMonsters[i].mPacmanIsNear = true;
					} else {
						mMonsters[i].mPacmanIsNear = false;
					}
					// если время испуга истекло - переходим в режим преследования
					if(mMonsters[i].mStateTime > Monster::TIME_FRIGHTENED) {
						mMonsters[i].mState = Monster::STATE_CHASE;
						mMonsters[i].mStateTime = 0.0f;
					}
				}
			}


			if(mMonsters[i].mState == Monster::STATE_EATEN) {
				mMonsters[i].mDirection = Monster::DIRECTION_UP;

				// если время "съеденности" истекло - переходим в домик
				if(mMonsters[i].mStateTime > Monster::TIME_EATEN) {
					mMonsters[i].mPlace = Monster::PLACE_ATHOME;
					mMonsters[i].mState = Monster::STATE_CHASE;
					mMonsters[i].mStateTime = 0.0f;
					mMonsters[i].mPlaceTime = 0.0f;
					switch(mMonsters[i].mType) {
					case Monster::TYPE_RED :
						mMonsters[i].mCoords.set(18.5f, 15.0f);
						break;
					case Monster::TYPE_PINK :
						mMonsters[i].mCoords.set(18.5f, 13.0f);
						break;
					case Monster::TYPE_CYAN :
						mMonsters[i].mCoords.set(17.0f, 14.0f);
						break;
					case Monster::TYPE_BROWN :
						mMonsters[i].mCoords.set(20.0f, 14.0f);
						break;
					}
				}
			}


			// ----------------------------------------------------------------------------------------------------

			// телепортируем монстров при переходах
			if(fabs(mMonsters[i].mCoords.getX()-1.0f)<0.1f &&
			   fabs(mMonsters[i].mCoords.getY()-14.0f)<0.1f &
			   mMonsters[i].mDirection == Monster::DIRECTION_LEFT) {
				mMonsters[i].mCoords.set(35.0f, 14.0f);
			}
			if(fabs(mMonsters[i].mCoords.getX()-35.0f)<0.1f &&
			   fabs(mMonsters[i].mCoords.getY()-14.0f)<0.1f &
			    mMonsters[i].mDirection == Monster::DIRECTION_RIGHT) {
				mMonsters[i].mCoords.set(1.0f, 14.0f);
			}

			mMonsters[i].mDeltaTimeChangeDirections += pDeltaTime;
			mMonsters[i].mPreviousDirection = mMonsters[i].mDirection;
			mMonsters[i].update(pDeltaTime);
		}
	}


















	void World::updatePacman(float pDeltaTime) {

	// меняем направление движения Packman'a по перемещению пальца по экрану
		if(mInputService->getTouchAction() == InputService::TOUCH_DRAGGED) {
			float lDeltaX, lDeltaY;
			lDeltaX = mInputService->getTouchX()-mInputService->getPreviousTouchX();
			lDeltaY = mInputService->getTouchY()-mInputService->getPreviousTouchY();

			if(lDeltaX>0 		 // перемещение вправо
			&& lDeltaY>0 		 // перемещение вниз
			&& lDeltaX>lDeltaY){ // перемещения вправо больше чем вниз
				mPacman.mPreviousDirection = mPacman.mDirection;
				mPacman.mDirection = Pacman::DIRECTION_RIGHT;
				if(mPacman.mState == Pacman::STATE_STAY) {
					// ...
				} else {
					mPacman.mState = Pacman::STATE_RUN;
				}
			} else {
				if(lDeltaX>0		 // перемещение вправо
				&& lDeltaY>0		 // перемещение вниз
				&& lDeltaX<lDeltaY){ // перемещение вниз больше чем вправо
					mPacman.mPreviousDirection = mPacman.mDirection;
					mPacman.mDirection = Pacman::DIRECTION_DOWN;
					if(mPacman.mState == Pacman::STATE_STAY) {
						// ...
					} else {
						mPacman.mState = Pacman::STATE_RUN;
					}
				} else {
					if(lDeltaX<0		  // перемещение влево
					&& lDeltaY>0		  // перемещение вниз
					&&(-lDeltaX)>lDeltaY) {// перемещение влево больше чем вниз
						mPacman.mPreviousDirection = mPacman.mDirection;
						mPacman.mDirection = Pacman::DIRECTION_LEFT;
						if(mPacman.mState == Pacman::STATE_STAY) {
							// ...
						} else {
							mPacman.mState = Pacman::STATE_RUN;
						}
					} else {
						if(lDeltaX<0		  // перемещение влево
						&& lDeltaY>0		  // перемещение вниз
						&&(-lDeltaX)<lDeltaY){ // перемещение вниз больше чем влево
							mPacman.mPreviousDirection = mPacman.mDirection;
							mPacman.mDirection = Pacman::DIRECTION_DOWN;
							if(mPacman.mState == Pacman::STATE_STAY) {
								// ...
							} else {
								mPacman.mState = Pacman::STATE_RUN;
							}
						} else {
							if(lDeltaX>0		   // перемещение вправо
							&& lDeltaY<0		   // перемещение вверх
							&& lDeltaX>(-lDeltaY)){ // перемещение вправо больше чем вверх
								mPacman.mPreviousDirection = mPacman.mDirection;
								mPacman.mDirection = Pacman::DIRECTION_RIGHT;
								if(mPacman.mState == Pacman::STATE_STAY) {
									// ...
								} else {
									mPacman.mState = Pacman::STATE_RUN;
								}
							} else {
								if(lDeltaX>0		   // перемещение вправо
								&& lDeltaY<0		   // перемещение вверх
								&& lDeltaX<(-lDeltaY)){ // перемещение вверх больше чем вправо
									mPacman.mPreviousDirection = mPacman.mDirection;
									mPacman.mDirection = Pacman::DIRECTION_UP;
									if(mPacman.mState == Pacman::STATE_STAY) {
										// ...
									} else {
										mPacman.mState = Pacman::STATE_RUN;
									}
								} else {
									if(lDeltaX<0		   // перемещение влево
									&& lDeltaY<0		   // перемещение вверх
									&& (-lDeltaX)>(-lDeltaY)){ // перемещение влево больше чем вверх
										mPacman.mPreviousDirection = mPacman.mDirection;
										mPacman.mDirection = Pacman::DIRECTION_LEFT;
										if(mPacman.mState == Pacman::STATE_STAY) {
											// ...
										} else {
											mPacman.mState = Pacman::STATE_RUN;
										}

									} else {
										if(lDeltaX<0		      // перемещение влево
										&& lDeltaY<0		      // перемещение вверх
										&& (-lDeltaX)<(-lDeltaY)){ // перемещение вверх больше чем влево
											mPacman.mPreviousDirection = mPacman.mDirection;
											mPacman.mDirection = Pacman::DIRECTION_UP;
											if(mPacman.mState == Pacman::STATE_STAY) {
												// ...
											} else {
												mPacman.mState = Pacman::STATE_RUN;
											}

										}
									}
								}
							}
						}
					}
				}
			}
		}

		int32_t li, lj; // идентификаторы ближайшей ячейки

		// здесь находим ближайшую ячейку к Packman'y
		if((mPacman.mCoords.getX()-floor(mPacman.mCoords.getX())) <= (ceil(mPacman.mCoords.getX())-mPacman.mCoords.getX())) {
			li = floor(mPacman.mCoords.getX());
		} else {
			li = ceil(mPacman.mCoords.getX());
		}
		if((mPacman.mCoords.getY()-floor(mPacman.mCoords.getY())) <= (ceil(mPacman.mCoords.getY())-mPacman.mCoords.getY())) {
			lj = floor(mPacman.mCoords.getY());
		} else {
			lj = ceil(mPacman.mCoords.getY());
		}


		int lNexti, lNextj; // идентификаторы ячейки, к которой хочет направиться Packman

		// тут определяем i и j ячейки, к которой хочет направиться Packman
		switch(mPacman.mDirection) {
			case 0: // Up
				lNexti = lj+1; lNextj = li;
				break;
			case 2: // Down
				lNexti = lj-1; lNextj = li;
				break;
			case 3: // Left
				lNexti = lj; lNextj = li-1;
				break;
			case 1: // Right
			    lNexti = lj; lNextj = li+1;
				break;
			}

		// Если ячейка к которой хочет направиться Packman недосягаема
		if(mCells[lNexti-1][lNextj-1].mState == Cell::STATE_UNREACHABLE) {
			switch(mPacman.mDirection) {
			// если при этом Packman хочет направиться наверх
				case 0:  // Direction Up
				// если направление в течение короткого промежутка времени не менялось
					if(mPacman.mPreviousDirection == mPacman.mDirection) {
					// если пекмен приблизился "слишком близко" к недосягаемой клетке остановить его
						if(mPacman.mCoords.getY()>mCells[lj-1][li-1].mCoords.getX()) {
							mPacman.mCoords.set(mPacman.mCoords.getX(), mCells[lj-1][li-1].mCoords.getX());
							mPacman.mState = mPacman.STATE_STAY;
						}
					} else {
					// если он хочет поменять направление относительно себя влево или вправо и при этом
					// боковая клетка недосягаема  Packman продолжает свое направление
						mPacman.mDirection = mPacman.mPreviousDirection;
					}
					break;
				case 2: // Direction Down
					if(mPacman.mPreviousDirection == mPacman.mDirection) {
						if(mPacman.mCoords.getY()<mCells[lj-1][li-1].mCoords.getX()) {
							mPacman.mCoords.set(mPacman.mCoords.getX(), mCells[lj-1][li-1].mCoords.getX());
							mPacman.mState = mPacman.STATE_STAY;
						}
					} else {
						mPacman.mDirection = mPacman.mPreviousDirection;
					}
					break;
				case 3: // Direction Left
					if(mPacman.mPreviousDirection == mPacman.mDirection) {
						if(mPacman.mCoords.getX()<mCells[lj-1][li-1].mCoords.getY()) {
							mPacman.mCoords.set(mCells[lj-1][li-1].mCoords.getY(), mPacman.mCoords.getY());
							mPacman.mState = mPacman.STATE_STAY;
						}
					} else {
						mPacman.mDirection = mPacman.mPreviousDirection;
					}
					break;
				case 1: // Direction Right
					if(mPacman.mPreviousDirection == mPacman.mDirection) {
						if(mPacman.mCoords.getX()>mCells[lj-1][li-1].mCoords.getY()) {
							mPacman.mCoords.set(mCells[lj-1][li-1].mCoords.getY(), mPacman.mCoords.getY());
							mPacman.mState = mPacman.STATE_STAY;
						}
					} else {
						mPacman.mDirection = mPacman.mPreviousDirection;
					}
					break;
				}
		} else { // если ячейка, куда хочет направится Packman доступна

			/*
			 *  Суть последующего блока в том, чтобы при поворотах он шел "ровно" по клеткам при смене
			 *   направления. TURN_ERROR - поргешность, которую можно допустить при повороте, чем она меньше
			 *   тем больше вероятность того, что пользователь не сможет повернуть т.к время когда можно
			 *    повернуть будет очень маленькое, в свою очередь чем меньше эта погрешность, тем более "гладко"
			 *    Packman будет совершать этот самый поворот
			 */
			switch(mPacman.mDirection) {
				case 0: // Up
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_LEFT) { // |_
						if(mPacman.mCoords.getX()>mCells[lj-1][li-1].mCoords.getY() + TURN_ERROR ||
						   mPacman.mCoords.getX()<mCells[lj-1][li-1].mCoords.getY() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_LEFT;
						} else {
							mPacman.mCoords.set(mCells[lj-1][li-1].mCoords.getY(), mPacman.mCoords.getY());
						}
					}
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_RIGHT) { // _|
						if(mPacman.mCoords.getX()>mCells[lj-1][li-1].mCoords.getY() + TURN_ERROR ||
						   mPacman.mCoords.getX()<mCells[lj-1][li-1].mCoords.getY() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_RIGHT;
						} else {
							mPacman.mCoords.set(mCells[lj-1][li-1].mCoords.getY(), mPacman.mCoords.getY());
						}
					}
					break;
				case 2: // Down													   _
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_LEFT) { // |
						if(mPacman.mCoords.getX()>mCells[lj-1][li-1].mCoords.getY() + TURN_ERROR ||
						   mPacman.mCoords.getX()<mCells[lj-1][li-1].mCoords.getY() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_LEFT;
						} else {
							mPacman.mCoords.set(mCells[lj-1][li-1].mCoords.getY(), mPacman.mCoords.getY());
						}
					}															//  _
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_RIGHT) { //   |
						if(mPacman.mCoords.getX()>mCells[lj-1][li-1].mCoords.getY() + TURN_ERROR ||
						   mPacman.mCoords.getX()<mCells[lj-1][li-1].mCoords.getY() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_RIGHT;
						} else {
							mPacman.mCoords.set(mCells[lj-1][li-1].mCoords.getY(), mPacman.mCoords.getY());
						}
					}
					break;
				case 3: // Left
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_UP) { //   |
						if(mPacman.mCoords.getY()>mCells[lj-1][li-1].mCoords.getX() + TURN_ERROR ||
						   mPacman.mCoords.getY()<mCells[lj-1][li-1].mCoords.getX() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_UP;
						} else {
							mPacman.mCoords.set( mPacman.mCoords.getX(),mCells[lj-1][li-1].mCoords.getX());
						}
					}
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_DOWN) {
						if(mPacman.mCoords.getY()>mCells[lj-1][li-1].mCoords.getX() + TURN_ERROR ||
						   mPacman.mCoords.getY()<mCells[lj-1][li-1].mCoords.getX() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_DOWN;
						} else {
							mPacman.mCoords.set( mPacman.mCoords.getX(),mCells[lj-1][li-1].mCoords.getX());
						}
					}
					break;
				case 1: // Right
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_UP) { //   |
						if(mPacman.mCoords.getY()>mCells[lj-1][li-1].mCoords.getX() + TURN_ERROR ||
						   mPacman.mCoords.getY()<mCells[lj-1][li-1].mCoords.getX() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_UP;
						} else {
							mPacman.mCoords.set( mPacman.mCoords.getX(),mCells[lj-1][li-1].mCoords.getX());
						}
					}
					if(mPacman.mPreviousDirection==Pacman::DIRECTION_DOWN) {
						if(mPacman.mCoords.getY()>mCells[lj-1][li-1].mCoords.getX() + TURN_ERROR ||
						   mPacman.mCoords.getY()<mCells[lj-1][li-1].mCoords.getX() - TURN_ERROR) {
							mPacman.mDirection = Pacman::DIRECTION_DOWN;
						} else {
							mPacman.mCoords.set( mPacman.mCoords.getX(),mCells[lj-1][li-1].mCoords.getX());
						}
					}
					break;
			}


			// если та клетка, к которой хочет направиться Packman досягаема позволим ему передвигаться дальше
			mPacman.mState = mPacman.STATE_RUN;
		}

		// телепортируем Packman'a при переходах
		if(fabs(mPacman.mCoords.getX()-1.0f)<0.1f &&
		   fabs(mPacman.mCoords.getY()-14.0f)<0.1f &
		   mPacman.mDirection == Pacman::DIRECTION_LEFT) {
			mPacman.mCoords.set(35.0f, 14.0f);
		}
		if(fabs(mPacman.mCoords.getX()-35.0f)<0.1f &&
		   fabs(mPacman.mCoords.getY()-14.0f)<0.1f &
		   mPacman.mDirection == Pacman::DIRECTION_RIGHT) {
			mPacman.mCoords.set(1.0f, 14.0f);
		}

		mPacman.mPreviousDirection = mPacman.mDirection;
		mPacman.update(pDeltaTime);
	}




	void World::checkGameOver() {
		if(mPacmanLives == 0) {
			mState = STATE_GAMEOVER;
		}
	}

	void World::checkTimeIsUp() {
		if(mTimeLeft <= 1) {
			mState = STATE_TIMEISUP;
		}
	}

	void World::checkTouchEvent() {
		if(mInputService->getTouchAction() == InputService::TOUCH_UP) {
			if(mState == STATE_TIMEISUP || mState == STATE_GAMEOVER) {
				 if(mSoundService->soundEnabled()) {
					mSoundService->playSound(mSoundService->getAssets()->mSoundSadness);
				 }
				mState = STATE_SHOWENDSCORE;
			} else {
				if(mState == STATE_SHOWENDSCORE) {
					mState = STATE_EXIT;
				}
			}
		}
	}

	void World::checkCollisions() {
		checkMonsterCollisions();
		checkStarCollisions();
		checkCandyCollisions();
		checkCandyStrongerCollisions();
	}

	void World::checkMonsterCollisions() {
		Rectangle lRPacman(mPacman.mCoords.getX(),
						    mPacman.mCoords.getY(),
						    mPacman.WIDTH-0.3f,
						    mPacman.HEIGHT-0.3f);

		Rectangle lRMonsters[4];

		for(int i=0;i<4;i++) {
			lRMonsters[i].setInfo(mMonsters[i].mCoords.getX(),
								  mMonsters[i].mCoords.getY(),
								  mMonsters[i].WIDTH-0.3f,
								  mMonsters[i].HEIGHT-0.3f);
			// если Pacman касается монстра
				if(mOverlapTester.overlapRectangles(&lRPacman,&lRMonsters[i])) {
					// когда пакман кушает монстра
						if(mMonsters[i].mState == Monster::STATE_FRIGHTENED) {
							mMonsters[i].mState = Monster::STATE_EATEN;
							mMonsters[i].mStateTime = 0.0f;
							mMonsters[i].mPacmanIsNear = false;
							mScore += Monster::SCORE_INCREMENT;
							if(mSoundService->soundEnabled()){
								mSoundService->playSound(mSoundService->getAssets()->mSoundPacmanEat);
							}
						}
					// когда монстр кушает пакмана
						if(mMonsters[i].mState == Monster::STATE_CHASE ||
						   mMonsters[i].mState == Monster::STATE_SCATTER) {
							if(mPacmanLives>0) {
								mPacmanLives--;
							}
							if(mSoundService->soundEnabled() && mPacmanLives != 0){
								mSoundService->playSound(mSoundService->getAssets()->mSoundSadness);
							}
							for(int j=0;j<4;j++) {
								mMonsters[j].mPlace = Monster::PLACE_ATHOME;
								mMonsters[j].mState = Monster::STATE_CHASE;
								mMonsters[j].mPlaceTime = -TIME_WAIT;
								mMonsters[j].mStateTime = 0.0f;
							switch(mMonsters[j].mType) {
							case Monster::TYPE_RED :
								mMonsters[j].mCoords.set(18.5f, 15.0f);
								mMonsters[j].mDirection = Monster::DIRECTION_UP;
								break;
							case Monster::TYPE_PINK :
								mMonsters[j].mCoords.set(18.5f, 13.0f);
								mMonsters[j].mDirection = Monster::DIRECTION_UP;
								break;
							case Monster::TYPE_CYAN :
								mMonsters[j].mCoords.set(17.0f, 14.0f);
								mMonsters[j].mDirection = Monster::DIRECTION_UP;
								break;
							case Monster::TYPE_BROWN :
								mMonsters[j].mCoords.set(20.0f, 14.0f);
								mMonsters[j].mDirection = Monster::DIRECTION_UP;
								break;
							}
						}
							mState = STATE_WAITING;
							mStateTime = 0.0f;
						}
				}

		}
	}

	void World::checkStarCollisions() {

		Rectangle lRPacman(mPacman.mCoords.getX(),
						   mPacman.mCoords.getY(),
						   mPacman.WIDTH-0.1f,
						   mPacman.HEIGHT-0.1f);

		Rectangle lRStar(0.0f, 0.0f, 0.0f, 0.0f);


		for(int i=0;i<119;i++) {
			if(mStars[i].mState == Star::STATE_EXIST) {
				lRStar.setInfo(mStars[i].mCoords.getX(),
							   mStars[i].mCoords.getY(),
							   mStars[i].WIDTH-0.1f,
							   mStars[i].HEIGHT-0.1f);
				if(mOverlapTester.overlapRectangles(&lRPacman, &lRStar)) {
					mStars[i].mState = Star::STATE_NONE;
					mScore += Star::SCORE_INCREMENT;
				    if(mSoundService->soundEnabled()) {
						 mSoundService->playSound(mSoundService->getAssets()->mSoundPacmanEat);
					}
				}
			}
		}

	// если вся левая сторона съедена - создаем правую конфетку
		int lCounter = 0;
		for(int i=0; i<62; i++) {
			if(mStars[i].mState != Star::STATE_NONE) {
				lCounter++;
			}
		}
		if(lCounter == 0) mCandies[1].mState = Candy::STATE_EXIST;

		lCounter = 0;
	// если вся правая сторона съедена - создаем левую конфетку
		for(int i=62; i<119; i++) {
			if(mStars[i].mState != Star::STATE_NONE) {
				lCounter++;
			}
		}
		if(lCounter == 0) mCandies[0].mState = Candy::STATE_EXIST;
	}

	void World::checkCandyCollisions() {
		Rectangle lRPackman(mPacman.mCoords.getX(),
						    mPacman.mCoords.getY(),
						    mPacman.WIDTH-0.1f,
						    mPacman.HEIGHT-0.1f);

		Rectangle lRCandy1(mCandies[0].mCoords.getX(),
						   mCandies[0].mCoords.getY(),
						   mCandies[0].WIDTH-0.1f,
						   mCandies[0].HEIGHT-0.1f);

		Rectangle lRCandy2(mCandies[1].mCoords.getX(),
						   mCandies[1].mCoords.getY(),
						   mCandies[1].WIDTH-0.1f,
						   mCandies[1].HEIGHT-0.1f);

	// если Packman ест левую карамельку - создаем звездочки по правую сторону
		if(mCandies[0].mState == Candy::STATE_EXIST) {
		if(mOverlapTester.overlapRectangles(&lRPackman, &lRCandy1)) {
			for(int i=62;i<119;i++) {
				mStars[i].mState = mStars[i].STATE_INIT;
				mStars[i].mStateTime = 0.0f;
				mCandies[0].mState = mCandies[0].STATE_NONE;
			}
		    if(mSoundService->soundEnabled()) {
				 mSoundService->playSound(mSoundService->getAssets()->mSoundPacmanEat);
			}
			mScore += mCandies[0].SCORE_INCREMENT;
		}
		}
	// если Packman ест правую карамельку - создаем звездочки по левую сторону
		if(mCandies[1].mState == Candy::STATE_EXIST) {
		if(mOverlapTester.overlapRectangles(&lRPackman, &lRCandy2)) {
			for(int i=0;i<62;i++) {
				mStars[i].mState = Star::STATE_INIT;
				mStars[i].mStateTime = 0.0f;
				mCandies[1].mState = Candy::STATE_NONE;
			}
			mScore += Candy::SCORE_INCREMENT;
		    if(mSoundService->soundEnabled()) {
				 mSoundService->playSound(mSoundService->getAssets()->mSoundPacmanEat);
			}
		}
	}
	}

	void World::checkCandyStrongerCollisions() {
		Rectangle lRPacman(mPacman.mCoords.getX(),
						    mPacman.mCoords.getY(),
						    mPacman.WIDTH-0.1f,
						    mPacman.HEIGHT-0.1f);

		Rectangle lRCandies[4];

		for(int i=0;i<4;i++) {
			lRCandies[i].setInfo(mCandiesStrong[i].mCoords.getX(),
							      mCandiesStrong[i].mCoords.getY(),
							      mCandiesStrong[i].WIDTH-0.1f,
							      mCandiesStrong[i].HEIGHT-0.1f);
			// если Pacman ест карамельку всех монстров переводим в режим страха
			if(mCandiesStrong[i].mState == CandyStrongMaker::STATE_EXIST) {
				if(mOverlapTester.overlapRectangles(&lRPacman,&lRCandies[i])) {
					for(int j=0; j<4; j++) {
						if(mMonsters[j].mState != Monster::STATE_EATEN) {
							mMonsters[j].mState = Monster::STATE_FRIGHTENED;
							mMonsters[j].mStateTime = 0.0f;
							if(mMonsters[j].mPlace == Monster::PLACE_INLABYRINTH) {
								switch(mMonsters[j].mDirection) {
								case Monster::DIRECTION_DOWN :
									mMonsters[j].mDirection = Monster::DIRECTION_UP;
									break;
								case Monster::DIRECTION_UP :
									mMonsters[j].mDirection = Monster::DIRECTION_DOWN;
									break;
								case Monster::DIRECTION_LEFT :
									mMonsters[j].mDirection = Monster::DIRECTION_RIGHT;
									break;
								case Monster::DIRECTION_RIGHT :
									mMonsters[j].mDirection = Monster::DIRECTION_LEFT;
									break;
								}
							}
						}
					}
				    if(mSoundService->soundEnabled()) {
						 mSoundService->playSound(mSoundService->getAssets()->mSoundPacmanEat);
					}
					mCandiesStrong[i].mState = CandyStrongMaker::STATE_NONE;
					mScore += CandyStrongMaker::SCORE_INCREMENT;
				}
			}
		}
	}

	void World::fillMonstersInfo() {
		mMonsters[0].setInfo(18.5f, 15.0f, Monster::STATE_CHASE, Monster::PLACE_ATHOME, Monster::TYPE_RED, Monster::DIRECTION_DOWN);
		mMonsters[1].setInfo(18.5f, 13.0f, Monster::STATE_CHASE, Monster::PLACE_ATHOME, Monster::TYPE_PINK, Monster::DIRECTION_UP);
		mMonsters[2].setInfo(17.0f, 14.0f, Monster::STATE_CHASE, Monster::PLACE_ATHOME, Monster::TYPE_CYAN, Monster::DIRECTION_UP);
		mMonsters[3].setInfo(20.0f, 14.0f, Monster::STATE_CHASE, Monster::PLACE_ATHOME, Monster::TYPE_BROWN, Monster::DIRECTION_UP);
	}

	void World::fillCandiesInfo() {
	// левая карамелька
		mCandies[0].setInfo(14.0f, 14.0f, Candy::STATE_NONE);
	// правая карамелька
		mCandies[1].setInfo(23.0f, 14.0f, Candy::STATE_NONE);

	// левая нижняя усиливающая карамелька
		mCandiesStrong[0].setInfo(2.0f, 6.0f, CandyStrongMaker::STATE_EXIST);
	// правая нижняя усиливающая карамелька
		mCandiesStrong[1].setInfo(34.0f, 6.0f, CandyStrongMaker::STATE_EXIST);
	// левая верхняя усиливающая карамелька
		mCandiesStrong[2].setInfo(2.0f, 22.0f, CandyStrongMaker::STATE_EXIST);
	// правая верхняя усиливающая карамелька
		mCandiesStrong[3].setInfo(34.0f, 22.0f, CandyStrongMaker::STATE_EXIST);
	}

	void World::fillStarsInfo() {
		int32_t lState_INIT = Star::STATE_INIT;

	// левая сторона
		mStars[0].setInfo(5, 11, lState_INIT);   mStars[1].setInfo(6, 11, lState_INIT);
		mStars[2].setInfo(7, 11, lState_INIT);   mStars[3].setInfo(8, 11, lState_INIT);
		mStars[4].setInfo(9, 11, lState_INIT);	 mStars[5].setInfo(10, 11, lState_INIT);
		mStars[6].setInfo(11, 11, lState_INIT);  mStars[7].setInfo(11, 10, lState_INIT);
		mStars[8].setInfo(11, 9, lState_INIT);   mStars[9].setInfo(11, 8, lState_INIT);
		mStars[10].setInfo(11, 7, lState_INIT);  mStars[11].setInfo(11, 6, lState_INIT);
		mStars[12].setInfo(11, 5, lState_INIT);  mStars[13].setInfo(10, 12, lState_INIT);
		mStars[14].setInfo(10, 13, lState_INIT); mStars[15].setInfo(10, 14, lState_INIT);
		mStars[16].setInfo(10, 15, lState_INIT); mStars[17].setInfo(10, 16, lState_INIT);
		mStars[18].setInfo(10, 17, lState_INIT); mStars[19].setInfo(11, 17, lState_INIT);
		mStars[20].setInfo(12, 17, lState_INIT); mStars[21].setInfo(13, 17, lState_INIT);
		mStars[22].setInfo(9, 17, lState_INIT);  mStars[23].setInfo(8, 17, lState_INIT);
		mStars[24].setInfo(7, 17, lState_INIT);  mStars[25].setInfo(6, 17, lState_INIT);
		mStars[26].setInfo(5, 17, lState_INIT);  mStars[27].setInfo(5, 4, lState_INIT);
		mStars[28].setInfo(5, 5, lState_INIT);   mStars[29].setInfo(5, 6, lState_INIT);
		mStars[30].setInfo(5, 7, lState_INIT);   mStars[31].setInfo(5, 8, lState_INIT);
		mStars[32].setInfo(5, 9, lState_INIT);   mStars[33].setInfo(5, 10, lState_INIT);
		mStars[34].setInfo(5, 11, lState_INIT);  mStars[35].setInfo(6, 4, lState_INIT);
		mStars[36].setInfo(7, 4, lState_INIT);   mStars[37].setInfo(8, 4, lState_INIT);
		mStars[38].setInfo(9, 4, lState_INIT);   mStars[39].setInfo(10, 4, lState_INIT);
		mStars[40].setInfo(11, 4, lState_INIT);  mStars[41].setInfo(12, 11, lState_INIT);
		mStars[42].setInfo(13, 11, lState_INIT); mStars[43].setInfo(5, 18, lState_INIT);
		mStars[44].setInfo(5, 19, lState_INIT);  mStars[45].setInfo(5, 20, lState_INIT);
		mStars[46].setInfo(5, 21, lState_INIT);  mStars[47].setInfo(5, 22, lState_INIT);
		mStars[48].setInfo(5, 23, lState_INIT);  mStars[49].setInfo(5, 24, lState_INIT);
		mStars[50].setInfo(6, 24, lState_INIT);  mStars[51].setInfo(7, 24, lState_INIT);
		mStars[52].setInfo(8, 24, lState_INIT);  mStars[53].setInfo(9, 24, lState_INIT);
		mStars[54].setInfo(10, 24, lState_INIT); mStars[55].setInfo(11, 24, lState_INIT);
		mStars[56].setInfo(11, 23, lState_INIT); mStars[57].setInfo(11, 22, lState_INIT);
		mStars[58].setInfo(11, 21, lState_INIT); mStars[59].setInfo(11, 20, lState_INIT);
		mStars[60].setInfo(11, 19, lState_INIT); mStars[61].setInfo(11, 18, lState_INIT);

	// правая сторона
		mStars[62].setInfo(31, 11, lState_INIT);  mStars[63].setInfo(30, 11, lState_INIT);
		mStars[64].setInfo(29, 11, lState_INIT);  mStars[65].setInfo(28, 11, lState_INIT);
		mStars[66].setInfo(27, 11, lState_INIT);  mStars[67].setInfo(26, 11, lState_INIT);
		mStars[68].setInfo(26, 10, lState_INIT);  mStars[69].setInfo(26, 9, lState_INIT);
		mStars[70].setInfo(26, 8, lState_INIT);   mStars[71].setInfo(26, 7, lState_INIT);
		mStars[72].setInfo(26, 6, lState_INIT);   mStars[73].setInfo(26, 5, lState_INIT);
		mStars[74].setInfo(27, 12, lState_INIT);  mStars[75].setInfo(27, 13, lState_INIT);
		mStars[76].setInfo(27, 14, lState_INIT);  mStars[77].setInfo(27, 15, lState_INIT);
		mStars[78].setInfo(27, 16, lState_INIT);  mStars[79].setInfo(27, 17, lState_INIT);
		mStars[80].setInfo(26, 17, lState_INIT);  mStars[81].setInfo(25, 17, lState_INIT);
		mStars[82].setInfo(24, 17, lState_INIT);  mStars[83].setInfo(28, 17, lState_INIT);
		mStars[84].setInfo(29, 17, lState_INIT);  mStars[85].setInfo(30, 17, lState_INIT);
		mStars[86].setInfo(31, 17, lState_INIT);  mStars[87].setInfo(31, 4, lState_INIT);
		mStars[88].setInfo(31, 5, lState_INIT);   mStars[89].setInfo(31, 6, lState_INIT);
		mStars[90].setInfo(31, 7, lState_INIT);   mStars[91].setInfo(31, 8, lState_INIT);
		mStars[92].setInfo(31, 9, lState_INIT);   mStars[93].setInfo(31, 10, lState_INIT);
		mStars[94].setInfo(30, 4, lState_INIT);   mStars[95].setInfo(29, 4, lState_INIT);
		mStars[96].setInfo(28, 4, lState_INIT);   mStars[97].setInfo(27, 4, lState_INIT);
		mStars[98].setInfo(26, 4, lState_INIT);   mStars[99].setInfo(25, 11, lState_INIT);
		mStars[100].setInfo(24, 11, lState_INIT); mStars[101].setInfo(31, 18, lState_INIT);
		mStars[102].setInfo(31, 19, lState_INIT); mStars[103].setInfo(31, 20, lState_INIT);
		mStars[104].setInfo(31, 21, lState_INIT); mStars[105].setInfo(31, 22, lState_INIT);
		mStars[106].setInfo(31, 23, lState_INIT); mStars[107].setInfo(31, 24, lState_INIT);
		mStars[108].setInfo(30, 24, lState_INIT); mStars[109].setInfo(29, 24, lState_INIT);
		mStars[110].setInfo(28, 24, lState_INIT); mStars[111].setInfo(27, 24, lState_INIT);
		mStars[112].setInfo(26, 24, lState_INIT); mStars[113].setInfo(26, 23, lState_INIT);
		mStars[114].setInfo(26, 22, lState_INIT); mStars[115].setInfo(26, 21, lState_INIT);
		mStars[116].setInfo(26, 20, lState_INIT); mStars[117].setInfo(26, 19, lState_INIT);
		mStars[118].setInfo(26, 18, lState_INIT);
	}

	void World::fillCells() {
	// заполняем информацию о лабиринте
		static const int32_t lSTATE_UNREACHABLE = Cell::STATE_UNREACHABLE;
		static const int32_t lSTATE_EMPTY = Cell::STATE_EMPTY;
		static const int32_t lSTATE_FILLED_PACKMAN = Cell::STATE_FILLED_PACKMAN;
		static const int32_t lSTATE_FILLED_MONSTER = Cell::STATE_FILLED_MONSTER;

	/*
	 * первая размерность - строка лабиринта
	 * вторая размерность - столбик лабиринта
	 * начало с левого нижнего угла
	 */

	// заполняем координаты каждой клетки
		for(int i=0; i<27; i++) {
			for(int j=0;j<35;j++) {
				mCells[i][j].mCoords.set(i+1,j+1);
			}
		}

	// заполняем состояние каждой клетки построчно
	// для 1-ой строки
		for(int i=0;i<35;i++) {
			mCells[0][i].mState = lSTATE_UNREACHABLE;
		}

	// для 2-ой строки
		mCells[1][0].mState = lSTATE_UNREACHABLE;
		for(int i=1;i<35;i++) {
			mCells[1][i].mState = lSTATE_EMPTY;
		}
		mCells[1][34].mState = lSTATE_UNREACHABLE;

	// для 3-ей строки
		for(int i=0;i<35;i++) {
			mCells[2][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[2][1].mState = lSTATE_EMPTY;  mCells[2][7].mState = lSTATE_EMPTY;
		mCells[2][13].mState = lSTATE_EMPTY; mCells[2][16].mState = lSTATE_EMPTY;
		mCells[2][21].mState = lSTATE_EMPTY; mCells[2][33].mState = lSTATE_EMPTY;

	// для 4-ой строки
		for(int i=0;i<35;i++) {
			mCells[3][i].mState = lSTATE_EMPTY;
		}
		mCells[3][0].mState = lSTATE_UNREACHABLE;  mCells[3][19].mState = lSTATE_UNREACHABLE;
		mCells[3][20].mState = lSTATE_UNREACHABLE; mCells[3][34].mState = lSTATE_UNREACHABLE;

	// для 5-ой строки
		for(int i=0;i<35;i++) {
			mCells[4][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[4][1].mState = lSTATE_EMPTY;  mCells[4][4].mState = lSTATE_EMPTY;
		mCells[4][10].mState = lSTATE_EMPTY; mCells[4][15].mState = lSTATE_EMPTY;
		mCells[4][18].mState = lSTATE_EMPTY; mCells[4][21].mState = lSTATE_EMPTY;
		mCells[4][25].mState = lSTATE_EMPTY; mCells[4][30].mState = lSTATE_EMPTY;
		mCells[4][33].mState = lSTATE_EMPTY;


	// для 6-ой строки
		for(int i=0;i<35;i++) {
			mCells[5][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[5][1].mState = lSTATE_EMPTY;  mCells[5][4].mState = lSTATE_EMPTY;
		mCells[5][10].mState = lSTATE_EMPTY; mCells[5][15].mState = lSTATE_EMPTY;
		mCells[5][18].mState = lSTATE_EMPTY; mCells[5][19].mState = lSTATE_EMPTY;
		mCells[5][20].mState = lSTATE_EMPTY; mCells[5][21].mState = lSTATE_EMPTY;
		mCells[5][25].mState = lSTATE_EMPTY; mCells[5][30].mState = lSTATE_EMPTY;
		mCells[5][33].mState = lSTATE_EMPTY;

	// для 7-ой строки
		for(int i=0;i<35;i++) {
			mCells[6][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[6][1].mState = lSTATE_EMPTY;  mCells[6][4].mState = lSTATE_EMPTY;
		mCells[6][10].mState = lSTATE_EMPTY; mCells[6][15].mState = lSTATE_EMPTY;
		mCells[6][21].mState = lSTATE_EMPTY; mCells[6][25].mState = lSTATE_EMPTY;
		mCells[6][30].mState = lSTATE_EMPTY; mCells[6][33].mState = lSTATE_EMPTY;

	// для 8-ой строки
		for(int i=0;i<35;i++) {
			mCells[7][i].mState = lSTATE_UNREACHABLE;
		}
		for(int i=1;i<5;i++) {
			mCells[7][i].mState = lSTATE_EMPTY;
		}
		mCells[7][10].mState = lSTATE_EMPTY; mCells[7][15].mState = lSTATE_EMPTY;
		mCells[7][20].mState = lSTATE_EMPTY; mCells[7][21].mState = lSTATE_EMPTY;
		mCells[7][25].mState = lSTATE_EMPTY;
		for(int i=30;i<34;i++) {
			mCells[7][i].mState = lSTATE_EMPTY;
		}

	// для 9-ой строки
		for(int i=0;i<35;i++) {
			mCells[8][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[8][4].mState = lSTATE_EMPTY;  mCells[8][10].mState = lSTATE_EMPTY;
		mCells[8][15].mState = lSTATE_EMPTY; mCells[8][20].mState = lSTATE_EMPTY;
		mCells[7][24].mState = lSTATE_EMPTY; mCells[8][25].mState = lSTATE_EMPTY;
		mCells[8][30].mState = lSTATE_EMPTY;

	// для 10-ой строки
		for(int i=0;i<35;i++) {
			mCells[9][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[9][4].mState = lSTATE_EMPTY;  mCells[9][10].mState = lSTATE_EMPTY;
		mCells[9][15].mState = lSTATE_EMPTY; mCells[9][20].mState = lSTATE_EMPTY;
		mCells[9][25].mState = lSTATE_EMPTY; mCells[9][30].mState = lSTATE_EMPTY;

	// для 11-ой строки
		for(int i=0; i<35; i++) {
			mCells[10][i].mState = lSTATE_UNREACHABLE;
		}
		for(int i=4;i<31;i++) {
			mCells[10][i].mState = lSTATE_EMPTY;
		}

	// для 12-ой строки
		for(int i=0;i<35;i++) {
			mCells[11][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[11][9].mState = lSTATE_EMPTY;  mCells[11][13].mState = lSTATE_EMPTY;
		mCells[11][22].mState = lSTATE_EMPTY; mCells[11][26].mState = lSTATE_EMPTY;

	// для 13-ой строки
		for(int i=0;i<35;i++) {
			mCells[12][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[12][9].mState = lSTATE_EMPTY;  mCells[12][13].mState = lSTATE_EMPTY;
		mCells[12][22].mState = lSTATE_EMPTY; mCells[12][26].mState = lSTATE_EMPTY;

	// для 14-ой строки
		for(int i=0;i<35;i++) {
			mCells[13][i].mState = lSTATE_UNREACHABLE;
		}
		for(int i=0;i<10;i++) {
			mCells[13][i].mState = lSTATE_EMPTY;
		}
		mCells[13][13].mState = lSTATE_EMPTY;
		mCells[13][22].mState = lSTATE_EMPTY;
		for(int i=26;i<35;i++) {
			mCells[13][i].mState = lSTATE_EMPTY;
		}

	// для 15-ой строки
		for(int i=0;i<35;i++) {
			mCells[14][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[14][9].mState = lSTATE_EMPTY;  mCells[14][13].mState = lSTATE_EMPTY;
		mCells[14][22].mState = lSTATE_EMPTY; mCells[14][26].mState = lSTATE_EMPTY;

	// для 16-ой строки
		for(int i=0;i<35;i++) {
			mCells[15][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[15][9].mState = lSTATE_EMPTY;  mCells[15][13].mState = lSTATE_EMPTY;
		mCells[15][22].mState = lSTATE_EMPTY; mCells[15][26].mState = lSTATE_EMPTY;

	// для 17-ой строки
		for(int i=0;i<35;i++) {
			mCells[16][i].mState = lSTATE_UNREACHABLE;
		}
		for(int i=4;i<31;i++) {
			mCells[16][i].mState = lSTATE_EMPTY;
		}

	// для 18-ой строки
		for(int i=0;i<35;i++) {
			mCells[17][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[17][4].mState = lSTATE_EMPTY;  mCells[17][7].mState = lSTATE_EMPTY;
		mCells[17][10].mState = lSTATE_EMPTY; mCells[17][15].mState = lSTATE_EMPTY;
		mCells[17][20].mState = lSTATE_EMPTY; mCells[17][25].mState = lSTATE_EMPTY;
		mCells[17][30].mState = lSTATE_EMPTY;

	// для 19-ой строки
		for(int i=0;i<35;i++) {
			mCells[18][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[18][4].mState = lSTATE_EMPTY;  mCells[18][7].mState = lSTATE_EMPTY;
		mCells[18][10].mState = lSTATE_EMPTY; mCells[18][15].mState = lSTATE_EMPTY;
		mCells[18][20].mState = lSTATE_EMPTY; mCells[18][25].mState = lSTATE_EMPTY;
		mCells[18][30].mState = lSTATE_EMPTY;

	// для 20-ой строки
		for(int i=0;i<35;i++) {
			mCells[19][i].mState = lSTATE_UNREACHABLE;
		}
		for(int i=1;i<5;i++) {
			mCells[19][i].mState = lSTATE_EMPTY;
		}
		mCells[19][10].mState = lSTATE_EMPTY;
		mCells[19][15].mState = lSTATE_EMPTY;
		for(int i=18;i<21;i++) {
			mCells[19][i].mState = lSTATE_EMPTY;
		}
		for(int i=23;i<26;i++) {
			mCells[19][i].mState = lSTATE_EMPTY;
		}
		for(int i=30;i<34;i++) {
			mCells[19][i].mState = lSTATE_EMPTY;
		}

	// для 21-ой строки
		for(int i=0;i<35;i++) {
			mCells[20][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[20][1].mState = lSTATE_EMPTY;  mCells[20][4].mState = lSTATE_EMPTY;
		mCells[20][10].mState = lSTATE_EMPTY; mCells[20][15].mState = lSTATE_EMPTY;
		mCells[20][18].mState = lSTATE_EMPTY; mCells[20][20].mState = lSTATE_EMPTY;
		mCells[20][23].mState = lSTATE_EMPTY; mCells[20][25].mState = lSTATE_EMPTY;
		mCells[20][30].mState = lSTATE_EMPTY; mCells[20][33].mState = lSTATE_EMPTY;

	// для 22-ой строки
		for(int i=0;i<35;i++) {
			mCells[21][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[21][1].mState = lSTATE_EMPTY;  mCells[21][4].mState = lSTATE_EMPTY;
		mCells[21][10].mState = lSTATE_EMPTY; mCells[21][15].mState = lSTATE_EMPTY;
		mCells[21][18].mState = lSTATE_EMPTY; mCells[21][20].mState = lSTATE_EMPTY;
		mCells[21][23].mState = lSTATE_EMPTY; mCells[21][25].mState = lSTATE_EMPTY;
		mCells[21][30].mState = lSTATE_EMPTY; mCells[21][33].mState = lSTATE_EMPTY;

	// для 23-ей строки
		for(int i=0;i<35;i++) {
			mCells[22][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[22][1].mState = lSTATE_EMPTY;  mCells[22][4].mState = lSTATE_EMPTY;
		mCells[22][7].mState = lSTATE_EMPTY;  mCells[22][10].mState = lSTATE_EMPTY;
		mCells[22][15].mState = lSTATE_EMPTY; mCells[22][18].mState = lSTATE_EMPTY;
		mCells[22][20].mState = lSTATE_EMPTY; mCells[22][23].mState = lSTATE_EMPTY;
		mCells[22][25].mState = lSTATE_EMPTY; mCells[22][30].mState = lSTATE_EMPTY;
		mCells[22][33].mState = lSTATE_EMPTY;

	// для 24-ой строки
		for(int i=0;i<35;i++) {
			mCells[23][i].mState = lSTATE_EMPTY;
		}
		mCells[23][0].mState = lSTATE_UNREACHABLE;  mCells[23][19].mState = lSTATE_UNREACHABLE;
		mCells[23][24].mState = lSTATE_UNREACHABLE; mCells[23][34].mState = lSTATE_UNREACHABLE;

	// для 25-ой строки
		for(int i=0;i<35;i++) {
			mCells[24][i].mState = lSTATE_UNREACHABLE;
		}
		mCells[24][1].mState = lSTATE_EMPTY;  mCells[24][7].mState = lSTATE_EMPTY;
		mCells[24][13].mState = lSTATE_EMPTY; mCells[24][16].mState = lSTATE_EMPTY;
		mCells[24][20].mState = lSTATE_EMPTY; mCells[24][25].mState = lSTATE_EMPTY;
		mCells[24][33].mState = lSTATE_EMPTY;

	// для 26-ой строки
		for(int i=0;i<35;i++) {
			mCells[25][i].mState = lSTATE_EMPTY;
		}
		mCells[25][0].mState = lSTATE_UNREACHABLE;
		mCells[25][34].mState = lSTATE_UNREACHABLE;

	// для 27-ой строки
		for(int i=0;i<35;i++) {
			mCells[26][i].mState = lSTATE_UNREACHABLE;
		}
	}
