// Axis.cpp: implementation of the Axis class.
//
// This class designed to move target point based on independent axis.
// It allows move target coordinate on rotated, scaled and gauche QR Code image
//////////////////////////////////////////////////////////////////////

#include "Axis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Axis::Axis(int myDECIMAL_POINT)
{
	DECIMAL_POINT=myDECIMAL_POINT;
}

Axis::~Axis()
{

}

Axis::Axis(int * angle, int TheModulePitch, int myDECIMAL_POINT)
{
	DECIMAL_POINT=myDECIMAL_POINT;
	sin = angle[0];
	cos = angle[1];
	modulePitch = TheModulePitch;
	//origin = new Point();
}

void Axis::setOrigin(Point TheOrigin)
{
	origin = TheOrigin;
}
	
	
void Axis::setModulePitch(int TheModulePitch)
{
	modulePitch = TheModulePitch;
}
	
Point Axis::translate(int moveX, int moveY)
{
	long dp = DECIMAL_POINT;
	Point point;
  	int dx = (moveX == 0) ? 0 : (modulePitch * moveX) >> dp;
  	int dy = (moveY == 0) ? 0 : (modulePitch * moveY) >> dp;		
  	point.translate((dx * cos - dy * sin) >> dp, (dx * sin + dy * cos) >> dp);
  	point.translate(origin.x, origin.y);

	return point;
}
	
Point Axis::translate(Point offset)
{
	int moveX = offset.x;
	int moveY = offset.y;
	return translate(moveX, moveY);
}

Point Axis::translate(Point origin, Point offset)
{
	setOrigin(origin);
	int moveX = offset.x;
	int moveY = offset.y;
	return translate(moveX, moveY);
}

Point Axis::translate(Point origin, int moveX, int moveY)
{
	setOrigin(origin);
	return translate(moveX, moveY);
}

Point Axis::translate(Point origin, int modulePitch, int moveX, int moveY)
{
	setOrigin(origin);
	setModulePitch(modulePitch);
	return translate(moveX, moveY);
}
