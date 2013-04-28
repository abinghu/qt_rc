// AlignmentPattern.cpp: implementation of the AlignmentPattern class.
//
//////////////////////////////////////////////////////////////////////

#include "AlignmentPattern.h"
#include "QRCodeInfo.h"
#include <QMessageBox>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AlignmentPattern::AlignmentPattern()
{

}

AlignmentPattern::~AlignmentPattern()
{

}

//Ð£ÕýÍ¼ÐÎ¼ì²â
void AlignmentPattern::findAlignmentPattern(unsigned char ** mybitmap, int mynWidth, int mynHeight, FinderPattern * finderPattern, int myDECIMAL_POINT) 
{
	DECIMAL_POINT=myDECIMAL_POINT;
	bitmap=mybitmap;
	nWidth=mynWidth;
	nHeight=mynHeight;
	m_finderPattern=finderPattern;

	getLogicalCenter(m_finderPattern);

	logicalDistance = logicalCenters[1][0].x - logicalCenters[0][0].x;

	//With it converts in order to handle in the same way
	getCenter();

	//return new AlignmentPattern(centers, logicalDistance);

}

//get logical center coordinates of each alignment patterns
void AlignmentPattern::getLogicalCenter(FinderPattern * finderPattern)
{

	//get logical center coordinates of each alignment patterns
	int version = finderPattern->m_version;

	int logicalSeedsCnt;
	logicalSeedsCnt=QR_VersonInfo[version].ncAlignPoint+1;
	int * logicalSeeds=new int[logicalSeedsCnt];
	logicalSeeds[0]=6;
	for(int i=1;i<logicalSeedsCnt;i++)
		logicalSeeds[i]=QR_VersonInfo[version].nAlignPoint[i-1];
	
	logicalCenters = new Point *[logicalSeedsCnt];
	for(int i=0;i<logicalSeedsCnt;i++)
		logicalCenters[i]=new Point[logicalSeedsCnt];
	//Point[][] logicalCenters = new Point[logicalSeeds.length][logicalSeeds.length];
	
	//create real relative coordinates
	for (int col = 0; col < logicalSeedsCnt; col++)
	{
		for (int row = 0; row < logicalSeedsCnt; row++)
		{
			logicalCenters[row][col] = Point(logicalSeeds[row], logicalSeeds[col]);
		}
	}
	logicalCentersCnt=logicalSeedsCnt;
	
}

void AlignmentPattern::getCenter() 
{
	int moduleSize = m_finderPattern->m_moduleSize[UL];

	int * angle=m_finderPattern->m_sincos;
	Axis axis = Axis(angle, moduleSize, DECIMAL_POINT);
	
	m_centersCnt = logicalCentersCnt;
	m_centers = new Point *[m_centersCnt];
	for(int i=0;i<m_centersCnt;i++)
		m_centers[i]=new Point[m_centersCnt];
	//Point[][] centers = new Point[m_centersCnt][m_centersCnt];
	
	axis.setOrigin(m_finderPattern->m_center[UL]);
	m_centers[0][0] = axis.translate(3, 3);

	axis.setOrigin(m_finderPattern->m_center[UR]);
	m_centers[m_centersCnt - 1][0] = axis.translate(-3, 3);

	axis.setOrigin(m_finderPattern->m_center[DL]);
	m_centers[0][m_centersCnt - 1] = axis.translate(3, -3);

	Point tmpPoint=m_centers[0][0];

	for (int y = 0; y < m_centersCnt; y++)
	{
		for (int x = 0; x < m_centersCnt; x++)
		{
			if ((x==0 && y==0) || (x==0 && y==m_centersCnt-1) || (x==m_centersCnt-1 && y==0))
			{
				continue;
			}
			Point target;
			if (y == 0)
			{
				if (x > 0 && x < m_centersCnt - 1)
				{
					target = axis.translate(m_centers[x-1][y], logicalCenters[x][y].x-logicalCenters[x-1][y].x, 0);
					m_centers[x][y] = Point(target.x, target.y);
				}
			}
			else if (x == 0)
			{
				if (y > 0 && y < m_centersCnt - 1)
				{
					target = axis.translate(m_centers[x][y-1], 0, logicalCenters[x][y].y-logicalCenters[x][y-1].y);
					m_centers[x][y] = Point(target.x, target.y);
				}
			}
			else
			{
				Point t1 = axis.translate(m_centers[x-1][y], logicalCenters[x][y].x-logicalCenters[x-1][y].x, 0);
				Point t2 = axis.translate(m_centers[x][y-1], 0, logicalCenters[x][y].y-logicalCenters[x][y-1].y);
				m_centers[x][y] = Point((t1.x+t2.x)/2, (t1.y + t2.y)/2+1);
			}
			if (m_finderPattern->m_version > 1)
			{
				Point precisionCenter = getPrecisionCenter(m_centers[x][y]);

				int dx = precisionCenter.x - m_centers[x][y].x;
				int dy = precisionCenter.y - m_centers[x][y].y;
				
				m_centers[x][y] = precisionCenter;
			}
			tmpPoint=m_centers[x][y];
		}
	}
}

