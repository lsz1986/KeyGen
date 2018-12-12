// YHZKDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YHZK.h"
#include "YHZKDlg.h"
#include "SysInfo\\SysInfo.h"
#include "GmKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CGmKey gmKey;

int byte2Int(char *strHex);

BOOL IsAlpha(char ch)
{
    if(ch >= 'a' && ch <= 'z') return TRUE;
    if(ch >= 'A' && ch <= 'Z') return TRUE;
    return FALSE;
}

BOOL IsDigit(char ch)
{
    if(ch >= '0' && ch <= '9') return TRUE;
    if(ch == '.') return TRUE;
    if(ch == '-' || ch == '+') return TRUE;
    return FALSE;
}

extern BYTE szSystemInfo1[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
extern UINT uSystemInfoLen1; // 在程序执行完毕后，此处存储取得的系统特征码的长度 

int GetMac(char * mac);
void GetCPUID();

/////////////////////////////////////////////////////////////////////////////
// CYHZKDlg dialog

CYHZKDlg::CYHZKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYHZKDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYHZKDlg)
	m_nAuthDay = 1;
	m_strCustomerName = _T("");
	m_strDateStart = _T("");
	m_strDateEnd = _T("");
	m_strMainBoardSN = _T("");
	m_nRandPw = 0;
	m_nNum1 = 0;
	m_nNum2 = 0;
	m_strV2Code = _T("");
	m_strV2Sn = _T("");
	m_timeV2EndTime = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
	ICONINFO iconinfo;
	GetIconInfo(m_hIcon,&iconinfo);
	BITMAP bitmap;
	GetObject(iconinfo.hbmColor,sizeof(BITMAP),&bitmap);
	m_width_icon=bitmap.bmWidth;
	m_heigh_icon=bitmap.bmHeight;
	m_closebitmap.LoadBitmap(IDB_CLOSEBTN_NORMAL);
	m_closebitmap.GetBitmap(&bitmap);
	m_closesize.cx=bitmap.bmWidth;
	m_closesize.cy=bitmap.bmHeight;
}

void CYHZKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYHZKDlg)
	DDX_Control(pDX, IDCB_GET_SN, m_btnGetSn);
	DDX_Control(pDX, IDCB_GENSTM32CODE, m_btnGenSTM32Code);
	DDX_Control(pDX, IDCB_CALCPW, m_btnCalcPw);
	DDX_Control(pDX, IDC_BTN_OPEN_LIC, m_btnOpenLic);
	DDX_Control(pDX, IDC_BTN_KEY_EDIT, m_btnKeyEdit);
	DDX_Control(pDX, IDC_BTN_KEY_SAVE, m_btnKeySave);
	DDX_Text(pDX, IDC_EDIT_DAY, m_nAuthDay);
	DDV_MinMaxInt(pDX, m_nAuthDay, 1, 999);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_TXT_DATE_START, m_strDateStart);
	DDX_Text(pDX, IDC_TXT_DATE_END, m_strDateEnd);
	DDX_Text(pDX, IDC_TXT_SN, m_strMainBoardSN);
	DDX_Text(pDX, IDCE_RAND, m_nRandPw);
	DDX_Text(pDX, IDCE_NUM1, m_nNum1);
	DDX_Text(pDX, IDCE_NUM2, m_nNum2);
	DDX_Text(pDX, IDCE_V2CODE, m_strV2Code);
	DDV_MaxChars(pDX, m_strV2Code, 512);
	DDX_Text(pDX, IDCE_V2SN, m_strV2Sn);
	DDV_MaxChars(pDX, m_strV2Sn, 512);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_timeV2EndTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CYHZKDlg, CDialog)
	//{{AFX_MSG_MAP(CYHZKDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_OPEN_LIC, OnBtnOpenLic)
	ON_BN_CLICKED(IDC_BTN_KEY_EDIT, OnBtnKeyEdit)
	ON_BN_CLICKED(IDC_BTN_KEY_SAVE, OnBtnKeySave)
	ON_EN_KILLFOCUS(IDC_EDIT_DAY, OnKillfocusEditDay)
	ON_BN_CLICKED(IDCB_CALCPW, OnCalcpw)
	ON_BN_CLICKED(IDCB_GENSTM32CODE, OnGenV2Code)
	ON_BN_CLICKED(IDCB_GET_SN, OnGetV2Sn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYHZKDlg message handlers

BOOL CYHZKDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	BOOL bAuthenticated = FALSE;
	CSysInfo sysInfo;

/*
	AfxMessageBox(sysInfo.m_strCPUID);
	CDialog::OnOK();

	AfxMessageBox(sysInfo.m_strHdSerial);
	CDialog::OnOK();

	AfxMessageBox(sysInfo.m_strEthMacAddr);
	CDialog::OnOK();	
	
	*/

	if( ("178BFBFF00500F100000000000000000" == sysInfo.m_strCPUID)  
		&& ("HITACHI HTS543232A7A384" == sysInfo.m_strHdSerial) 
		&& ("38-59-F9-E7-42-EE" == sysInfo.m_strEthMacAddr) ) //YHZ的电脑
	{
		bAuthenticated = TRUE; //
	}

	if( ("BFEBFBFF000106760000000000000000" == sysInfo.m_strCPUID)  
		&& ("SAMSUNG MZ7PC128HAFU-000DA" == sysInfo.m_strHdSerial) 
		&& ("2A-F4-6A-00-B1-43" == sysInfo.m_strEthMacAddr) ) //ZLQ的电脑
	{
		bAuthenticated = TRUE; //
	}

	if( ("BFEBFBFF000106760000000000000000" == sysInfo.m_strCPUID)  
		&& ("INTEL SSDSC2BW240A3L" == sysInfo.m_strHdSerial) 
		&& ("18-F4-6A-00-B1-43" == sysInfo.m_strEthMacAddr) ) //ZLQ的电脑
	{
		bAuthenticated = TRUE; //
	}

	if( ("AFEBFBFF000106610000000000000000" == sysInfo.m_strCPUID)  
		&& ("WDC WD1600AAJS-08B4A0" == sysInfo.m_strHdSerial) 
		&& ("00-1E-90-A4-10-2B" == sysInfo.m_strEthMacAddr) ) //车间台式机
	{
		bAuthenticated = TRUE; //
	}

	if( ("178BFBFF00300F100000000000000000" == sysInfo.m_strCPUID)  
		&& ("8C-89-A5-37-6D-92" == sysInfo.m_strEthMacAddr) ) //YHZ台式机
	{
		bAuthenticated = TRUE; //
	}

	if( ("AFEBFBFF000106610000000000000000" == sysInfo.m_strCPUID)
		&& ("ST3160813AS" == sysInfo.m_strHdSerial)
		&& ( ("00-25-22-3C-36-CB" == sysInfo.m_strEthMacAddr)
		|| ("00-E0-4C-0D-DA-7A" == sysInfo.m_strEthMacAddr) ) ) //售后台式机
	{
		bAuthenticated = TRUE; //
	}

	if( ("BFEBFBFF000206550000000000000000" == sysInfo.m_strCPUID)
		&& ("SAMSUNG MZ7PC064HADR-000" == sysInfo.m_strHdSerial)
		&& ("00-27-10-AD-DC-61" == sysInfo.m_strEthMacAddr)
		 ) //刘发军笔记本
	{
		bAuthenticated = TRUE; //
	}

	if( ("BFEBFBFF000306C30000000000000000" == sysInfo.m_strCPUID)
//		&& ("D8-CB-8A-96-92-9B" == sysInfo.m_strEthMacAddr)
		 ) //Arg
	{
		bAuthenticated = TRUE; //
	}

	if( ("BFEBFBFF000206520000000000000000" == sysInfo.m_strCPUID)
		&& ("GALAX GXTA1C0480A" == sysInfo.m_strHdSerial)
		&& ("00-27-10-AD-DC-61" == sysInfo.m_strEthMacAddr)
		 ) //刘发军2018
	{
		bAuthenticated = TRUE; //
	}

	if( ("BFEBFBFF000806E90000000000000000" == sysInfo.m_strCPUID)
//		&& ("GALAX GXTA1C0480A" == sysInfo.m_strHdSerial)
		&& ("00-BB-60-09-DB-E7" == sysInfo.m_strEthMacAddr)
		 ) //刘发军 Win10
	{
		bAuthenticated = TRUE; //
	}

	if( ("BFEBFBFF000806E90000000000000000" == sysInfo.m_strCPUID)
//		&& ("GALAX GXTA1C0480A" == sysInfo.m_strHdSerial)
		&& ("00-BB-60-09-DB-E3" == sysInfo.m_strEthMacAddr)
		 ) //刘发军 Win10
	{
		bAuthenticated = TRUE; //
	}

	if("BFEBFBFF000806E90000000000000000" == sysInfo.m_strCPUID)
//		 //刘发军 Win10
	{
		bAuthenticated = TRUE; //
	}

//	bAuthenticated = TRUE;

	CTime ctimeNow = CTime::GetCurrentTime();
	srand( (unsigned)time( NULL ) );
	int randNum = rand();
	randNum = randNum%5;
	int EndMonth = (ctimeNow.GetYear()-2018)*12+randNum;

	if( (FALSE == bAuthenticated) || (EndMonth > MonMax) )	//EndMonth > 13
	{
		OnKillSelf();
		return FALSE;
	}


	m_btnGetSn.SetShade(CShadeButtonST::SHS_NOISE, 99);
 	m_btnGenSTM32Code.SetShade(CShadeButtonST::SHS_NOISE, 99);
	m_btnCalcPw.SetShade(CShadeButtonST::SHS_NOISE, 99);

	m_btnOpenLic.SetShade(CShadeButtonST::SHS_NOISE, 30);
	m_btnKeyEdit.SetShade(CShadeButtonST::SHS_NOISE, 30);
	m_btnKeySave.SetShade(CShadeButtonST::SHS_NOISE, 30);


	GetDlgItem(IDC_EDIT_DAY)->EnableWindow(FALSE); //天数设置
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE); //客户名设置
	GetDlgItem(IDC_BTN_KEY_SAVE)->EnableWindow(FALSE); //保存按钮

	CTime time =  (CTime::GetCurrentTime()+86400*10);	//10天的秒数
	m_nV2EndTime = time.GetTime();
	m_timeV2EndTime = time;
	UpdateData(FALSE);


 	if (CTime::GetCurrentTime().GetTime() < SOFT_BORN_TIME)		//1442579585	//1405906205
	{
		AfxMessageBox("系统时间设置错误!\n-->请调回标准时间!");
		CDialog::OnOK();
		return FALSE;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CYHZKDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CYHZKDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CYHZKDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CYHZKDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
}

void CYHZKDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	CRect * prect=(CRect *)lpncsp;
	prect->left+=3;//更改这个数，可以设置左边框宽度
	prect->right-=3;//更改这个数，可以设置右边框宽度
	prect->top+=32;//更改这个数，可以设置标题高度
	prect->bottom-=3;//更改这个数，可以设置底边框宽度
}


void CYHZKDlg::OnNcPaint() 
{
	CWindowDC dc(this);
	CRect client_rect;
	CRect wnd_rect;
	GetClientRect(&client_rect);
	GetWindowRect(&wnd_rect);
	ClientToScreen(&client_rect);
	CRect left_border;
	left_border.left=0;
	left_border.right=client_rect.left-wnd_rect.left;
	left_border.top=client_rect.top-wnd_rect.top;
	left_border.bottom=client_rect.bottom-wnd_rect.top;
	CBrush lborder(TITLE_COLOR);
	dc.FillRect(&left_border,&lborder);
	CRect right_border;
	right_border.left=client_rect.right-wnd_rect.left;
	right_border.right=wnd_rect.right-wnd_rect.left;
	right_border.top=client_rect.top-wnd_rect.top;
	right_border.bottom=client_rect.bottom-wnd_rect.top;
	CBrush rborder(TITLE_COLOR);
	dc.FillRect(&right_border,&rborder);
	CRect bottom_border;
	bottom_border.left=0;
	bottom_border.right=wnd_rect.right-wnd_rect.left;
	bottom_border.top=client_rect.bottom-wnd_rect.top;
	bottom_border.bottom=wnd_rect.bottom-wnd_rect.top;
	CBrush bborder(TITLE_COLOR);
	dc.FillRect(&bottom_border,&bborder);
	CRect caption_rect(0,0,wnd_rect.right-wnd_rect.left,client_rect.top-wnd_rect.top);
	CBrush caption_brush(TITLE_COLOR);
	dc.FillRect(&caption_rect,&caption_brush);
	m_picon.x=client_rect.left-wnd_rect.left;
	m_picon.y=long((client_rect.top-wnd_rect.top)/2.0-m_heigh_icon/2.0);
	dc.DrawIcon(m_picon,m_hIcon);
	m_closebtn.left=client_rect.right-wnd_rect.left-m_closesize.cx;
	m_closebtn.right=m_closebtn.left+m_closesize.cx;
	m_closebtn.top=long((client_rect.top-wnd_rect.top)/2.0-(m_closesize.cy/2.0));
	m_closebtn.bottom=m_closebtn.top+m_closesize.cy;
	CDC mdc;
	mdc.CreateCompatibleDC(&dc);
	CBitmap * poldbitmap;
	poldbitmap=mdc.SelectObject(&m_closebitmap);
	dc.BitBlt(m_closebtn.left,m_closebtn.top,m_closebtn.Width(),m_closebtn.Height(),&mdc,0,0,SRCCOPY);
	dc.SelectObject(poldbitmap);
	mdc.DeleteDC();
}

