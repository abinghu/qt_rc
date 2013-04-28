// Point.h: interface for the Point class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT_H__59ACBEEE_F203_4FBF_94AE_514BEADE49AD__INCLUDED_)
#define AFX_POINT_H__59ACBEEE_F203_4FBF_94AE_514BEADE49AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	RIGHT 1
#define BOTTOM 2
#define LEFT 4
#define TOP 8

class Point  
{
public:
	Point();
	Point(int a, int b);
	virtual ~Point();

public:

	int x;
	int y;
	void translate(int dx, int dy);
	void set(int a, int b);
	//CString toString();
	Point getCenter(Point p1, Point p2);
	bool equals(Point compare);
	int distanceOf(Point other);

};

#endif // !defined(AFX_POINT_H__59ACBEEE_F203_4FBF_94AE_514BEADE49AD__INCLUDED_)
