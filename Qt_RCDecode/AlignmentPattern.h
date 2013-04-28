// AlignmentPattern.h: interface for the AlignmentPattern class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALIGNMENTPATTERN_H__93CF4CC8_0FFC_4961_BD84_F52A524CA14E__INCLUDED_)
#define AFX_ALIGNMENTPATTERN_H__93CF4CC8_0FFC_4961_BD84_F52A524CA14E__INCLUDED_

#include "FinderPattern.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AlignmentPattern  
{
public:
	AlignmentPattern();
	virtual ~AlignmentPattern();

public:

	int DECIMAL_POINT;
	unsigned char ** bitmap;
	int nWidth;
	int nHeight;

	FinderPattern * m_finderPattern;
	Point ** logicalCenters;
	int logicalCentersCnt;
	int logicalDistance;
	Point ** m_centers;
	int m_centersCnt;

	void findAlignmentPattern(unsigned char ** mybitmap, int mynWidth, int mynHeight, FinderPattern * finderPattern, int myDECIMAL_POINT);
	void getLogicalCenter(FinderPattern * finderPattern);
	void getCenter();
	Point getPrecisionCenter(Point targetPoint);
	bool targetPointOnTheCorner(int x, int y, int nx, int ny);

};

#endif // !defined(AFX_ALIGNMENTPATTERN_H__93CF4CC8_0FFC_4961_BD84_F52A524CA14E__INCLUDED_)
