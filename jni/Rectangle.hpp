#ifndef _RECTANGLE_HPP_
#define _RECTANGLE_HPP_

#include "Vector2D.hpp"

	class Rectangle {
	public:
		Rectangle(float pX, float pY, float pWidth, float pHeight);
		Rectangle();

		float getX();
		float getY();
		float getWidth();
		float getHeight();

		void setInfo(float pX, float pY, float pWidth, float pHeight);

	private:
		Vector2D mVCenter;    // координаты
		float mWidth;  		  // ширина
		float mHeight; 		  // высота
	};

#endif
