#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "InputService.hpp"
#include "SoundService.hpp"
#include "Pacman.hpp"
#include "Star.hpp"
#include "Candy.hpp"
#include "CandyStrongMaker.hpp"
#include "Monster.hpp"
#include "Log.hpp"
#include "Cell.hpp"
#include "OverlapTester.hpp"
#include "Rectangle.hpp"
#include "Circle.hpp"
#include "Types.hpp"

	/*
	 *   World - класс, объекты которого представляют игровой мир, содержащий информацию
	 *  об игровых объектах(их месторасположении и т.д) и ответственные за обновление
	 *  состояния.
	 */

	class World {
	public:
	     World();
	     ~World();
	     void setInfo(InputService* pInputService,
					  SoundService* pSoundService);
	     void update(float pDeltaTime);			 // обновляем игровой мир
	     void refresh();						 // сбрасываем состояние мира в начальное

	private:
	     void updatePacman(float pDeltaTime);    // обновляем Pacman'a
	     void updateMonsters(float pDeltaTime);  // обновляем монстров
	     void updateStars(float pDeltaTime);	 // обновляем еду
	     void updateCandies(float pDeltaTime);	 // обновляем карамельки

	     void checkCollisions();				 // проверяем столкновения игровых объектов
	     void checkMonsterCollisions();			 // проверяем столкновение Pacman'a с монстром
	     void checkStarCollisions();			 // проверяем столкновение Pacman'a с едой
	     void checkCandyCollisions();			 // проверяем столкновение Pacman'a c карамелькой
	     void checkCandyStrongerCollisions();	 // проверяем столкновение Pacman'a c усилителем

	     void checkTimeIsUp();					 // проверяем кончилось ли время
	     void checkGameOver();					 // проверяем кончились ли жизни у Pacman'a
	     void checkTouchEvent();				 // в конце игры проверям нажал ли игрок на экран

	     void fillCells();						 // заполняем информацию о клетках лабиринта
	     void fillStarsInfo();					 // заполняем информацию о еде в лабиринте
	     void fillCandiesInfo();				 // заполняем информацию о карамельках в лабиринте
	     void fillMonstersInfo();				 // заполняем информацию о монстрах в лабиринте

	public:
	     // игровые объекты
	     Pacman mPacman;						 // Pacman
	     Star mStars[119];						 // Еда (119шт.)
	     Candy mCandies[2];						 // Карамельки (2шт.)
	     CandyStrongMaker mCandiesStrong[4];     // Усилители (4шт.)
	     Monster mMonsters[4];					 // Монстры (4шт.)
	     Cell mCells[27][35];					 // Игровые клетки (27x35шт)

	     float mTimeLeft;						 // прошедшее время с начала игры
	     float mStateTime;						 // прошедшее время с начала определенного состояния
	     int32_t mScore;						 // игровые очки
	     int32_t mState;						 // состояние
	     int32_t mPacmanLives;					 // кол-во оставшихся у Pacman'a жизней

	     InputService* mInputService;
	     SoundService* mSoundService;
	     OverlapTester mOverlapTester;

	public:
		 static const float WIDTH = 35.0f;
	     static const float HEIGHT = 27.0f;
	     static const int32_t STATE_RUNNING = 0;
	     static const int32_t STATE_GAMEOVER = 1;
	     static const int32_t STATE_TIMEISUP = 2;
	     static const int32_t STATE_SHOWENDSCORE = 3;
	     static const int32_t STATE_EXIT = 4;
	     static const int32_t STATE_WAITING = 5;
	     static const int32_t TIME_LEVEL = 90;
	     static const int32_t TIME_WAIT = 2;
	     static const int32_t PACMAN_LIVES = 3;
	     static const float TURN_ERROR = 0.15f;
	};

#endif
