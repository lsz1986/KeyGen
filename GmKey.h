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
	CString m_strLicMac; //���������ַ
	CString m_strLicCpuId; //CPUID
	CString m_strLicHDSerial; //Ӳ�����к�
	int m_nLicMainboardSN; //�������к�

	CTime m_ctimeLicStart; //��ʼʱ��
	CTime m_ctimeLicEnd; //��ֹʱ��
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
