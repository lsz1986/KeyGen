// GmKey.cpp: implementation of the CGmKey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YHZK.h"
#include "GmKey.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//9λ���� ��� 23 27 5E 62
UINT YHZIntArray[16] = {
	214072011,/*0*/
		180416531,/*1*/
		188092317,/*2*/
		122988619,/*3*/
		142797956,/*4*/
		366543210,/*5*/
		305219805,/*6*/
		111022060,/*7*/
		151032792,/*8*/
		187654328,/*9*/
		169227610,/*10*/
		168688886,/*11*/
		121314872,/*12*/
		151327678,/*13*/
		141526598,/*14*/
		589782626/*15*/
};

char printtoi(char c)
{
	if( ('0'<=c) && (c<='9') )
	{
		return (c-'0');
	}
	if( ('a'<=c) && (c<='z') )
	{
		return (10+c-'a');
	}
	if( ('A'<=c) && (c<='Z') )
	{
		return (40+c-'A');
	}
	return 0;
}


char a16toi(char c)
{
	char val;
	switch (c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		val = c -'0';
		break;
		
	case 'a':
	case 'A':
		val=10;
		break;
	case 'b':
	case 'B':
		val=11;
		break;
	case 'c':
	case 'C':
		val=12;
		break;
	case 'd':
	case 'D':
		val=13;
		break;
	case 'e':
	case 'E':
		val=14;
		break;
	case 'f':
	case 'F':
		val=15;
		break;
	default:
		val=0;
		break;
	}
	return val;
}

int byte2Int(char *strHex)
{
	return a16toi(*(strHex))*16+a16toi(*(strHex+1));
}

CGmKey::CGmKey()
{

}

CGmKey::~CGmKey()
{

}

BOOL CGmKey::OnReadGmLicFile(CString strFileName)
{
	CFile fileGmlic;
	int i;
	char tempBuff[120];
	char rdBuff[1024];
	char m_n1;
	char m_n2;
	char m_n3;
	char m_n4;
	char m_n5;
	char m_n6;
	char m_n7;
	
	if(!fileGmlic.Open(strFileName,CFile::modeRead))
	{
		AfxMessageBox("���ļ�ʧ��");
		return FALSE;
	}
	int nFileLen=fileGmlic.Read(rdBuff,1024);
	if (nFileLen<32)
	{
		AfxMessageBox("�Ƿ���Lic�ļ�");
		return FALSE;
	}
	
	m_n1 = byte2Int(&rdBuff[0]);
	m_n2 = byte2Int(&rdBuff[2]);
	m_n3 = byte2Int(&rdBuff[4]);
	m_n4 = byte2Int(&rdBuff[6]);
	m_n5 = byte2Int(&rdBuff[8]);
	m_n6 = byte2Int(&rdBuff[10]);
	m_n7 = byte2Int(&rdBuff[12]);
	
	if (nFileLen != m_n3+m_n4+m_n5+m_n6+m_n7+14)
	{
		AfxMessageBox("�Ƿ���Lic�ļ�");
		return FALSE;
	}
	
	for (i=0; i< m_n3; i++)
	{
		tempBuff[i] = rdBuff[i+14];
	}
	tempBuff[i]=0;
	m_strLicMac = tempBuff;
	
	for (i=0; i< m_n4; i++)
	{
		tempBuff[i] = rdBuff[i+14+m_n3];
	}
	tempBuff[i]=0;
	m_strLicCpuId = tempBuff;
	
	for (i=0; i< m_n5; i++)
	{
		tempBuff[i] = rdBuff[i+14+m_n3+m_n4];
	}
	tempBuff[i]=0;
	m_strLicHDSerial = tempBuff;
	
	
	for (i=0; i< m_n6; i++)
	{
		tempBuff[i] = rdBuff[i+14+m_n3+m_n4+m_n5];
	}
	tempBuff[i]=0;
	
	m_nLicMainboardSN = atoi(tempBuff)^YHZIntArray[m_n2];
	
	
	for (i=0; i< m_n7; i++)
	{
		tempBuff[i] = rdBuff[i+14+m_n3+m_n4+m_n5+m_n6];
	}
	tempBuff[i]=0;
	
	m_ctimeLicStart = atoi(tempBuff)^YHZIntArray[m_n1];
	
	fileGmlic.Close();
	return TRUE;
}

