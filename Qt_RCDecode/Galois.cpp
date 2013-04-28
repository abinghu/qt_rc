// Galois.cpp: implementation of the Galois class.
//
//////////////////////////////////////////////////////////////////////

#include "Galois.h"
#include <cstring>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Galois::Galois()
{
	initGaloisTable();
}

Galois::~Galois()
{

}

/*
* 整数、指数相互变换数据的生成
*/
void Galois::initGaloisTable()
{
	//logTbl[0] = 0;
	int d = 1;
	for(int i = 0; i < 255; i++) 
	{
		expTbl[i] = expTbl[255 + i] = d;
		logTbl[d] = i;
		d <<= 1;
		if((d & 0x100) != 0) 
		{
			d=(d^POLYNOMIAL)&0xff;
		}
	}
}

/**
 *指数到整数的变换
 *
 * @param a int
 * @return int
 */
int Galois::toExp(int a)
{
	return expTbl[a];
}

/**
 * 整数到指数的变换
 *
 * @param a int
 * @return int
 */
int Galois::toLog(int a)
{
	return logTbl[a];
}

/**
 * 计算错误位置index
 *
 * @param length int,数据长度
 * @param a int,错误位置整数
 * @return int,错误位置index
 */
int Galois::toPos(int length, int a)
{
	return length - 1 - logTbl[a];
}

/**
 * 乘法
 *
 * @param a int
 * @param b int
 * @return int = a * b
 */
int Galois::mul(int a, int b)
{
	return (a == 0 || b == 0)? 0 : expTbl[logTbl[a] + logTbl[b]];
}

/**
 * 乘以指数
 *
 * @param a int
 * @param b int
 * @return int = a * α^b
 */
int Galois::mulExp(int a, int b)
{
	return (a == 0)? 0 : expTbl[logTbl[a] + b];
}

/**
 * 除法
 *
 * @param a int
 * @param b int
 * @return int = a / b
 */
int Galois::div(int a, int b)
{
	return (a == 0)? 0 : expTbl[logTbl[a] - logTbl[b] + 255];
}

/**
 * 除以指数
 *
 * @param a int
 * @param b int
 * @return int = a / α^b
 */
int Galois::divExp(int a, int b)
{
	return (a == 0)? 0 : expTbl[logTbl[a] - b + 255];
}

/**
 * 倒数
 *
 * @param a int
 * @return int = 1/a
 */
int Galois::inv(int a)
{
	return expTbl[255 - logTbl[a]];
}

/**
 * 多项式乘法
 *
 * @param seki int[]
 * 		seki = a * b
 * @param a int[]
 * @param b int[]
 */
void Galois::mulPoly(int * seki, int * a, int * b, int sekilength, int alength, int blength)
{
	memset(seki, 0, sekilength);
	for(int ia = 0; ia < alength; ia++) 
	{
		if(a[ia] != 0) 
		{
			int loga = logTbl[a[ia]];
			int ib2 = blength < (sekilength-ia) ? blength : (sekilength-ia);
			for(int ib = 0; ib < ib2; ib++) 
			{
				if(b[ib] != 0) 
				{
					seki[ia + ib] ^= expTbl[loga + logTbl[b[ib]]];	// = a[ia] * b[ib]
				}
			}
		}
	}
}

/**
 * 校验计算
 * @param data int[]
 *		输入数据数组
 * @param length int
 *		数据长度
 * @param syn int[]
 *		(x - α^0) (x - α^1) (x - α^2) ...的校验
 * @return boolean
 *		true: 所有校验为0，校验错误数为0
 */
bool Galois::calcSyndrome(unsigned char * data, int length, int * syn, int synlength)
{
	int hasErr = 0;
	for(int i = 0; i < synlength;  i++) 
	{
		int wk = 0;
		for(int idx = 0; idx < length; idx++) 
		{
			 wk = data[idx] ^ ((wk == 0)? 0 : expTbl[logTbl[wk] + i]);		// wk = data + wk * α^i
		}
		syn[i] = wk;
		hasErr |= wk;
	}
	return hasErr == 0;
}
