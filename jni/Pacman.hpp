#ifndef _PACMAN_HPP_
#define _PACMAN_HPP_

#include "Vector2D.hpp"
#include "Types.hpp"

	class World;
	class WorldRenderer;

	/*
	 *   Pacman - класс, объект которого представляет собой Pacman'a, бегающего
	 *  по лабиринту от монстров и собирающего еду.
	 */
	class Pacman {
	public:
		Pacman(float pX, 				// координата X
			   float pY, 				// координата Y
			   int32_t pState, 			// состояние
			   int32_t pDirection);		// направление движения
		void update(float pDeltaTime);	// обновление

		friend class World;
		friend class WorldRenderer;

	private:
		void updateAnimFrame();			// обновление кадра тектуры

	private:
		Vector2D mCoords;				// координаты
		float mStateTime;				// время нахождения в состоянии
		int32_t mDirection,				// направение движения
				mPreviousDirection;		// прошлое направление движения
		int32_t mState;					// состояние
		int32_t mAnimFrame;				// текущий кадр тектуры

	public:
		static const int32_t STATE_STAY = 0;
		static const int32_t STATE_RUN = 1;
		static const int32_t STATE_HIT = 2;
		static const float WIDTH = 1.3f;
		static const float HEIGHT = 1.3f;
		static const float MOVE_VELOCITY = 4.5f;
		static const float ANIM_VELOCITY = 2.0f;
		static const int32_t DIRECTION_UP = 0;
		static const int32_t DIRECTION_RIGHT = 1;
		static const int32_t DIRECTION_DOWN = 2;
		static const int32_t DIRECTION_LEFT = 3;
	};


#endif
