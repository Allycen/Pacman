#include "TextureRegion.hpp"

	TextureRegion::TextureRegion(Texture* pTexture, float pX, float pY, float pWidth, float pHeight) {
		mTexture = pTexture;
		Vector2D* lV1 = new Vector2D(pX / (*pTexture).getWidth(),
									 pY / (*pTexture).getHeight());
		Vector2D* lV2 = new Vector2D((*lV1).getX() + pWidth / (*pTexture).getWidth(),
									 (*lV1).getY() + pHeight / (*pTexture).getHeight());
		mV1 = lV1;
		mV2 = lV2;
	}

	Vector2D* TextureRegion::getV1() {
		return mV1;
	}

	Vector2D* TextureRegion::getV2() {
		return mV2;
	}

	Texture* TextureRegion::getTexture() {
		return mTexture;
	}
