// ContentDecoder.h: interface for the ContentDecoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECODER_H__AC3D15D3_0D11_467B_B981_E73FD0B97EB7__INCLUDED_)
#define AFX_DECODER_H__AC3D15D3_0D11_467B_B981_E73FD0B97EB7__INCLUDED_

#include "RsDecode.h"
#include <QString>

// 版本组
#define QR_VRESION_S	0 // 1-9
#define QR_VRESION_M	1 // 10-26
#define QR_VRESION_L	2 // 27-40

// 纠错级别
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// 数据类型
/*
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_HANZI		3
*/

#define MAX_ALLCODEWORD	 3706 // 码字总数最大值
#define MAX_DATACODEWORD 2956 // 数据码字最大值（版本40-L）
#define MAX_CODEBLOCK	  153 // 模块数据码字数最大值（包括RS码字）
#define MAX_MODULESIZE	  177 // 模块数最大值177*177

// 非数据码字标记
#define NOT_DATA 111

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ContentDecoder  
{
public:
	ContentDecoder();
	virtual ~ContentDecoder();
	
public:
	int m_nCodeSize;//大小
	int m_nLevel;//纠错级别
	int m_nVersion;//版本
	int m_nMaskingNo;//掩膜方式
	int m_nVersionGroup;//版本组号1~9 10~26 27~40
	
	QString m_strData;//数据

	unsigned char m_CodeData[MAX_MODULESIZE][MAX_MODULESIZE]; // 存储条形码位数据

	int m_ncAllCodeWord; //总码字数
	int m_ncDataCodeWord; //数据码字数
	int m_ncRSCodeWord; //RS纠错码字数
	unsigned char m_byAllCodeWord[MAX_ALLCODEWORD]; //码字数据
	unsigned char m_byDataCodeWord[MAX_DATACODEWORD]; //数据码字数据
	unsigned char m_byRSWork[MAX_CODEBLOCK];//RS码字数据
	int m_nIndex;//位流中当前位置

public:
	bool DecodeData(int nCodeSize,int nVersion,unsigned char CodeData[MAX_MODULESIZE][MAX_MODULESIZE]);
	
	//设置非数据标记
	void SetFinderPattern();
	void SetAlignmentPattern();
	void SetFormatPattern();
	void SetVersionPattern();

	int GetVersionInfo();//版本信息
	int GetFormatInfo();//格式信息

	void GetCodeWord();//获取码字
	void ParseDataCodeWord();//解析码字，翻译为数据
	unsigned short GetBitStream(int ncData);//获取位流
	char BinaryToAlphabet(unsigned char by);
	void CorrectDataBlocks();

};

#endif // !defined(AFX_DECODER_H__AC3D15D3_0D11_467B_B981_E73FD0B97EB7__INCLUDED_)
