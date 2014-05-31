#include "Monster.hpp"

	Monster::Monster(float pX, float pY, int32_t pState, int32_t pPlace, int32_t pType, int32_t pDirection)
	: mCoords(pX, pY)
	{
		mState = pState;
		mPlace = pPlace;
		mType = pType;
		mStateTime = 0.0f;
		mPlaceTime = 0.0f;
		mAnimTime = 0.0f;
		mAnimFrame = 0;
		mDirection = pDirection;
		mPacmanIsNear = false;
		mTimeToKill = false;
	}

	Monster::Monster()
	: mCoords(0,0)
	{ }

	void Monster::setInfo(float pX, float pY, int32_t pState, int32_t pPlace, int32_t pType, int32_t pDirection) {
		mCoords.set(pX, pY);
		mState = pState;
		mPlace = pPlace;
		mType = pType;
		mStateTime = 0.0f;
		mPlaceTime = 0.0f;
		mAnimFrame = 0;
		mDirection = pDirection;
		mPacmanIsNear = false;
		mTimeToKill = false;
	}

	void Monster::update(float pDeltaTime) {
		if(mState != STATE_FRIGHTENED) {
			switch(mDirection) {
				case DIRECTION_UP:
					mCoords.add(0, MOVE_NORMAL_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_DOWN:
					mCoords.add(0, -MOVE_NORMAL_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_LEFT:
					mCoords.add(-MOVE_NORMAL_VELOCITY*pDeltaTime, 0);
					break;
				case DIRECTION_RIGHT:
					mCoords.add(MOVE_NORMAL_VELOCITY*pDeltaTime, 0);
					break;
			}
		} else {
			switch(mDirection) {
				case DIRECTION_UP:
					mCoords.add(0, MOVE_FRIGHTENED_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_DOWN:
					mCoords.add(0, -MOVE_FRIGHTENED_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_LEFT:
					mCoords.add(-MOVE_FRIGHTENED_VELOCITY*pDeltaTime, 0);
					break;
				case DIRECTION_RIGHT:
					mCoords.add(MOVE_FRIGHTENED_VELOCITY*pDeltaTime, 0);
					break;
			}
		}

		if(mState == STATE_EATEN) {
			switch(mDirection) {
				case DIRECTION_UP:
					mCoords.add(0, MOVE_EATEN_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_DOWN:
					mCoords.add(0, -MOVE_EATEN_VELOCITY*pDeltaTime);
					break;
				case DIRECTION_LEFT:
					mCoords.add(-MOVE_EATEN_VELOCITY*pDeltaTime, 0);
					break;
				case DIRECTION_RIGHT:
					mCoords.add(MOVE_EATEN_VELOCITY*pDeltaTime, 0);
					break;
			}
		}

		updateAnimFrame();
		mStateTime += pDeltaTime;
		mPlaceTime += pDeltaTime;
		mAnimTime += pDeltaTime;
	}

	void Monster::updateAnimFrame() {

		if(mState != STATE_FRIGHTENED) {
			if(mAnimTime*ANIM_NORMAL_VELOCITY < 0.25) {
				mAnimFrame = 0;
			}
			if(mAnimTime*ANIM_NORMAL_VELOCITY >= 0.25 && mAnimTime*ANIM_NORMAL_VELOCITY < 0.5) {
				mAnimFrame = 1;
			}
			if(mAnimTime*ANIM_NORMAL_VELOCITY >= 0.5 && mAnimTime*ANIM_NORMAL_VELOCITY < 0.75) {
				mAnimFrame = 0;
			}
			if(mAnimTime*ANIM_NORMAL_VELOCITY >= 0.75 && mAnimTime*ANIM_NORMAL_VELOCITY < 1) {
				mAnimFrame = 2;
			}
			if(mAnimTime*ANIM_NORMAL_VELOCITY >= 1) {
				mAnimTime = 0;
			}
		} else {
			if(mAnimTime*ANIM_FRIGHTENED_VELOCITY < 0.25) {
				mAnimFrame = 3;
			}
			if(mAnimTime*ANIM_FRIGHTENED_VELOCITY >= 0.25 && mAnimTime*ANIM_FRIGHTENED_VELOCITY < 0.5) {
				mAnimFrame = 4;
			}
			if(mAnimTime*ANIM_FRIGHTENED_VELOCITY >= 0.5 && mAnimTime*ANIM_FRIGHTENED_VELOCITY < 0.75) {
				mAnimFrame = 3;
			}
			if(mAnimTime*ANIM_FRIGHTENED_VELOCITY >= 0.75 && mAnimTime*ANIM_FRIGHTENED_VELOCITY < 1) {
				mAnimFrame = 5;
			}
			if(mAnimTime*ANIM_FRIGHTENED_VELOCITY >= 1) {
				mAnimTime = 0;
			}
		}
		if(mState == STATE_EATEN) {
			mAnimFrame = 6;
		}
	}