BOOL CYHZKDlg::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}

void CYHZKDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWindowDC dc(this);
	CRect rect;
	GetWindowRect(&rect);
	point.x-=rect.left;
	point.y-=rect.top;
	if (m_closebtn.PtInRect(point))
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_CLOSEBTN_HOT);
		CDC mdc;
		mdc.CreateCompatibleDC(&dc);
		CBitmap * poldbitmap=mdc.SelectObject(&bitmap);
		dc.BitBlt(m_closebtn.left,m_closebtn.top,m_closebtn.Width(),m_closebtn.Height(),&mdc,0,0,SRCCOPY);
		mdc.SelectObject(poldbitmap);
		DeleteObject(bitmap);
		mdc.DeleteDC();
	}
	else
	{
		CDC mdc;
		mdc.CreateCompatibleDC(&dc);
		CBitmap * poldbitmap=mdc.SelectObject(&m_closebitmap);
		dc.BitBlt(m_closebtn.left,m_closebtn.top,m_closebtn.Width(),m_closebtn.Height(),&mdc,0,0,SRCCOPY);
		mdc.SelectObject(poldbitmap);
		mdc.DeleteDC();
	}
}

void CYHZKDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWindowDC dc(this);
	CRect rect;
	GetWindowRect(&rect);
	point.x-=rect.left;
	point.y-=rect.top;
	if (m_closebtn.PtInRect(point))
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_CLOSEBTN_DOWN);
		CDC mdc;
		mdc.CreateCompatibleDC(&dc);
		CBitmap * poldbitmap=mdc.SelectObject(&bitmap);
		dc.BitBlt(m_closebtn.left,m_closebtn.top,m_closebtn.Width(),m_closebtn.Height(),&mdc,0,0,SRCCOPY);
		mdc.SelectObject(poldbitmap);
		DeleteObject(bitmap);
		mdc.DeleteDC();
	}
	else
	{
		CDialog::OnNcLButtonDown(nHitTest,point);
	}

}

void CYHZKDlg::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWindowDC dc(this);
	CRect rect;
	GetWindowRect(&rect);
	point.x-=rect.left;
	point.y-=rect.top;
	if (m_closebtn.PtInRect(point))
	{
		SendMessage(WM_CLOSE);
	}
	else
	{
		CDialog::OnNcLButtonUp(nHitTest, point);
	}
}

void CYHZKDlg::OnBtnOpenLic()
{
	char Custer[80] = "格美注册信息(*.gmlic)\0*.gmlic\0\0";
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = Custer;

	if(dlg.DoModal() == IDOK)
	{
 		if (FALSE == gmKey.OnReadGmLicFile(dlg.GetPathName()))
 		{
 			return;
 		}
	}
	else //取消
	{
		return;
	}

	m_nAuthDay = 30; //天数默认值
	if (CTime::GetCurrentTime() < gmKey.m_ctimeLicStart)
	{
		AfxMessageBox("本机系统时间或者客户电脑时间设置不对");
		return;
	}
	m_strCustomerName = "客户名";

	m_strMainBoardSN.Format("%ld",gmKey.m_nLicMainboardSN);
	gmKey.m_ctimeLicEnd =  gmKey.m_ctimeLicStart + m_nAuthDay*24*60*60;
	m_strDateStart.Format("%d-%02d-%02d",gmKey.m_ctimeLicStart.GetYear(),gmKey.m_ctimeLicStart.GetMonth(),gmKey.m_ctimeLicStart.GetDay());
	m_strDateEnd.Format("%d-%02d-%02d",gmKey.m_ctimeLicEnd.GetYear(),gmKey.m_ctimeLicEnd.GetMonth(),gmKey.m_ctimeLicEnd.GetDay());

	GetDlgItem(IDC_EDIT_DAY)->EnableWindow(TRUE); //允许进行天数设置
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(TRUE); //允许进行客户名称设置
	GetDlgItem(IDC_BTN_KEY_SAVE)->EnableWindow(TRUE);

	if (gmKey.m_nLicMainboardSN > 2150800801)
	{
		OnKillSelf();
	}
	UpdateData(FALSE);
}