BOOL CGmKey::OnWriteKeyFile(CString strFileName)
{
	char cEncryptNum;
	char cl1,cl2,cl3,cl4,cl5,cl6,cl7,cl8;
	CString strTemp;
	CString strBuf;

	m_ctimeLicBorn = CTime::GetCurrentTime(); //���򷢲�ʱ�� 1405906205
	if (m_ctimeLicBorn.GetTime() < SOFT_BORN_TIME)
	{
		AfxMessageBox("ϵͳʱ�����ô���!\n����ر�׼ʱ��!");
		return FALSE;
	}
	cEncryptNum = (m_ctimeLicBorn.GetTime()+m_nLicAuthDay)%16;

	strTemp.Format("%ld",(m_nLicMainboardSN^YHZIntArray[cEncryptNum]));
	cl1 = strTemp.GetLength();

	strTemp.Format("%ld",m_ctimeLicStart.GetTime()^YHZIntArray[cEncryptNum] );
	cl2 = strTemp.GetLength();

	strTemp.Format("%ld",m_ctimeLicEnd.GetTime()^YHZIntArray[cEncryptNum] );
	cl3 = strTemp.GetLength();

	cl4=m_strLicMac.GetLength();
	cl5=m_strLicCpuId.GetLength();
	cl6=m_strLicHDSerial.GetLength();
	cl7=m_strLicCustomerName.GetLength();

	strTemp.Format("%ld",m_ctimeLicBorn.GetTime()^YHZIntArray[cEncryptNum] );
	cl8 = strTemp.GetLength();

	CFile fileGmKey;
	fileGmKey.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);

	strBuf.Format("%02x%02x%02x%02x%02x%02x%02x%02x%02x",cEncryptNum,cl1,cl2,cl3,cl4,cl5,cl6,cl7,cl8);
	fileGmKey.Write(strBuf.GetBuffer(0),18);

//------1-2-3-4-8-5-6-7-----------------------------------
	strTemp.Format("%ld",(m_nLicMainboardSN^YHZIntArray[cEncryptNum]));
	fileGmKey.Write(strTemp.GetBuffer(0),strTemp.GetLength()); //�������к�

	strTemp.Format("%ld",m_ctimeLicStart.GetTime()^YHZIntArray[cEncryptNum] );
	fileGmKey.Write(strTemp.GetBuffer(0),strTemp.GetLength()); //��ʼʱ��

	strTemp.Format("%ld",m_ctimeLicEnd.GetTime()^YHZIntArray[cEncryptNum] ); 
	fileGmKey.Write(strTemp.GetBuffer(0),strTemp.GetLength()); //����ʱ��

	fileGmKey.Write(m_strLicMac.GetBuffer(0),m_strLicMac.GetLength()); //���������ַ

	strTemp.Format("%ld",m_ctimeLicBorn.GetTime()^YHZIntArray[cEncryptNum] );
	fileGmKey.Write(strTemp.GetBuffer(0),strTemp.GetLength()); //����Key��ʱ��

	fileGmKey.Write(m_strLicCpuId.GetBuffer(0),m_strLicCpuId.GetLength()); //CPUID
	fileGmKey.Write(m_strLicHDSerial.GetBuffer(0),m_strLicHDSerial.GetLength()); //Ӳ�����к�
	fileGmKey.Write(m_strLicCustomerName.GetBuffer(0),m_strLicCustomerName.GetLength()); //�ͻ�����
	strBuf.Format("%03d",m_nLicAuthDay); //��Ȩ����
	fileGmKey.Write(strBuf.GetBuffer(0),strBuf.GetLength());
	fileGmKey.Close();

	fileGmKey.Open(strFileName,CFile::modeReadWrite|CFile::typeBinary);
	char buff[1024];
	int nfilelen;
	int checksum;
	int i;
	checksum = 0;

	fileGmKey.Seek(0,SEEK_SET);
	nfilelen=fileGmKey.Read(buff,1024);

	for (i=0;i<nfilelen;i++)
	{
		if (i%11 <= 5)
		{
			checksum += printtoi(buff[i]);
		}
		else
		{
			checksum += buff[i]%13;
		}
	}

	checksum += YHZIntArray[checksum%16];
	fileGmKey.Seek(0,SEEK_END);
	strBuf.Format("%ld",checksum);
	fileGmKey.Write(strBuf.GetBuffer(0),strBuf.GetLength());
	fileGmKey.Close();
	return TRUE;
}

