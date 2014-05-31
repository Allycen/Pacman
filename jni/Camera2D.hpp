#ifndef _CAMERA2D_HPP_
#define _CAMERA2D_HPP_

#include "Vector2D.hpp"

	/*
	 *   Camera2D - класс, объекты которого устанавливают ракурс и видовое
	 *  представление.
	 */

	class Camera2D {
	public:
		Camera2D(float pFrustrumWidth, float pFrustrumHeight);
		void setViewportAndMatrices();

		void setFrustumResolution(float pFrustrumWidth, float pFrustrumHeight);
		void setCoords(float pX, float pY);
	private:
		Vector2D mPosition;

	    float mFrustumWidth,
			  mFrustumHeight,
			  mZoom;
	};

#endif
