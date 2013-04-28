// FinderPattern.cpp: implementation of the FinderPattern class.
//
//////////////////////////////////////////////////////////////////////

#include "FinderPattern.h"
#include "QRCodeInfo.h"

#include <QMessageBox>
#include<vector>
using std::vector;
#include<math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FinderPattern::FinderPattern()
{

}

FinderPattern::~FinderPattern()
{

}

//探测图形检查
void FinderPattern::findFinderPattern(unsigned char ** mybitmap, int mynWidth, int mynHeight, int myDECIMAL_POINT)
{
	DECIMAL_POINT=myDECIMAL_POINT;
	bitmap=mybitmap;
	nWidth=mynWidth;
	nHeight=mynHeight;

	findLineAcross();
	findLineCross();
	getCenter(m_lineCross,m_lineCrossCnt);
	getAngle(m_center,m_centerCnt);
	sort(m_center,m_sincos);
	getWidth();
	// moduleSize for version recognition
	m_moduleSize[0]=(m_width[UL] << DECIMAL_POINT)/7;
	m_moduleSize[1]=(m_width[UR] << DECIMAL_POINT)/7;
	m_moduleSize[2]=(m_width[DL] << DECIMAL_POINT)/7;
	
	m_version = calcRoughVersion(m_center, m_width);
	if (m_version > 6)
	{
		m_version = calcExactVersion(m_center, m_sincos, m_moduleSize);			
	}

	//return FinderPattern (center, version, sincos, width, moduleSize);
}

//粗略版本号
int FinderPattern::calcRoughVersion(Point * center, int * width)
{
	const int dp = DECIMAL_POINT;
	int lengthAdditionalLine = (Line(center[UL], center[UR]).getLength()) << dp ;
	int avarageWidth = ((width[UL] + width[UR]) << dp) / 14;
	int roughVersion = ((lengthAdditionalLine / avarageWidth) - 10) / 4;
	if (((lengthAdditionalLine / avarageWidth) - 10) % 4 >= 2) {
		roughVersion++;
	}
	return roughVersion;	
}

//精确版本号
int FinderPattern::calcExactVersion(Point * centers, int * angle, int * moduleSize) 
{
	unsigned char * VersionInfo = new unsigned char[18];
	Point * points = new Point[18];
	Point target;

	//右上 --- 版本信息1
	Axis axis = Axis(angle, moduleSize[UR], DECIMAL_POINT); //UR
	axis.setOrigin(centers[UR]);
	for (int y = 0; y < 6; y++) {
		for (int x = 0; x < 3; x++) {
			target = axis.translate(x - 7, y - 3);
			VersionInfo[x + y * 3] = bitmap[target.x][target.y];
			points[x + y * 3] = target;
		}
	}
	
    //纠错检查
	int errorCount=0,versionBase;
    for(versionBase=0;versionBase<=33;versionBase++)
	{
        errorCount=0;
        for(int j=0;j<18;j++)
		{
            if((VersionInfo[j]^(VersionInfoBit[versionBase]>>j))%2==1)
                errorCount++;
        }
        if(errorCount<=3)break;
    }
    if(errorCount<=3)
        return 7+versionBase;
    else
	{
		//左下 --- 版本信息2
		axis.setOrigin(centers[DL]);
		axis.setModulePitch(moduleSize[DL]); //DL		
		for (int x = 0; x < 6; x++) {
			for (int y = 0; y < 3; y++) {
				target = axis.translate(x - 3, y - 7);
				VersionInfo[y + x * 3] = bitmap[target.x][target.y];
				points[x + y * 3] = target;
			}
		}
        
		//纠错检查
		errorCount=0;
        for(versionBase=0;versionBase<=33;versionBase++)
		{
            errorCount=0;
            for(int j=0;j<18;j++)
			{
                if((VersionInfo[j]^(VersionInfoBit[versionBase]>>j))%2==1)
                    errorCount++;
            }
            if(errorCount<=3)break;
        }
        if(errorCount<=3)
            return 7+versionBase;
        else     
		{
			QMessageBox::critical(NULL, "Error", "版本信息纠错失败");
			exit(0);  
		}
	}

	return 0;
}

