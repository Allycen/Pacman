#ifndef _CIRCLE_HPP_
#define _CIRCLE_HPP_

#include "Vector2D.hpp"

	class Circle {
	public:
		Circle(float pX, float pY, float pRadius);

		float getX();
		float getY();
		float getRadius();
		Vector2D* getV();

		void setInfo(float pX, float pY, float pRadius);

	private:
		Vector2D mVCenter;   // координаты
		float mRadius;  	 // радиус
	};

#endif