int CGmKey::OnReadKeyFile(CString strFileName)
{
	CFile fileGmk;
	int i;
	char tempBuff[120];
	char rdBuff[1024];
	char cEncryptNum;
	char cl1,cl2,cl3,cl4,cl5,cl6,cl7,cl8;
	if (!fileGmk.Open(strFileName,CFile::modeRead))
	{
		AfxMessageBox("��Key�ļ�ʧ��");
		return -1;
	}
	int nFileLen = fileGmk.Read(rdBuff,1024);
	if (nFileLen<40)
	{
		fileGmk.Close();
		return -1;
	}

	cEncryptNum = byte2Int(&rdBuff[0]); //(m_ctimeLicBorn.GetTime()+m_nAuthDay )%16;
	cl1 = byte2Int(&rdBuff[2]); //MainBoardSN���ܺ�ĳ���
	cl2 = byte2Int(&rdBuff[4]); //���ܺ����ʼʱ��ת�����ַ�����ĳ���
	cl3 = byte2Int(&rdBuff[6]); //���ܺ����ֹʱ��ת�����ַ����ĳ���

	cl4 = byte2Int(&rdBuff[8]); //���������ַ �ַ�������
	cl5 = byte2Int(&rdBuff[10]); //CPUID�ַ�������
	cl6 = byte2Int(&rdBuff[12]); //Ӳ�����к�
	cl7 = byte2Int(&rdBuff[14]); //�û��� �ַ�������
	cl8 = byte2Int(&rdBuff[16]); //����Key��ʱ����ܺ���ִ�����

	for (i=0; i< cl1; i++)
	{
		tempBuff[i] = rdBuff[i+18];
	}
	tempBuff[i]=0;
	m_nLicMainboardSN = atoi(tempBuff)^YHZIntArray[cEncryptNum]; //�������к�

	for (i=0; i< cl2; i++)
	{
		tempBuff[i] = rdBuff[i+18+cl1];
	}
	tempBuff[i]=0;
	m_ctimeLicStart = atoi(tempBuff)^YHZIntArray[cEncryptNum]; //��ʼʱ��

	for (i=0; i< cl3; i++)
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2];
	}
	tempBuff[i]=0;
	m_ctimeLicEnd = atoi(tempBuff)^YHZIntArray[cEncryptNum]; //����ʱ��

	for (i=0; i< cl4; i++) //���������ַ
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2+cl3];
	}
	tempBuff[i]=0;
	m_strLicMac = tempBuff;

	for (i=0; i< cl8; i++) //����Key��ʱ��
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2+cl3+cl4];
	}
	tempBuff[i]=0;
	m_ctimeLicBorn = atoi(tempBuff)^YHZIntArray[cEncryptNum];

	for (i=0; i< cl5; i++) //CPUID
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2+cl3+cl4+cl8];
	}
	tempBuff[i]=0;
	m_strLicCpuId = tempBuff;

	for (i=0; i< cl6; i++) //Ӳ�����к�
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2+cl3+cl4+cl5+cl8];
	}
	tempBuff[i]=0;
	m_strLicHDSerial = tempBuff;

	for (i=0; i< cl7; i++) //�ͻ�����
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2+cl3+cl4+cl5+cl6+cl8];
	}
	tempBuff[i]=0;
	m_strLicCustomerName = tempBuff;

	for (i=0; i< 3; i++) //��Ȩ������3λ��
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2+cl3+cl4+cl5+cl6+cl7+cl8];
	}
	tempBuff[i]=0;
	m_nLicAuthDay = atoi(tempBuff);

	int checksum,rdchecksum;
	checksum=0;
	int aa = nFileLen - (18+cl1+cl2+cl3+cl4+cl5+cl6+cl7+cl8+3);
	for (i=0; i<aa; i++) //У����
	{
		tempBuff[i] = rdBuff[i+18+cl1+cl2+cl3+cl4+cl5+cl6+cl7+cl8+3];
	}
	tempBuff[i]=0;
	rdchecksum = atoi(tempBuff);

	for (i=0;i<nFileLen-aa;i++)
	{
		if (i%11 <= 5)
		{
			checksum += printtoi(rdBuff[i]);
		}
		else
		{
			checksum += rdBuff[i]%13;
		}
	}
	checksum += YHZIntArray[checksum%16];

	CTime ctimeNow = CTime::GetCurrentTime();
	if (checksum != rdchecksum)
	{
		AfxMessageBox("�Ƿ���key");
		return -2;
	}
	if( (m_ctimeLicStart > m_ctimeLicEnd) || (m_ctimeLicStart > ctimeNow) )
	{
		AfxMessageBox("�Ƿ���key");
		return -2;
	}
	fileGmk.Close();
	return 0;
}

