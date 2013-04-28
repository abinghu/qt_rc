// Galois.h: interface for the Galois class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GALOIS_H__A3338436_5E78_48F4_8D2B_C5272731225E__INCLUDED_)
#define AFX_GALOIS_H__A3338436_5E78_48F4_8D2B_C5272731225E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define POLYNOMIAL 0x1d

class Galois  
{
public:
	Galois();
	virtual ~Galois();

public:
	int expTbl[255*2];
	int logTbl[255+1];
	void initGaloisTable();
	int toExp(int a);
	int toLog(int a);
	int toPos(int length, int a);
	int mul(int a, int b);
	int mulExp(int a, int b);
	int div(int a, int b);
	int divExp(int a, int b);
	int inv(int a);
	void mulPoly(int * seki, int * a, int * b, int sekilength, int alength, int blength);
	bool calcSyndrome(unsigned char * data, int length, int * syn, int synlength);
};

#endif // !defined(AFX_GALOIS_H__A3338436_5E78_48F4_8D2B_C5272731225E__INCLUDED_)