//获取探测图形中心点
void FinderPattern::getCenter(Line * crossLines,int crossLinesCnt) 
{
	int i,j;
	vector<Point> centers;
	for (i = 0; i < crossLinesCnt - 1; i++)
	{
		Line compareLine = crossLines[i];
		for (j = i + 1; j < crossLinesCnt; j++)
		{
			Line comparedLine = crossLines[j];
			if (line.isCross(compareLine, comparedLine))
			{
				int x = 0;
				int y = 0;
				if (compareLine.isHorizontal())
				{
					x = compareLine.getCenter().x;
					y = comparedLine.getCenter().y;		
				}
				else {
					x = comparedLine.getCenter().x;
					y = compareLine.getCenter().y;
				}
				centers.push_back(Point(x,y));
			}
		}
	}

	m_centerCnt=centers.size();
	m_center=new Point[centers.size()];	
	for (i = 0; i < m_centerCnt; i++)
		m_center[i] = centers[i];
	
	if (m_centerCnt != 3)
	{
		QMessageBox::critical(NULL, "Error", "Invalid number of Finder Pattern detected");
		exit(0);
	}
}

//检查角度 obtain slope of symbol
void FinderPattern::getAngle(Point * centers,int centersCnt)
{

	int i;
	Line * additionalLine = new Line[3];

	for (i = 0; i < 3; i++)
	{
		additionalLine[i] = Line(centers[i],centers[(i + 1) % 3]);
	}
	// remoteLine - does not contain UL center
	Line remoteLine = line.getLongest(additionalLine,3);
	Point originPoint;
	for (i = 0; i < centersCnt; i++) {
		if (!remoteLine.getP1().equals(centers[i]) &&
			 !remoteLine.getP2().equals(centers[i])) {
			originPoint = centers[i];
			break;
		}
	}
	//canvas.println("originPoint is: " + originPoint);
	Point remotePoint;

	//with origin that the center of Left-Up Finder Pattern, determine other two patterns center.
	//then calculate symbols angle
	if (originPoint.y <= remoteLine.getP1().y && //1st or 2nd quadrant
			originPoint.y <= remoteLine.getP2().y)
		if (remoteLine.getP1().x < remoteLine.getP2().x)
			remotePoint = remoteLine.getP2();
		else
			remotePoint = remoteLine.getP1();
	else if (originPoint.x >= remoteLine.getP1().x && //2nd or 3rd quadrant
			originPoint.x >= remoteLine.getP2().x)
		if (remoteLine.getP1().y < remoteLine.getP2().y)
			remotePoint = remoteLine.getP2();
		else
			remotePoint = remoteLine.getP1();
	else if (originPoint.y >= remoteLine.getP1().y && //3rd or 4th quadrant
			originPoint.y >= remoteLine.getP2().y)
		if (remoteLine.getP1().x < remoteLine.getP2().x)
			remotePoint = remoteLine.getP1();
		else
			remotePoint = remoteLine.getP2();
	else //1st or 4th quadrant
		if (remoteLine.getP1().y < remoteLine.getP2().y)
			remotePoint = remoteLine.getP1();
		else
			remotePoint = remoteLine.getP2();
	
	int r = Line(originPoint, remotePoint).getLength();
	m_sincos[0] = ((remotePoint.y - originPoint.y) << DECIMAL_POINT) / r; //Sin
	m_sincos[1] = ((remotePoint.x - originPoint.x) << DECIMAL_POINT) / r; //Cos
	
}

