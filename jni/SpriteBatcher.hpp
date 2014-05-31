#ifndef _SPRITEBATCHER_HPP_
#define _SPRITEBATCHER_HPP_

#include <cmath>
#include "TextureRegion.hpp"
#include "Log.hpp"

	/*
	 *    SpriteBatcher - класс, объект которого рисует игровые объекты на экране
	 */
	class SpriteBatcher {
	public:
												// Характеристики рисуемого объекта:

		void draw(float pX,						//  1) координата по X
				  float pY,						//  2) координата по Y
				  float pWidth, 				//  3) ширина
				  float pHeight, 				//  4) высота
				  TextureRegion* pTextReg);   	//  5) определение части отрисовываемой текстуры

		void draw(float pX, 					//  1) координата по X
				  float pY, 					//  2) координата по Y
				  float pWidth, 				//  3) ширина
				  float pHeight, 				//  4) высота
				  float pAngle, 				//  5) угол
				  TextureRegion* pTextReg);		//  6) определение части отрисовываемой текстуры

		void drawInverted(float pX,
						  float pY,
						  float pWidth,
						  float pHeight,
						  TextureRegion* pTextReg);

		void drawInverted(float pX,
						  float pY,
						  float pWidth,
						  float pHeight,
						  float pAngle,
						  TextureRegion* pTextReg);
	};

#endif
