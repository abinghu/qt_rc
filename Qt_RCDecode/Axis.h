// Axis.h: interface for the Axis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXIS_H__5BD0B0F2_5449_4455_BDC6_5E10C38A841E__INCLUDED_)
#define AFX_AXIS_H__5BD0B0F2_5449_4455_BDC6_5E10C38A841E__INCLUDED_

#include "Point.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Axis  
{
public:
	Axis(int myDECIMAL_POINT);
	Axis(int * angle, int TheModulePitch, int myDECIMAL_POINT);
	virtual ~Axis();
public:
	int DECIMAL_POINT;
	int sin, cos;
	int modulePitch;
	Point origin;
	void setOrigin(Point TheOrigin);
	void setModulePitch(int TheModulePitch);
	Point translate(int moveX, int moveY);
	Point translate(Point offset);
	Point translate(Point origin, Point offset);
	Point translate(Point origin, int moveX, int moveY);
	Point translate(Point origin, int modulePitch, int moveX, int moveY);
	
};

#endif // !defined(AFX_AXIS_H__5BD0B0F2_5449_4455_BDC6_5E10C38A841E__INCLUDED_)
