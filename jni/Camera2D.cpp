#include "Camera2D.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>

	Camera2D::Camera2D(float pFrustrumWidth, float pFrustrumHeight) {
		mFrustumWidth = pFrustrumWidth;
		mFrustumHeight = pFrustrumHeight;
		mZoom = 1.0f;
		mPosition.set(pFrustrumWidth/2, pFrustrumHeight/2);
	}

	void Camera2D::setViewportAndMatrices() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(mPosition.getX()-mFrustumWidth/2,  // левая сторона
				 mPosition.getX()+mFrustumWidth/2,  // правая сторона
				 mPosition.getY()-mFrustumHeight/2, // нижняя сторона
				 mPosition.getY()+mFrustumHeight/2, // верхняя сторона
				 1, 0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void Camera2D::setFrustumResolution(float pFrustrumWidth, float pFrustrumHeight) {
		mFrustumWidth = pFrustrumWidth;
		mFrustumHeight = pFrustrumHeight;
	}

	void Camera2D::setCoords(float pX, float pY) {
		mPosition.set(pX,pY);
	}
