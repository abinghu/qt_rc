// SamplingGrid.cpp: implementation of the SamplingGrid class.
//
/**
 * This class is used for sampling grid
 * It allows one area to have a different size from another area
 */
//////////////////////////////////////////////////////////////////////

#include "SamplingGrid.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SamplingGrid::SamplingGrid()
{

}

SamplingGrid::~SamplingGrid()
{

}


SamplingGrid::SamplingGrid(int sqrtNumArea)
{
	size=sqrtNumArea;
	grid=new AreaGrid *[sqrtNumArea];
	for(int i=0;i<sqrtNumArea;i++)
		grid[i]=new AreaGrid[sqrtNumArea];
	//grid=new AreaGrid[sqrtNumArea][sqrtNumArea];
}

void SamplingGrid::initGrid(int ax,int ay, int width, int height)
{
	grid[ax][ay]=AreaGrid(width, height);
}
	
void SamplingGrid::setXLine(int ax, int ay, int x, Line line)
{
	grid[ax][ay].setXLine(x,line);
}

void SamplingGrid::setYLine(int ax, int ay, int y, Line line)
{
	grid[ax][ay].setYLine(y,line);
}

Line SamplingGrid::getXLine(int ax, int ay, int x)
{
	return(grid[ax][ay].getXLine(x));
}

Line SamplingGrid::getYLine(int ax, int ay, int y)
{
	return(grid[ax][ay].getYLine(y));
}

Line * SamplingGrid::getXLines(int ax, int ay)
{
	return (grid[ax][ay].getXLines());
}

Line * SamplingGrid::getYLines(int ax, int ay)
{
	return(grid[ax][ay].getYLines());
}

int SamplingGrid::getWidth()
{
	return size;
}

int SamplingGrid::getHeight()
{
	return size;
}

int SamplingGrid::getWidth(int ax, int ay)
{
	return(grid[ax][ay].getWidth());
}

int SamplingGrid::getHeight(int ax, int ay)
{
	return(grid[ax][ay].getHeight());
}
	
int SamplingGrid::getTotalWidth()
{
	int total=0;
	for(int i=0;i<size;i++)
	{
		total+=grid[i][0].getWidth();
		if(i>0)
			total-=1;
	}
	return total;
}

int SamplingGrid::getTotalHeight()
{
	int total=0;
	for(int i=0;i<size;i++)
	{
		total+=grid[0][i].getHeight();
		if(i>0)
			total-=1;
	}
	return total;
}

int SamplingGrid::getX(int ax, int x)
{
	int total=x;
	for(int i=0;i<ax;i++)
	{
		total+=grid[i][0].getWidth()-1;
	}
	return total;
}

int SamplingGrid::getY(int ay, int y)
{
	int total=y;
	for(int i=0;i<ay;i++)
	{
		total+=grid[0][i].getHeight()-1;
	}
	return total;
	}
	
void SamplingGrid::adjust(Point adjust)
{
	int dx = adjust.x, dy = adjust.y;
	for (int ay = 0; ay < size; ay++)
	{
		for (int ax = 0; ax < size; ax++)
		{
			for (int i = 0; i < grid[ax][ay].width; i++)
				grid[ax][ay].xLine[i].translate(dx, dy);
			for (int j = 0; j < grid[ax][ay].height; j++)
				grid[ax][ay].yLine[j].translate(dx, dy);
		}
	}
}