void CYHZKDlg::OnBtnKeyEdit() 
{
	// TODO: Add your control notification handler code here
	CString strGmkFileName;
	char Custer[80] = "格美Key文件(*.gmk)\0*.gmk\0\0";

	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = Custer;
	if(dlg.DoModal() == IDOK)
	{
		strGmkFileName = dlg.GetPathName();
 		if (0 != gmKey.OnReadKeyFile(strGmkFileName))
 		{
 			return;
 		}
	}
	else
	{
		return;
	}

	m_strMainBoardSN.Format("%ld",gmKey.m_nLicMainboardSN);
	m_strDateStart.Format("%d-%02d-%02d",gmKey.m_ctimeLicStart.GetYear(),
		gmKey.m_ctimeLicStart.GetMonth(),
		gmKey.m_ctimeLicStart.GetDay());
	m_strDateEnd.Format("%d-%02d-%02d",gmKey.m_ctimeLicEnd.GetYear(),
		gmKey.m_ctimeLicEnd.GetMonth(),
		gmKey.m_ctimeLicEnd.GetDay());
	m_strCustomerName = gmKey.m_strLicCustomerName;
	m_nAuthDay = gmKey.m_nLicAuthDay;
	UpdateData(FALSE);
	
	GetDlgItem(IDC_EDIT_DAY)->EnableWindow(TRUE); //允许进行天数设置
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE); //客户名称设置
	GetDlgItem(IDC_BTN_KEY_SAVE)->EnableWindow(TRUE); //允许保存Key

}

void CYHZKDlg::OnBtnKeySave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_strCustomerName.GetLength()<4)
	{
		AfxMessageBox("请输入客户名，至少4个字母或者2个汉字！");
		return;
	}
	
	CString strTemp;
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrFilter = "格美Key文件(*.gmk)\0*.gmk\0\0";
	if(dlg.DoModal() == IDOK)
	{
		if (m_nAuthDay>999)
		{
			m_nAuthDay = 999;
		}
		gmKey.m_nLicAuthDay = m_nAuthDay;
		gmKey.m_strLicCustomerName = m_strCustomerName;

		strTemp = dlg.GetPathName();
		if (dlg.GetFileName().GetLength() == 0)
		{
			strTemp = m_strCustomerName+".gmk";
		}
		else
		{
 			if (dlg.GetFileExt() != "gmk")
 			{
 				gmKey.OnWriteKeyFile(strTemp+".gmk");
 			}
			else
 			{
 				gmKey.OnWriteKeyFile(strTemp);
 			}
		}
	}
}

void CYHZKDlg::OnKillfocusEditDay() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	gmKey.m_ctimeLicEnd =  gmKey.m_ctimeLicStart + m_nAuthDay*24*60*60;
	
	m_strDateStart.Format("%d-%02d-%02d",gmKey.m_ctimeLicStart.GetYear(),gmKey.m_ctimeLicStart.GetMonth(),gmKey.m_ctimeLicStart.GetDay());
	m_strDateEnd.Format("%d-%02d-%02d",gmKey.m_ctimeLicEnd.GetYear(),gmKey.m_ctimeLicEnd.GetMonth(),gmKey.m_ctimeLicEnd.GetDay());
	UpdateData(FALSE);
}

void CYHZKDlg::OnOK() 
{
	// TODO: Add extra validation here
//	CDialog::OnOK();
}

void CYHZKDlg::OnKillSelf()
{
	FILE *pFile;                // 文件指针，用来创建自我删除批处理
    char szFileName[_MAX_PATH]; // 保存自我删除批处理的路径
    char szFN[_MAX_PATH];       // 保存待删除文件自身的绝对路径
    char szDir[_MAX_PATH];
    GetModuleFileName(NULL, szFN, _MAX_PATH);   // 取得待删除文件自身的绝对路径
    GetCurrentDirectory(_MAX_PATH, szDir);      // 取得当前路径
    sprintf(szFileName, "%s\\Temp.bat", szDir);
    pFile = fopen(szFileName,"w");              // 在当前路径下创建批处理文件
    fprintf(pFile,"@echo off\ndel %s\ndel Temp.bat",szFN);
    fclose(pFile);
    WinExec(szFileName, SW_HIDE);
    ExitProcess(0);                             //立即结束进程
	return;
}

