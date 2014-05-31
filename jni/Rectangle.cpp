#include "Rectangle.hpp"

	Rectangle::Rectangle(float pX, float pY, float pWidth, float pHeight) {
		mVCenter.set(pX, pY);
		mWidth = pWidth;
		mHeight = pHeight;
	}

	Rectangle::Rectangle() {
		mVCenter.set(0, 0);
		mWidth = 0;
		mHeight = 0;
	}

	float Rectangle::getX() {
		return mVCenter.getX();
	}

	float Rectangle::getY() {
		mVCenter.getY();
	}

	float Rectangle::getHeight() {
		return mHeight;
	}

	float Rectangle::getWidth() {
		return mWidth;
	}

	void Rectangle::setInfo(float pX, float pY, float pWidth, float pHeight) {
		mVCenter.set(pX, pY);
		mWidth = pWidth;
		mHeight = pHeight;
	}
