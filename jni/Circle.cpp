#include "Circle.hpp"

	Circle::Circle(float pX, float pY, float pRadius) {
		mVCenter.set(pX, pY);
		mRadius = pRadius;
	}

	float Circle::getX() {
		return mVCenter.getX();
	}

	float Circle::getY() {
		//return mVCenter->getY();
		return mVCenter.getY();
	}

	float Circle::getRadius() {
		return mRadius;
	}

	Vector2D* Circle::getV() {
		return &mVCenter;
	}

	void Circle::setInfo(float pX, float pY, float pRadius) {
		mVCenter.set(pX, pY);
		mRadius = pRadius;
	}