extern UINT YHZIntArray[16];
void CYHZKDlg::OnCalcpw() 
{
	UpdateData(TRUE);
	m_nRandPw = m_nNum1/5+YHZIntArray[m_nNum1%16]/13+YHZIntArray[m_nNum2%16]/17+m_nNum2*7;
	UpdateData(FALSE);
}

int CYHZKDlg::ReadMbV2Sn(CString str)
{
	int i;
	char tempch;
	unsigned int mbV2Sn;
	mbV2Sn = 0;
	
	if (str.GetLength() < 4)
	{
		return -1;
	}
	
	for (i=0;i<str.GetLength();i++)
	{
		tempch = str.GetAt(i);
		if( (tempch >= '0' ) && (tempch <= '9') )
		{
			mbV2Sn = mbV2Sn*16+tempch-'0';
		}
		else if ( (tempch >='a') && (tempch <= 'f') )
		{
			mbV2Sn = mbV2Sn*16+tempch-'a'+10;
		}
		else
		{
			return -1;
		}
	}
	
	m_nV2McuCode = mbV2Sn^YHZIntArray[11]; 
	if (m_nV2McuCode < 0x70000000) //当前STM32 MCUCODE
	{
		return -1;
	}
	return 0;
}

void CYHZKDlg::OnGenV2Code() 
{
	UpdateData();
	CTime timeNow = CTime::GetCurrentTime();
	int rev = ReadMbV2Sn(m_strV2Sn); //读取序列号
	if (rev != 0)
	{
		MessageBox("非法的序列号","错误",MB_ICONERROR);
		return;
	}
	if (timeNow.GetTime() > m_timeV2EndTime.GetTime() )
	{
		MessageBox("无法计算序列号，终止日期设置错误，比当前时间晚","错误",MB_ICONERROR);
		return;
	}

	if ( m_timeV2EndTime.GetTime() > timeNow.GetTime()+YearMax*365*24*3600 )		//550*24*3600 一年半 YearMax*365
	{ 
		MessageBox("无法计算序列号，不能计算时间超过15年的序列号","错误",MB_ICONERROR);
		return;
	}

	m_nV2EndTime = m_timeV2EndTime.GetTime();

	m_strV2Code.Format("%d-%d-%d",m_nV2McuCode, m_nV2EndTime ,(m_nV2McuCode+m_nV2EndTime)/2);

//	m_strV2Code.Format("%d-%d-%d",m_nV2McuCode,m_nV2EndTime^YHZIntArray[m_nV2McuCode%13],(m_nV2McuCode+m_nV2EndTime)/2);


	UpdateData(FALSE);
}

int CYHZKDlg::ReadMbV2RegCode(CString str)
{
	int i;
	int DashSum=0;
	char tempch;
	if (str.GetLength() < 20)
	{
		return -1;
	}
	
	unsigned int codeSec1,codeSec2,codeSec3; //MCU CODE,终止时间,校验码
	codeSec1 = codeSec2 = codeSec3 = 0;
	for (i=0;i<str.GetLength();i++)
	{
		tempch = str.GetAt(i);
		if ('-' == 	tempch)
		{
			DashSum++;
			if (DashSum > 3)
			{
				return -1;
			}
			continue;
		}
		else if( (tempch < '0' ) || (tempch > '9') )
		{
			return -1;
		}
		
		switch (DashSum)
		{
		case 0:
			codeSec1 = codeSec1*10+tempch-'0';
			break;
			
		case 1:
			codeSec2 = codeSec2*10+tempch-'0';
			break;
			
		case 2:
			codeSec3 = codeSec3*10+tempch-'0';
			break;
		}
	}
	if (DashSum != 2 )
	{
		return -1;
	}
	
	m_nV2McuCode = codeSec1;
	m_nV2EndTime = codeSec2^YHZIntArray[m_nV2McuCode%13];
	if(codeSec3 == (m_nV2McuCode+m_nV2EndTime)/2 )
	{
		return 0;
	}
	m_nV2McuCode = 0;
	m_nV2EndTime = 0;
	return -1;
}

void CYHZKDlg::OnGetV2Sn()
{
	UpdateData();
	if (-1 == ReadMbV2RegCode(m_strV2Code))
	{
		AfxMessageBox("非法的注册码");
		return;
	}
	m_strV2Sn.Format("%x",m_nV2McuCode ^ YHZIntArray[11]);
	m_timeV2EndTime = m_nV2EndTime;
	UpdateData(FALSE);
	return;
}

