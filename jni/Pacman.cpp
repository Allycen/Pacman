#include "Pacman.hpp"

	Pacman::Pacman(float pX, float pY, int32_t pState, int32_t pDirection)
	: mCoords(pX, pY)
	{
		mState = pState;
		mDirection = pDirection;
		mPreviousDirection = pDirection;
		mAnimFrame = 0;
	}

	void Pacman::update(float pDeltaTime) {

		if(mState == STATE_RUN) {
			switch(mDirection) {
				case DIRECTION_UP:
					mCoords.add(0, MOVE_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_DOWN:
					mCoords.add(0, -MOVE_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_LEFT:
					mCoords.add(-MOVE_VELOCITY*pDeltaTime, 0);
					break;
			    case DIRECTION_RIGHT:
				    mCoords.add(MOVE_VELOCITY*pDeltaTime, 0);
				    break;
			}
		}

		updateAnimFrame();
		mStateTime += pDeltaTime;
	}

	void Pacman::updateAnimFrame() {
		if(mState == STATE_RUN) {
			if(mStateTime*ANIM_VELOCITY < 0.25) {
				mAnimFrame = 0;
			}
			if(mStateTime*ANIM_VELOCITY >= 0.25 && mStateTime*ANIM_VELOCITY < 0.5) {
				mAnimFrame = 1;
			}
			if(mStateTime*ANIM_VELOCITY >= 0.5 && mStateTime*ANIM_VELOCITY < 0.75) {
				mAnimFrame = 2;
			}
			if(mStateTime*ANIM_VELOCITY >= 0.75 && mStateTime*ANIM_VELOCITY < 1) {
				mAnimFrame = 3;
			}
			if(mStateTime*ANIM_VELOCITY > 1) {
				mStateTime = 0;
			}
		}

	}
