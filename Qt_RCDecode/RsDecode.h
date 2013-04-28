// RsDecode.h: interface for the RsDecode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSDECODE_H__FE4D1CBB_E370_45D8_9F1A_5359D19C47C1__INCLUDED_)
#define AFX_RSDECODE_H__FE4D1CBB_E370_45D8_9F1A_5359D19C47C1__INCLUDED_

#include "Galois.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RS_PERM_ERROR -1
#define RS_CORRECT_ERROR -2
class RsDecode  
{
public:
	RsDecode();
	virtual ~RsDecode();

public:
	Galois galois;
	int npar;
	RsDecode(int npar);
	int calcSigmaMBM(int * sigma, int * omega, int * syn);
	int chienSearch(int * pos, int n, int jisu, int * sigma);
	void doForney(unsigned char * data, int length, int jisu, int * pos, int * sigma, int * omega);
	int decode(unsigned char * data, int length, bool noCorrect);
	int decode(unsigned char * data, int length);
};

#endif // !defined(AFX_RSDECODE_H__FE4D1CBB_E370_45D8_9F1A_5359D19C47C1__INCLUDED_)