//按顺序存储探测图形 sort center of finder patterns as Left-Up: points[0], Right-Up: points[1], Left-Down: points[2].
void FinderPattern::sort(Point * centers, int * angle)
{

	Point * sortedCenters = new Point[3];

	int quadrant;
	int sin = angle[0];
	int cos = angle[1];
	if (sin >= 0 && cos > 0)quadrant=1;
	else if (sin > 0 && cos <= 0)quadrant=2;
	else if (sin <= 0 && cos < 0)quadrant=3;
	else if (sin < 0 && cos >= 0)quadrant=4;
	else quadrant=0;

	switch (quadrant) {
	case 1:
		sortedCenters[1] = getPointAtSide(centers, 3, RIGHT, BOTTOM);
		sortedCenters[2] = getPointAtSide(centers, 3, BOTTOM, LEFT);
		break;
	case 2:
		sortedCenters[1] = getPointAtSide(centers, 3, BOTTOM, LEFT);
		sortedCenters[2] = getPointAtSide(centers, 3, TOP, LEFT);
		break;
	case 3:
		sortedCenters[1] = getPointAtSide(centers, 3, LEFT, TOP);			
		sortedCenters[2] = getPointAtSide(centers, 3, RIGHT, TOP);
		break;
	case 4:
		sortedCenters[1] = getPointAtSide(centers, 3, TOP, RIGHT);
		sortedCenters[2] = getPointAtSide(centers, 3, BOTTOM, RIGHT);
		break;
	}

	//last of centers is Left-Up patterns one
	for (int i = 0; i < 3; i++) {
		if (!centers[i].equals(sortedCenters[1]) && !centers[i].equals(sortedCenters[2])) {
			sortedCenters[0] = centers[i];
		}
	}
	for (int i = 0; i < 3; i++)
		centers[i]=sortedCenters[i];

}

//获取宽度
void FinderPattern::getWidth() 
{	
	for (int i = 0; i < 3; i++)
	{
		bool flag = false;
		int lx, rx;
		int y = m_center[i].y;
		for (lx = m_center[i].x; lx >= 0; lx--)
		{
			if (bitmap[lx][y] == 1/*QRCodeImageReader.POINT_DARK*/ &&
					bitmap [lx - 1][y] == 0/*QRCodeImageReader.POINT_LIGHT*/)
			{
				if (flag == false) flag = true;
				else break;
			}
		}
		flag = false;
		for (rx = m_center[i].x; rx < nWidth; rx++)
		{
			if (bitmap[rx][y] == 1/*QRCodeImageReader.POINT_DARK*/ &&
					bitmap[rx + 1][y] == 0/*QRCodeImageReader.POINT_LIGHT*/)
			{
				if (flag == false) flag = true;
				else break;
			}
		}
		m_width[i] = (rx - lx + 1);
	}
}

Point FinderPattern::getPointAtSide(Point * points,int pointCnt,int side1,int side2)
{
	int x = ((side1 == RIGHT || side2 == RIGHT) ? 0 : MAX_VALUE);
	int y = ((side1 == BOTTOM || side2 == BOTTOM) ? 0 : MAX_VALUE);
	Point sidePoint = Point(x, y);
		

	for (int i = 0; i < pointCnt; i++)
	{
		switch (side1) {
		case RIGHT:
			if (sidePoint.x < points[i].x) {
				sidePoint = points[i];
			}
			else if (sidePoint.x == points[i].x) {
				if (side2 == BOTTOM) {
					if (sidePoint.y < points[i].y) {
						sidePoint = points[i];
					}
				}
				else {
					if (sidePoint.y > points[i].y) {
						sidePoint = points[i];
					}
				}
			}
			break;
		case BOTTOM:
			if (sidePoint.y < points[i].y) {
				sidePoint = points[i];
			}
			else if (sidePoint.y == points[i].y) {
				if (side2 == RIGHT) {
					if (sidePoint.x < points[i].x) {
						sidePoint = points[i];
					}
				}
				else {
					if (sidePoint.x > points[i].x) {
						sidePoint = points[i];
					}
				}
			}
			break;
		case LEFT:
			if (sidePoint.x > points[i].x) {
				sidePoint = points[i];
			}
			else if (sidePoint.x == points[i].x) {
				if (side2 == BOTTOM) {
					if (sidePoint.y < points[i].y) {
						sidePoint = points[i];
					}
				}
				else {
					if (sidePoint.y > points[i].y) {
						sidePoint = points[i];
					}
				}
			}
			break;
		case TOP:
			if (sidePoint.y > points[i].y) {
				sidePoint = points[i];
			}
			else if (sidePoint.y == points[i].y) {
				if (side2 == RIGHT) {
					if (sidePoint.x < points[i].x) {
						sidePoint = points[i];
					}
				}
				else {
					if (sidePoint.x > points[i].x) {
						sidePoint = points[i];
					}
				}
			}
			break;
		}
	}
	return sidePoint;

}

