#include "CandyStrongMaker.hpp"

	CandyStrongMaker::CandyStrongMaker()
	: mCoords(0,0)
	{
		mState = STATE_NONE;
		mStateTime = 0.0f;
		mOffset = 0;
		mDirectionUp = true;
	}

	CandyStrongMaker::CandyStrongMaker(float pX, float pY, int32_t pState)
	: mCoords(pX, pY)
	{
		mState = pState;
		mStateTime = 0.0f;
		mOffset = 0;
		mDirectionUp = true;
	}

	void CandyStrongMaker::update(float pDeltaTime) {
		if(mState == STATE_EXIST) {
			float lMAX_OFFSET = 0.3f;
			if(mDirectionUp) {
				mOffset += ANIM_VELOCITY * pDeltaTime;
			} else {
				mOffset -= ANIM_VELOCITY * pDeltaTime;
			}

			if(mOffset > lMAX_OFFSET) {
				mDirectionUp = false;
			} else {
				if(mOffset < -lMAX_OFFSET) {
					mDirectionUp = true;
				}
			}
		}
		mStateTime += pDeltaTime;
	}


	void CandyStrongMaker::setInfo(float pX, float pY, int32_t pState) {
		mCoords.set(pX, pY);
		mState = pState;
		mStateTime = 0.0f;
	}
