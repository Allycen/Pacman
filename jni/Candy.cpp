#include "Candy.hpp"

	Candy::Candy()
	: mCoords(0,0)
	{
		mState = STATE_NONE;
		mStateTime = 0.0f;
		mAngle = 0;
	}

	Candy::Candy(float pX, float pY, int32_t pState)
	: mCoords(pX, pY)
	{
		mState = pState;
		mStateTime = 0.0f;
	}

	void Candy::update(float pDeltaTime) {
		if(mState == STATE_EXIST) {
			mAngle += ANIM_ANGLE_VELOCITY * pDeltaTime;
		}
		mStateTime += pDeltaTime;
	}

	void Candy::setInfo(float pX, float pY, int32_t pState) {
		mCoords.set(pX, pY);
		mState = pState;
		mStateTime = 0.0f;
	}
