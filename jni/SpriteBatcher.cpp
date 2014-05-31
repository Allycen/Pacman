#include "SpriteBatcher.hpp"

	void SpriteBatcher::draw(float pX,     float pY,
							 float pWidth, float pHeight,
							 TextureRegion* pTextReg) {


		float lHalfWidth = pWidth / 2;
		float lHalfHeight = pHeight / 2;

		float x1 = -lHalfWidth;
		float y1 = -lHalfHeight;
		float x2 =  lHalfWidth;
		float y2 = -lHalfHeight;
		float x3 =  lHalfWidth;
		float y3 =  lHalfHeight;
		float x4 = -lHalfWidth;
		float y4 =  lHalfHeight;

		 GLfloat lVertices[] = { x1, y1,    // (x,y) левая нижняя вершина
		    	            	 x2, y2,    // (x,y) правая нижняя вершина
		    	            	 x3, y3,    // (x,y) правая верхняя вершина
		    	            	 x4, y4 };  // (x,y) левая верхняя вершина

		 GLfloat lTexture[] =
					 { pTextReg->getV1()->getX(), pTextReg->getV1()->getY(),   // (x,y) левый нижний край текстуры
					   pTextReg->getV2()->getX(), pTextReg->getV1()->getY(),   // (х,y) правый нижний край текстуры
					   pTextReg->getV2()->getX(), pTextReg->getV2()->getY(),   // (x,y) правый верхний край текстуры
					   pTextReg->getV1()->getX(), pTextReg->getV2()->getY() }; // (x,y) левый верхний край текстуры


		 GLshort lIndices[] = { 0, 1, 2,
		    	    	    	0, 3, 2 };

		 GLfloat lColor[] = { 1.0f, 1.0f, 1.0f, 1.0f,
		    				  1.0f, 1.0f, 1.0f, 1.0f,
		    				  1.0f, 1.0f, 1.0f, 1.0f,
		    				  1.0f, 1.0f, 1.0f, 1.0f, };

		  //pTextReg->getTexture()->bind();

		  //glEnableClientState(GL_VERTEX_ARRAY);
		  //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		  //glEnableClientState(GL_COLOR_ARRAY);

		  glVertexPointer( 2, GL_FLOAT, 0, lVertices);
		  glTexCoordPointer(2, GL_FLOAT, 0, lTexture);
		  glColorPointer(4, GL_FLOAT, 0, lColor);

		  glTranslatef(pX, pY, 0);
		  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, lIndices);
		  glTranslatef(-pX, -pY, 0);

		  //glDisableClientState(GL_VERTEX_ARRAY);
		  //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		  //glDisableClientState(GL_COLOR_ARRAY);

		  // нужно ли удалять массивы lColor, lTexture, lVertices.. ?
		  // они ведь будут многократно создаваться при многократном
		  // вызове метода draw!
	}



	void SpriteBatcher::draw(float pX, 					//  1) координата по X
					         float pY, 					//  2) координата по Y
					         float pWidth, 				//  3) ширина
					         float pHeight, 		    //  4) высота
					         float pAngle, 				//  5) угол
					         TextureRegion* pTextReg)   //  6) определение части отрисовываемой текстуры
	{
		float lHalfWidth = pWidth / 2;
		float lHalfHeight = pHeight / 2;

		float cos = cosf(M_PI/180*-pAngle);
		float sin = sinf(M_PI/180*-pAngle);

		float x1 = -lHalfWidth * cos - (-lHalfHeight) * sin;
		float y1 = -lHalfWidth * sin + (-lHalfHeight) * cos;
		float x2 =  lHalfWidth * cos - (-lHalfHeight) * sin;
		float y2 =  lHalfWidth * sin + (-lHalfHeight) * cos;
		float x3 =  lHalfWidth * cos -   lHalfHeight  * sin;
		float y3 =  lHalfWidth * sin +   lHalfHeight  * cos;
		float x4 = -lHalfWidth * cos -   lHalfHeight  * sin;
		float y4 = -lHalfWidth * sin +   lHalfHeight  * cos;

		GLfloat lVertices[] = { x1, y1,    // (x,y) левая нижняя вершина
				    	        x2, y2,    // (x,y) правая нижняя вершина
				    	        x3, y3,    // (x,y) правая верхняя вершина
				    	        x4, y4 };  // (x,y) левая верхняя вершина

		GLfloat lTexture[] =
							 { pTextReg->getV1()->getX(), pTextReg->getV1()->getY(),   // (x,y) левый нижний край текстуры
							   pTextReg->getV2()->getX(), pTextReg->getV1()->getY(),   // (х,y) правый нижний край текстуры
							   pTextReg->getV2()->getX(), pTextReg->getV2()->getY(),   // (x,y) правый верхний край текстуры
							   pTextReg->getV1()->getX(), pTextReg->getV2()->getY() }; // (x,y) левый верхний край текстуры

		GLshort lIndices[] = { 0, 1, 2,
				    	       0, 3, 2 };

		GLfloat lColor[] = { 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f, };

		//pTextReg->getTexture()->bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer( 2, GL_FLOAT, 0, lVertices);
		glTexCoordPointer(2, GL_FLOAT, 0, lTexture);
		glColorPointer(4, GL_FLOAT, 0, lColor);

		glTranslatef(pX, pY, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, lIndices);
		glTranslatef(-pX, -pY, 0);

		//glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
	}

	void SpriteBatcher::drawInverted(float pX,
									 float pY,
									 float pWidth,
									 float pHeight,
									 TextureRegion* pTextReg) {

		float lHalfWidth = pWidth / 2;
		float lHalfHeight = pHeight / 2;

		float x1 = -lHalfWidth;
		float y1 = -lHalfHeight;
		float x2 =  lHalfWidth;
		float y2 = -lHalfHeight;
		float x3 =  lHalfWidth;
		float y3 =  lHalfHeight;
		float x4 = -lHalfWidth;
		float y4 =  lHalfHeight;

		GLfloat lVertices[] = { x1, y1,    // (x,y) левая нижняя вершина
				    	        x2, y2,    // (x,y) правая нижняя вершина
				    	        x3, y3,    // (x,y) правая верхняя вершина
				    	        x4, y4 };  // (x,y) левая верхняя вершина

		GLfloat lTexture[] =
							 { pTextReg->getV2()->getX(), pTextReg->getV1()->getY(),   // (x,y) правый нижний край текстуры
							   pTextReg->getV1()->getX(), pTextReg->getV1()->getY(),   // (х,y) левый нижний край текстуры
							   pTextReg->getV1()->getX(), pTextReg->getV2()->getY(),   // (x,y) левый верхний край текстуры
							   pTextReg->getV2()->getX(), pTextReg->getV2()->getY()};  // (x,y) правый верхний край текстуры


		GLshort lIndices[] = { 0, 1, 2,
				    	       0, 3, 2 };

		GLfloat lColor[] = { 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f, };

		//pTextReg->getTexture()->bind();

		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer( 2, GL_FLOAT, 0, lVertices);
		glTexCoordPointer(2, GL_FLOAT, 0, lTexture);
		glColorPointer(4, GL_FLOAT, 0, lColor);

		glTranslatef(pX, pY, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, lIndices);
		glTranslatef(-pX, -pY, 0);

		//glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
	}


	void SpriteBatcher::drawInverted(float pX,
								     float pY,
							         float pWidth,
							         float pHeight,
							         float pAngle,
							         TextureRegion* pTextReg) {

		float lHalfWidth = pWidth / 2;
		float lHalfHeight = pHeight / 2;

		float cos = cosf(M_PI/180*-pAngle);
		float sin = sinf(M_PI/180*-pAngle);

		float x1 = -lHalfWidth * cos - (-lHalfHeight) * sin;
		float y1 = -lHalfWidth * sin + (-lHalfHeight) * cos;
		float x2 =  lHalfWidth * cos - (-lHalfHeight) * sin;
		float y2 =  lHalfWidth * sin + (-lHalfHeight) * cos;
		float x3 =  lHalfWidth * cos -   lHalfHeight  * sin;
		float y3 =  lHalfWidth * sin +   lHalfHeight  * cos;
		float x4 = -lHalfWidth * cos -   lHalfHeight  * sin;
		float y4 = -lHalfWidth * sin +   lHalfHeight  * cos;

		GLfloat lVertices[] = { x1, y1,    // (x,y) левая нижняя вершина
				    	        x2, y2,    // (x,y) правая нижняя вершина
				    	        x3, y3,    // (x,y) правая верхняя вершина
				    	        x4, y4 };  // (x,y) левая верхняя вершина

		GLfloat lTexture[] =
							  { pTextReg->getV2()->getX(), pTextReg->getV1()->getY(),   // (x,y) правый нижний край текстуры
								pTextReg->getV1()->getX(), pTextReg->getV1()->getY(),   // (х,y) левый нижний край текстуры
								pTextReg->getV1()->getX(), pTextReg->getV2()->getY(),   // (x,y) левый верхний край текстуры
								pTextReg->getV2()->getX(), pTextReg->getV2()->getY()};  // (x,y) правый верхний край текстуры

		GLshort lIndices[] = { 0, 1, 2,
				    	       0, 3, 2 };

		GLfloat lColor[] = { 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f,
				    		 1.0f, 1.0f, 1.0f, 1.0f, };

		//pTextReg->getTexture()->bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer( 2, GL_FLOAT, 0, lVertices);
		glTexCoordPointer(2, GL_FLOAT, 0, lTexture);
		glColorPointer(4, GL_FLOAT, 0, lColor);

		glTranslatef(pX, pY, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, lIndices);
		glTranslatef(-pX, -pY, 0);

		//glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
	}




