// Line.h: interface for the Line class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__164AB76B_0E6A_448E_8B7E_F43E66088DA4__INCLUDED_)
#define AFX_LINE_H__164AB76B_0E6A_448E_8B7E_F43E66088DA4__INCLUDED_

#include "Point.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Line  
{
public:
	Line();
	virtual ~Line();

public:
	int x1;
	int y1;
	int x2;
	int y2;

	Line(int a1, int b1, int a2, int b2);
	Line(Point p1, Point p2);
	Point getP1();
	Point getP2();
	void setLine(int x1, int y1, int x2, int y2);
	void setLine(Point p1, Point p2);
	void setP1(Point p1);
	void setP1(int a1, int b1);
	void setP2(Point p2);
	void setP2(int a2, int b2);
	void translate(int dx, int dy);
	bool isNeighbor(Line line1, Line line2);
	bool isHorizontal();
	bool isVertical();
	bool isCross(Line line1, Line line2);
	Point getCenter();
	int getLength();
	Line getLongest(Line * lines,int length);
	//CString toString();
};

#endif // !defined(AFX_LINE_H__164AB76B_0E6A_448E_8B7E_F43E66088DA4__INCLUDED_)
