#include "Vector2D.hpp"

	Vector2D::Vector2D() {
		mX = 0;
		mY = 0;
	}

	Vector2D::Vector2D(float pX, float pY) {
		mX = pX;
		mY = pY;
	}

	float Vector2D::getX() {
		return mX;
	}

	float Vector2D::getY() {
		return mY;
	}

	void Vector2D::set(float pX, float pY) {
		mX = pX;
		mY = pY;
	}

	void Vector2D::set(Vector2D* pV) {
		mX = pV->getX();
		mY = pV->getY();
	}

	void Vector2D::add(float pX, float pY) {
		mX += pX;
		mY += pY;
	}

	void Vector2D::add(Vector2D* pV) {
		mX += pV->getX();
		mY += pV->getY();
	}

	void Vector2D::sub(float pX, float pY) {
		mX -= pX;
		mY -= pY;
	}

	void Vector2D::sub(Vector2D* pV) {
		mX -= pV->getX();
		mY -= pV->getY();
	}

	float Vector2D::distSquared(float pX, float pY) {
		float distX = mX - pX;
		float distY = mY - pY;
		return distX*distX + distY*distY;
	}

	float Vector2D::distSquared(Vector2D* pV) {
		float distX = mX - pV->getX();
		float distY = mY - pV->getY();
		return distX*distX + distY*distY;
	}

	void Vector2D::mul(float scalar) {
		mX *= scalar;
		mY *= scalar;
	}