//检验探测图形比例
bool FinderPattern::checkPattern(int * buffer, int pointer)
{
	int modelRatio[] = {1, 1, 3, 1, 1};	

	int baselength = 0;
	for (int i = 0; i < 5; i++) {
		baselength += buffer[i];
	}
	// pseudo fixed point calculation. I think it needs smarter code
	baselength <<= DECIMAL_POINT; 
	baselength /= 7;
	for  (int i = 0; i < 5; i++) {
		int leastlength = baselength * modelRatio[i] - baselength / 2;
		int mostlength = baselength * modelRatio[i] + baselength / 2;
		
		//TODO rough finder pattern detection
		
		int targetlength = buffer[(pointer + i + 1) % 5] << DECIMAL_POINT;
		if (targetlength < leastlength || targetlength > mostlength) {
			return false;
		}
	}
	return true;
}

void FinderPattern::findLineAcross()
{
	int i,j;

	const int READ_HORIZONTAL = 0;
	const int READ_VERTICAL = 1;

	//int currentX = 0, currentY = 0;
	Point current;
	
	vector<Line> lineAcross;
	
	//buffer contains recent length of modules which has same brightness
	int * lengthBuffer = new int[5];
	for(i=0;i<5;i++)
		lengthBuffer[i]=0;
	int  bufferPointer = 0;
	
	int direction = READ_HORIZONTAL; //start to read horizontally
	unsigned char lastElement = 0;/*QRCodeImageReader.POINT_LIGHT*/

	while(true) {
		//check points in image
		unsigned char currentElement = bitmap[current.x][current.y];
		if (currentElement == lastElement) { //target point has same brightness with last point
			lengthBuffer[bufferPointer]++;
		}
		else { //target point has different brightness with last point
			if (currentElement == 0/*QRCodeImageReader.POINT_LIGHT*/)
			{
				if (checkPattern(lengthBuffer, bufferPointer))
				{ //detected pattern
					int x1, y1, x2, y2;
					if (direction == READ_HORIZONTAL) {
						//obtain X coordinates of both side of the detected horizontal pattern
						x1 = current.x; 
						for (j = 0; j < 5; j++) {
							x1 -= lengthBuffer[j];
						}
						x2 = current.x - 1; //right side is last X coordinate
						y1 = y2 = current.y;
					}
					else {
						x1 = x2 = current.x;
						//obtain Y coordinates of both side of the detected vertical pattern
						// upper side is sum of length of buffer
						y1 = current.y; 
						for (j = 0; j < 5; j++) {
							y1 -= lengthBuffer[j];
						}
						y2 = current.y - 1; // bottom side is last Y coordinate
					}
					lineAcross.push_back(Line(x1, y1, x2, y2));
				}
			}
			bufferPointer = (bufferPointer + 1) % 5; 
			lengthBuffer[bufferPointer] = 1;
			lastElement = !lastElement;
		}
		
		// determine if read next, change read direction or terminate this loop
		if (direction == READ_HORIZONTAL) {
			if (current.x < nWidth - 1) {
				current.translate(1, 0);
			}
			else if (current.y < nHeight - 1) {
				current.set(0, current.y + 1);
				lengthBuffer =  new int[5];
			}
			else {
				current.set(0, 0); //reset target point
				lengthBuffer =  new int[5];
				direction = READ_VERTICAL; //start to read vertically
			}
		}
		else { //reading vertically
			if (current.y < nHeight - 1)
				current.translate(0, 1);
			else if (current.x < nWidth - 1) {
				current.set(current.x + 1, 0);
				lengthBuffer = new int[5];
			}
			else {
				break;
			}
		}
	}

	m_lineAcrossCnt = lineAcross.size();
	m_lineAcross = new Line[lineAcross.size()];
	for (unsigned int i = 0; i < lineAcross.size(); i++)
		m_lineAcross[i] = lineAcross[i];

}

