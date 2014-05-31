#ifndef _CANDY_HPP_
#define _CANDY_HPP_

#include "Types.hpp"
#include "Vector2D.hpp"

	class World;
	class WorldRenderer;

	/*
	 *   Candy - класс, объекты которого представляют собой конфетку, съев которую
	 *  появляется новая еда на противоположной стороне.
	 */

	class Candy {
	public:
		Candy();
		Candy(float pX, float pY, int32_t pState);
		void update(float pDeltaTime);
		void setInfo(float pX, float pY, int32_t pState);

		friend class World;
		friend class WorldRenderer;

	private:
		Vector2D mCoords;
		float mStateTime;
		int32_t mState;
		float mAngle;
	public:
		static const float WIDTH = 1.0f;
		static const float HEIGHT = 1.0f;
		static const int32_t STATE_EXIST = 0;
		static const int32_t STATE_NONE = 1;
		static const float ANIM_ANGLE_VELOCITY = 70.0f;
		static const int32_t SCORE_INCREMENT = 1000;
	};

#endif
