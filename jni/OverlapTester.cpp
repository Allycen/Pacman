#include "OverlapTester.hpp"


	bool OverlapTester::overlapCircles(Circle* pC1, Circle* pC2) {
		float distance = pC1->getV()->distSquared(pC2->getV());
		float radiusSum = pC1->getRadius() + pC2->getRadius();

		return(distance <= radiusSum*radiusSum);
	}

	bool OverlapTester::overlapRectangles(Rectangle* pR1, Rectangle* pR2) {
		float p1X = (*pR1).getX()-(*pR1).getWidth()/2;
		float p1Y = (*pR1).getY()-(*pR1).getHeight()/2;
		float p2X = (*pR2).getX()-(*pR2).getWidth()/2;
		float p2Y = (*pR2).getY()-(*pR2).getHeight()/2;

		if(	p1X <= p2X + (*pR2).getWidth() &&
			p1Y <= p2Y + (*pR2).getHeight() &&
			p1X + (*pR1).getWidth() >=  p2X &&
			p1Y + (*pR1).getHeight() >= p2Y)
			return true;
		else
			return false;
	}

	bool OverlapTester::overlapCircleRectangle(Circle* pC, Rectangle* pR) {
		float lClosestX = (*pC).getX();
		float lClosestY = (*pC).getY();
		float pR_X = (*pR).getX() - (*pR).getWidth()/2;
		float pR_Y = (*pR).getY() - (*pR).getHeight()/2;

		if((*pC).getX() < pR_X) {
			lClosestX = pR_X;
		}
		else if((*pC).getX() > pR_X+ (*pR).getWidth()) {
			lClosestX = pR_X + (*pR).getWidth();
		}
		if((*pC).getY() < pR_Y) {
			lClosestY = pR_Y;
		}
		else if((*pC).getY() > pR_Y + (*pR).getHeight()) {
			lClosestY = pR_Y + (*pR).getHeight();
		}

		return ((*pC).getV()->distSquared(lClosestX, lClosestY) <=
				(*pC).getRadius()*(*pC).getRadius());
	}

	bool OverlapTester::pointInCircle(Circle *pC, float pX, float pY) {
		return (*pC).getV()->distSquared(pX, pY) <= (*pC).getRadius() * (*pC).getRadius();
	}

	bool OverlapTester::pointInCircle(Circle* pC, Vector2D* pV) {
		return (*pC).getV()->distSquared(pV) <= (*pC).getRadius() * (*pC).getRadius();
	}

	bool OverlapTester::pointInRectangle(Rectangle* pR, float pX, float pY) {
		return (*pR).getX() - (*pR).getWidth()/2 <= pX &&
			   (*pR).getX() + (*pR).getWidth()/2 >= pX &&
			   (*pR).getY() - (*pR).getHeight()/2 <= pY &&
			   (*pR).getY() + (*pR).getHeight()/2 >= pY;
	}

	bool OverlapTester::pointInRectangle(Rectangle* pR, Vector2D* pV) {
		return (*pR).getX() - (*pR).getWidth()/2 <= (*pV).getX() &&
			   (*pR).getX() + (*pR).getWidth()/2 >= (*pV).getX() &&
			   (*pR).getY() - (*pR).getHeight()/2 <= (*pV).getY() &&
			   (*pR).getY() + (*pR).getHeight()/2 >= (*pV).getY();
	}