Point AlignmentPattern::getPrecisionCenter(Point targetPoint) 
{
	// find nearest dark point and update it as new rough center point 
	// when original rough center points light point 
	int tx = targetPoint.x, ty = targetPoint.y;
	if ((tx < 0 || ty < 0) || (tx > nWidth - 1 || ty > nHeight -1))
	{
		QMessageBox::critical(NULL, "Error", "Alignment Pattern finder exceeded out of image", QMessageBox::Ok);
		exit(0);
	}
		
	if (bitmap[targetPoint.x][targetPoint.y] == 0/*QRCodeImageReader.POINT_LIGHT*/)
	{
		int scope = 0;
		bool found = false;
		while (!found) {
			scope++;
			for (int dy = scope; dy > -scope; dy--)
			{
				for (int dx = scope; dx > -scope; dx--)
				{
					int x = targetPoint.x + dx;
					int y = targetPoint.y + dy;
					if ((x < 0 || y < 0) || (x > nWidth - 1 || y > nHeight -1))
					{
						QMessageBox::critical(NULL, "Error", "Alignment Pattern finder exceeded out of image", QMessageBox::Ok);
						exit(0);
					}
					if (bitmap[x][y] == 1/*RCodeImageReader.POINT_DARK*/)
					{
						targetPoint = Point(targetPoint.x + dx,targetPoint.y + dy);
						found = true;
					}
				}
			}
		}
	}
	int x, lx, rx, y, uy, dy;
	x = lx = rx = targetPoint.x;
	y = uy = dy = targetPoint.y;

	while (lx >= 1 && !targetPointOnTheCorner(lx, y, lx - 1, y)) lx--;
	while (rx < nWidth-1 && !targetPointOnTheCorner(rx, y, rx + 1, y)) rx++;
	while (uy >= 1 && !targetPointOnTheCorner(x, uy, x, uy - 1)) uy--;
	while (dy < nHeight-1 && !targetPointOnTheCorner(x, dy, x, dy + 1)) dy++;
	
	return Point((lx + rx + 1) / 2, (uy + dy + 1) / 2);
}

bool AlignmentPattern::targetPointOnTheCorner(int x, int y, int nx, int ny)
{
	if( x < 0 || y < 0 || nx < 0 || ny < 0 || x > nWidth || y > nHeight || nx > nWidth || ny > nHeight)
	{
		QMessageBox::critical(NULL, "Error", "Alignment Pattern finder exceeded out of image", QMessageBox::Ok);
		exit(0);
	}
	else {
		return(bitmap[x][y] == 0/*QRCodeImageReader.POINT_LIGHT*/ && bitmap[nx][ny] == 1/*QRCodeImageReader.POINT_DARK*/);
	}
}
