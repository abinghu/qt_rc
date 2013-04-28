// FinderPattern.h: interface for the FinderPattern class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINDERPATTERN_H__0E3C94D8_916A_4D12_B317_273ED71B9BAA__INCLUDED_)
#define AFX_FINDERPATTERN_H__0E3C94D8_916A_4D12_B317_273ED71B9BAA__INCLUDED_

#include "Point.h"
#include "Line.h"
#include "Axis.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_VALUE 25000
#define UL 0
#define UR 1
#define DL 2

class FinderPattern  
{
public:
	FinderPattern();
	virtual ~FinderPattern();
public:
	Line line;//for invoke
	Point point;//for invoke
	Line * m_lineAcross;
	int m_lineAcrossCnt;
	Line * m_lineCross;
	int m_lineCrossCnt;

	Point * m_center;
	int m_centerCnt;
	int m_version;
	int m_sincos[2];
	int m_width[3];
	int m_moduleSize[3];

	unsigned char ** bitmap;
	int DECIMAL_POINT;
	int nWidth;
	int nHeight;

	void findFinderPattern(unsigned char ** mybitmap,int mynWidth,int mynHeight,int myDECIMAL_POINT);
	void getCenter(Line * crossLines,int crossLinesCnt); 
	void getAngle(Point * centers,int centersCnt);
	void sort(Point * centers, int * angle);
	bool checkPattern(int * buffer, int pointer);
	Point getPointAtSide(Point * points,int pointCnt,int side1,int side2);
	bool cantNeighbor(Line line1, Line line2);
	void findLineAcross();
	void findLineCross();
	void getWidth() ;
	int calcRoughVersion(Point * center, int * width);
	int calcExactVersion(Point * centers, int * angle, int * moduleSize);

};

#endif // !defined(AFX_FINDERPATTERN_H__0E3C94D8_916A_4D12_B317_273ED71B9BAA__INCLUDED_)
