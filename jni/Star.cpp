#include "Star.hpp"

	Star::Star(float pX, float pY, int32_t pState)
	: mCoords(pX, pY)
	{
		mState = pState;
		mAnimFrame = 0;
		mStateTime = 0.0f;
	}

	Star::Star()
	: mCoords(0,0)
	{
		mAnimFrame = 0;
		mState = STATE_NONE;
		mStateTime = 0.0f;
	}

	void Star::setInfo(float pX, float pY, int32_t pState) {
		mCoords.set(pX, pY);
		mState = pState;
	}

	void Star::update(float pDeltaTime) {
		// проверяем закончилась ли создаваться звезда
		if (mState == STATE_INIT) {
			if(mStateTime*ANIM_INIT_VELOCITY > 1) {
				mState = STATE_EXIST;
			}
		}

		updateAnimFrame();
		mStateTime += pDeltaTime;
	}

	void Star::updateAnimFrame() {
		if(mState == STATE_INIT) {
			if(mStateTime*ANIM_INIT_VELOCITY < 0.25) {
				mAnimFrame = 0; // state1
			}
			if(mStateTime*ANIM_INIT_VELOCITY >= 0.25 && mStateTime*ANIM_INIT_VELOCITY <0.5) {
				mAnimFrame = 1; // state2
			}
			if(mStateTime*ANIM_INIT_VELOCITY >= 0.5 && mStateTime*ANIM_INIT_VELOCITY <0.75) {
				mAnimFrame = 2; // state 3
			}
			if(mStateTime*ANIM_INIT_VELOCITY >= 0.75 && mStateTime*ANIM_INIT_VELOCITY <1) {
				mAnimFrame = 3; // state 4
			}
		}

		if(mState == STATE_EXIST) {
			if(mStateTime*ANIM_EXIST_VELOCITY< 0.125) {
				mAnimFrame = 4; // state 4
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 0.125 && mStateTime*ANIM_EXIST_VELOCITY <0.25) {
				mAnimFrame = 5; // 10grad
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 0.25 && mStateTime*ANIM_EXIST_VELOCITY <0.375) {
				mAnimFrame = 6; // 20grad
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 0.375 && mStateTime*ANIM_EXIST_VELOCITY <0.5) {
				mAnimFrame = 5; // 10grad
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 0.5 && mStateTime*ANIM_EXIST_VELOCITY <0.625) {
				mAnimFrame = 4; // state 4
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 0.625 && mStateTime*ANIM_EXIST_VELOCITY <0.75) {
				mAnimFrame = 7; // minus10grad
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 0.75 && mStateTime*ANIM_EXIST_VELOCITY <0.875) {
				mAnimFrame = 8; // minus20grad
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 0.875 && mStateTime*ANIM_EXIST_VELOCITY <1) {
				mAnimFrame = 7; // minus10grad
			}
			if(mStateTime*ANIM_EXIST_VELOCITY >= 1) {
				mStateTime = 0.0f;
			}
		}


	}
