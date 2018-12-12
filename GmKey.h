// GmKey.h: interface for the CGmKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMKEY_H__7E217A88_0D20_4474_807B_7FA3C5C195A3__INCLUDED_)
#define AFX_GMKEY_H__7E217A88_0D20_4474_807B_7FA3C5C195A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGmKey  
{
public:
	CString m_strLicMac; //网卡物理地址
	CString m_strLicCpuId; //CPUID
	CString m_strLicHDSerial; //硬盘序列号
	int m_nLicMainboardSN; //主板序列号

	CTime m_ctimeLicStart; //起始时间
	CTime m_ctimeLicEnd; //终止时间
	CTime m_ctimeLicBorn;
	int m_nLicAuthDay;
	CString	m_strLicCustomerName;

public:
	BOOL OnReadGmLicFile(CString strFileName);
	BOOL OnWriteKeyFile(CString strFileName);
	int OnReadKeyFile(CString strFileName);
	CGmKey();
	virtual ~CGmKey();

};

#endif // !defined(AFX_GMKEY_H__7E217A88_0D20_4474_807B_7FA3C5C195A3__INCLUDED_)
