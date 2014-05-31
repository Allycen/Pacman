#ifndef _STAR_HPP_
#define _STAR_HPP_

#include "Vector2D.hpp"
#include "Types.hpp"

	class World;
	class WorldRenderer;

	/*
	 *   Star - класс, объекты которого представляют собой еду, которую ест Pacman
	 */
	class Star {
	public:
		Star(float pX, float pY, int32_t pState);
		Star();
		void update(float pDeltaTime);
		void setInfo(float pX, float pY, int32_t pState);

		friend class World;
		friend class WorldRenderer;
	private:
		void updateAnimFrame();

	private:
		Vector2D mCoords;
		float mStateTime;
		int32_t mState;
		int32_t mAnimFrame;

	public:
		static const float WIDTH = 0.8f;
		static const float HEIGHT = 0.8f;
		static const int32_t STATE_INIT = 0;
		static const int32_t STATE_EXIST = 1;
		static const int32_t STATE_NONE = 2;
		static const float ANIM_EXIST_VELOCITY = 1.0f;
		static const float ANIM_INIT_VELOCITY = 1.0f;
		static const int32_t SCORE_INCREMENT = 10;
	};

#endif
