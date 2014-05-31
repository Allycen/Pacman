#ifndef _OVERLAPTESTER_HPP_
#define _OVERLAPTESTER_HPP_

#include "Vector2D.hpp"
#include "Rectangle.hpp"
#include "Circle.hpp"
#include "Log.hpp"

	/*
	 * 	 OverlapTester - класс, объекты которого проверяют пересечение
	 *  игровых объектов
	 */
	class OverlapTester {
	public:
		bool overlapCircles(Circle* pC1, Circle* pC2);				// пересекаются ли круги
		bool overlapRectangles(Rectangle* pR1, Rectangle* pR2);		// пересекаются ли прямоугольники
		bool overlapCircleRectangle(Circle* pC, Rectangle* pR);		// пересекаются ли круг с прямоугольником

		bool pointInCircle(Circle* pC, float pX, float pY);			// принадлежит ли точка кругу
		bool pointInCircle(Circle* pC, Vector2D* pV);				// -//-/-

		bool pointInRectangle(Rectangle* pR, float pX, float pY);   // принадлежит ли точка прямоугольнику
		bool pointInRectangle(Rectangle* pR, Vector2D* pV);			// -//-/-
	};

#endif

	// Функции этого класса вернут true если объекты касаются друг друга гранями.
	// Если точка лежит на границе фигуры, считается, что точка принадлежит фигуре.
