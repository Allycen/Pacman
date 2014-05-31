#ifndef _VECTOR2D_HPP_
#define _VECTOR2D_HPP_


	/*
	 *  Реализация двухмерного вектора
	 */

	class Vector2D {
	public:
		Vector2D();
		Vector2D(float pX, float pY);


		void set(float pX, float pY);  		   // Устанавливаем
		void set(Vector2D* pV);	       		   // координаты вектору

		float getX();				   		   // Получаем
		float getY();				   		   // координаты вектора

		void add(float pX, float pY);  		   // Прибавляем к вектору
		void add(Vector2D* pV);  	  	       // от слова "add"


		void sub(float pX, float pY);  		   // Вычитаем из вектора
		void sub(Vector2D* pV);		   		   // от слова "subtraction"

		float distSquared(float pX, float pY); // Расстояние между
		float distSquared(Vector2D* pV);	   // векторами в квадрате

		void mul(float scalar);		   		   // Умножаем вектор на скаляр
											   // от слова "multiplication"



	private:
		float mX,  // координата X
			  mY;  // координата Y
	};

#endif
