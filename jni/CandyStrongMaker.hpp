#ifndef _CANDYSTRONGMAKER_HPP_
#define _CANDYSTRONGMAKER_HPP_

#include "Types.hpp"
#include "Vector2D.hpp"

	class World;
	class WorldRenderer;


	/*
	 *   CandyStrongMaker - класс, объекты которого представляют собой конфетку,
	 *   съев которую все монстры начинают бояться Pacman'a.
	 */

	class CandyStrongMaker {
	public:
		CandyStrongMaker();
		CandyStrongMaker(float pX, float pY, int32_t pState);
		void update(float pDeltaTime);
		void setInfo(float pX, float pY, int32_t pState);

		friend class World;
		friend class WorldRenderer;

	private:
		Vector2D mCoords;
		float mStateTime;
		int32_t mState;
		float mOffset;
		bool mDirectionUp;
	public:
		static const float WIDTH = 0.6f;
		static const float HEIGHT = 1.2f;
		static const int32_t STATE_EXIST = 0;
		static const int32_t STATE_NONE = 1;
		static const float ANIM_VELOCITY = 0.7f;
		static const int32_t SCORE_INCREMENT = 1000;
	};


#endif
