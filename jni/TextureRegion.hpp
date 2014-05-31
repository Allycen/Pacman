#ifndef _TEXTUREREGION_HPP_
#define _TEXTUREREGION_HPP_

#include "Texture.hpp"
#include "Vector2D.hpp"

	/*
	 *    Изображение может содержать в себе множество текстур к различным объектам.
	 *   Класс Texture ответственен за загрузку изображения, а TextureRegion(участок текстуры)
	 *   за определение из изображения отдельных текстурных участков (текстур для определенных объектов)
	 */

	class TextureRegion {
	public:
		TextureRegion(Texture* pTexture,	// тектура по которой будет сформирован участок
					  float pX, 			// координата X центра будущего участка
					  float pY, 			// координата Y центра будущего участка
					  float pWidth, 		// ширина участка
					  float pHeight); 		// высота участка

		Vector2D* getV1();
		Vector2D* getV2();
		Texture* getTexture();

	private:
		Vector2D *mV1, // координата левого нижнего угла участка
				 *mV2; // координата правого верхнего угла участка
				       // (относительно координат текстуры!
	    Texture* mTexture; // текстура откуда берется участок текстуры
	};

#endif