bool FinderPattern::cantNeighbor(Line line1, Line line2)
{
	if (line.isCross(line1, line2))
		return true;
	return line1.isHorizontal()? abs(line1.getP1().y - line2.getP1().y) > 1 : abs(line1.getP1().x - line2.getP1().x) > 1;
}

void FinderPattern::findLineCross()
{
	vector<Line> crossLines;
	vector<Line> lineNeighbor;
	vector<Line> lineCandidate;

	Line compareLine;
	for (int i = 0; i < m_lineAcrossCnt; i++)
		lineCandidate.push_back(m_lineAcross[i]);

	for (unsigned int i = 0; i < lineCandidate.size() - 1; i++)
	{
		lineNeighbor.clear();
		lineNeighbor.push_back(lineCandidate[i]);
		for (unsigned int j = i + 1; j < lineCandidate.size(); j++)
		{
			if (line.isNeighbor(lineNeighbor.back(), lineCandidate[j]))
			{
				lineNeighbor.push_back(lineCandidate[j]);
				compareLine = lineNeighbor.back();
				if (lineNeighbor.size() * 5 > compareLine.getLength() &&
						j == lineCandidate.size() - 1) {
					crossLines.push_back(lineNeighbor[lineNeighbor.size()/2]);
					for (int k = 0; k < lineNeighbor.size(); k++)
					{
						for(int index=0;index<lineCandidate.size();index++)
						{
							if(lineCandidate[index].x1==lineNeighbor[k].x1 && lineCandidate[index].y1==lineNeighbor[k].y1
								&& lineCandidate[index].x2==lineNeighbor[k].x2 && lineCandidate[index].y2==lineNeighbor[k].y2)
								lineCandidate.erase(lineCandidate.begin()+index);
						}
						//lineCandidate.removeElement(lineNeighbor.elementAt(k));
					}
				}
			}
			//terminate comparison if there are no possibility for found neighbour lines
			else if(cantNeighbor(lineNeighbor.back(), lineCandidate[j]) || (j == lineCandidate.size() - 1))  
			{
				compareLine = lineNeighbor.back();
				//determine lines across Finder Patterns when number of neighbour lines are bigger than 1/6 length of theirselves
				if (lineNeighbor.size() * 6 > compareLine.getLength())
				{
					crossLines.push_back(lineNeighbor[lineNeighbor.size()/2]);
					for (int k = 0; k < lineNeighbor.size(); k++)
					{
						for(int index=0;index<lineCandidate.size();index++)
						{
							if(lineCandidate[index].x1==lineNeighbor[k].x1 && lineCandidate[index].y1==lineNeighbor[k].y1
								&& lineCandidate[index].x2==lineNeighbor[k].x2 && lineCandidate[index].y2==lineNeighbor[k].y2)
								lineCandidate.erase(lineCandidate.begin()+index);
						}
						//lineCandidate.removeElement(lineNeighbor.elementAt(k));
					}
				}
				break;
			}
		}
	}	

	m_lineCrossCnt = crossLines.size();
	m_lineCross = new Line[crossLines.size()];
	for (int i = 0; i < crossLines.size(); i++)
		m_lineCross[i] = crossLines[i];

}
