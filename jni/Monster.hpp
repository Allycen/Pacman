#ifndef _MONSTER_HPP_
#define _MONSTER_HPP_

#include "Vector2D.hpp"
#include "Types.hpp"

	class World;
	class WorldRenderer;

	/*
	 *   Monster - класс, объекты которого представляют монстра, который
	 *  гонится за Pacman'ом
	 */

	class Monster {
	public:
		Monster(float pX,					// координата X
				float pY,					// координата Y
				int32_t pState,				// состояние(страх, преследование..)
				int32_t pPlace,				// положение(в домике, в лабиринте..)
				int32_t pType,				// тип(красный, коричневый..)
				int32_t pDirection);		// направление движения
		Monster();

		void setInfo(float pX,				// координата X
				float pY,					// координата Y
				int32_t pState,				// состояние(страх, преследование..)
				int32_t pPlace,				// положение(в домике, в лабиринте..)
				int32_t pType,				// тип(красный, коричневый..)
				int32_t pDirection);		// направление движения

		void update(float pDeltaTime);		// обновляем информацию о монстре
		friend class World;
		friend class WorldRenderer;
	private:
		void updateAnimFrame();				// обновление текущего кадра текстуры


	private:
		Vector2D mCoords;					// координаты
		float mStateTime,					// время нахождения в данном состоянии
		      mPlaceTime;					// время нахождения в данном положении
		bool mPacmanIsNear,					// находится ли близко Pacman
			 mTimeToKill;					// заканчивается ли время страха
		float mAnimTime;					//
		int32_t mDirection,					// текущее направление
				mPreviousDirection;			// прошлое направление
		int32_t mState, 					// состояние (страх, преследование..)
				mPlace;						// положение (в домике, в лабиринте..)
		int32_t mType;						// тип (красный, коричневый..)
		int32_t mAnimFrame;					// текущий кадр текстуры
		float mDeltaTimeChangeDirections;	// время между сменами направлений

	public:
		static const int32_t STATE_CHASE = 0;
		static const int32_t STATE_SCATTER = 1;
		static const int32_t STATE_FRIGHTENED = 2;
		static const int32_t STATE_EATEN = 3;
		static const int32_t PLACE_ATHOME = 0;
		static const int32_t PLACE_GETTINGFROMHOME = 1;
		static const int32_t PLACE_INLABYRINTH = 2;
		static const float WIDTH = 1.3f;
		static const float HEIGHT = 1.3f;
		static const float MOVE_NORMAL_VELOCITY = 3.5f;
		static const float MOVE_FRIGHTENED_VELOCITY = 1.2f;
		static const float MOVE_EATEN_VELOCITY = 3.0f;
		static const float ANIM_NORMAL_VELOCITY = 1.5f;
		static const float ANIM_FRIGHTENED_VELOCITY = 0.5f;
		static const int32_t DIRECTION_UP = 0;
		static const int32_t DIRECTION_RIGHT = 1;
		static const int32_t DIRECTION_DOWN = 2;
		static const int32_t DIRECTION_LEFT = 3;
		static const int32_t TYPE_RED = 0;
		static const int32_t TYPE_PINK = 1;
		static const int32_t TYPE_CYAN = 2;
		static const int32_t TYPE_BROWN = 3;
		static const int32_t TIME_CHASE = 15.0f;
		static const int32_t TIME_SCATTER = 10.0f;
		static const int32_t TIME_FRIGHTENED = 10.0f;
		static const int32_t TIME_EATEN = 4.0f;
		static const int32_t SCORE_INCREMENT = 500;
	};

#endif
