// Line.cpp: implementation of the Line class.
//
//////////////////////////////////////////////////////////////////////

#include "Line.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include<math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Line::Line()
{
	x1 = y1 = x2 = y2 = 0;
}

Line::~Line()
{

}

Line::Line(int a1, int b1, int a2, int b2)
{
	x1 = a1;
	y1 = b1;
	x2 = a2;
	y2 = b2;
}

Line::Line(Point p1, Point p2)
{
	x1 = p1.x;
	y1 = p1.y;
	x2 = p2.x;
	y2 = p2.y;
}

Point Line::getP1()
{
	return Point(x1, y1);
}
Point Line::getP2()
{
	return Point(x2, y2);
}

void Line::setLine(int a1, int b1, int a2, int b2)
{
	x1 = a1;
	y1 = b1;
	x2 = a2;
	y2 = b2;
}

void Line::setLine(Point p1, Point p2)
{
	x1 = p1.x;
	y1 = p1.y;
	x2 = p2.x;
	y2 = p2.y;
}

void Line::setP1(Point p1)
{
	x1 = p1.x;
	y1 = p1.y;
}

void Line::setP1(int a1, int b1)
{
	x1 = a1;
	y1 = b1;
}

void Line::setP2(Point p2)
{
	x2 = p2.x;
	y2 = p2.y;
}

void Line::setP2(int a2, int b2)
{
	x2 = a2;
	y2 = b2;
}

void Line::translate(int dx, int dy)
{
	x1 += dx;
	y1 += dy;
	x2 += dx;
	y2 += dy;
}

//check if two lines are neighboring. allow only 1 dot difference 
bool Line::isNeighbor(Line line1, Line line2)
{
	return ((abs(line1.getP1().x - line2.getP1().x) < 2 &&
			 abs(line1.getP1().y - line2.getP1().y) < 2) &&
			(abs(line1.getP2().x - line2.getP2().x) < 2 &&
			abs(line1.getP2().y - line2.getP2().y) < 2));
}

bool Line::isHorizontal()
{
	return (y1 == y2);
}

bool Line::isVertical()
{
	return (x1 == x2);
}

bool Line::isCross(Line line1, Line line2)
{
	if (line1.isHorizontal() && line2.isVertical())
	{
		if (line1.getP1().y > line2.getP1().y &&
				line1.getP1().y < line2.getP2().y &&
				line2.getP1().x > line1.getP1().x &&
				line2.getP1().x < line1.getP2().x)
			return true;
	} 
	else if (line1.isVertical() && line2.isHorizontal())
	{
		if (line1.getP1().x > line2.getP1().x &&
				line1.getP1().x < line2.getP2().x &&
				line2.getP1().y > line1.getP1().y &&
				line2.getP1().y < line1.getP2().y)
			return true;
	}
	
	return false;
}

Point Line::getCenter()
{
	int x = (x1 + x2) / 2;
	int y = (y1 + y2) / 2;
	return Point(x , y);
}

int Line::getLength()
{
	int x = abs(x2 - x1);
	int y = abs(y2 - y1);
	int r = (int)sqrt(float(x * x + y * y));
	return r;
}
	
Line Line::getLongest(Line * lines,int length)
{
	Line longestLine;
	for (int i = 0; i < length; i++) {
		if (lines[i].getLength() > longestLine.getLength())
		{
			longestLine = lines[i];
		}
	}
	return longestLine;
}

/*CString Line::toString()
{
return "(" + x1 + "," + y1 + ")-(" + x2 + "," + y2 + ")";
}
*/
