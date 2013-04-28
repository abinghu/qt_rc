// QRcodeImage.h: interface for the QRcodeImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QRCODEIMAGE_H__175E17CA_EB25_4222_8BAC_A1FEDB4AF1B5__INCLUDED_)
#define AFX_QRCODEIMAGE_H__175E17CA_EB25_4222_8BAC_A1FEDB4AF1B5__INCLUDED_

#include "Point.h"
#include "Line.h"
#include "Axis.h"
#include "SamplingGrid.h"
#include"ContentDecoder.h"

#include "FinderPattern.h"
#include "AlignmentPattern.h"

#include <QImage>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ModulePitch
{
public:
	int top;
	int left;
	int bottom;
	int right;
};

class QRcodeImage  
{
public:
	QRcodeImage();
	virtual ~QRcodeImage();
public:

	unsigned char ** bitmap;//位图数据指针
	unsigned char ** bitMatrix;//矩阵数据指针
	int DECIMAL_POINT;//小数点
	QImage	m_EncodeImage;
	int		m_Width;
	int		m_Height;

	void GetImageData();//返回需要的数据（基本）
	QRcodeImage(QString FilePathName);
	void filterImage();//颜色过滤
	SamplingGrid getSamplingGrid(FinderPattern * finderPattern, AlignmentPattern * alignmentPattern);
	int getAreaModulePitch(Point start, Point end, int logicalDistance);
	void getQRCodeMatrix(SamplingGrid gridLines);

	FinderPattern * finderPattern;
	AlignmentPattern * alignmentPattern;
	SamplingGrid samplingGrid;
};

#endif // !defined(AFX_QRCODEIMAGE_H__175E17CA_EB25_4222_8BAC_A1FEDB4AF1B5__INCLUDED_)
