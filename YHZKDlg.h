// YHZKDlg.h : header file
//

#if !defined(AFX_OWNDRAWDLG_H__9598C99F_D1DC_47DC_B1F3_2E5892337204__INCLUDED_)
#define AFX_OWNDRAWDLG_H__9598C99F_D1DC_47DC_B1F3_2E5892337204__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CYHZKDlg dialog
#include "BtnST.h"
#include "ShadeButtonST.h"

#define MonMax		(12*15+1)
#define YearMax		15

class CYHZKDlg : public CDialog
{
// Construction
public:
	CYHZKDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CYHZKDlg)
	enum { IDD = IDD_OWNDRAW_DIALOG };
	CShadeButtonST	m_btnGetSn;
	CShadeButtonST	m_btnGenSTM32Code;
	CShadeButtonST	m_btnCalcPw;
	CShadeButtonST	m_btnOpenLic;
	CShadeButtonST	m_btnKeyEdit;
	CShadeButtonST	m_btnKeySave;
	int		m_nAuthDay;
	CString	m_strCustomerName;
	CString	m_strDateStart;
	CString	m_strDateEnd;
	CString	m_strMainBoardSN;
	int		m_nRandPw;
	int		m_nNum1;
	int		m_nNum2;
	CString	m_strV2Code;
	CString	m_strV2Sn;
	CTime	m_timeV2EndTime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYHZKDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	int m_width_icon;
	int m_heigh_icon;
	// Generated message map functions
	//{{AFX_MSG(CYHZKDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnBtnOpenLic();
	afx_msg void OnBtnKeyEdit();
	afx_msg void OnBtnKeySave();
	afx_msg void OnKillfocusEditDay();
	virtual void OnOK();
	afx_msg void OnCalcpw();
	afx_msg void OnGenV2Code();
	afx_msg void OnGetV2Sn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_nV2McuCode;
	unsigned int m_nV2LastUseTime; //最后一次使用时间
	unsigned int m_nV2EndTime;
	unsigned int m_nV2CheckSum; //校验码
	
	int ReadMbV2Sn(CString str);
	int ReadMbV2RegCode(CString str);
	
	void OnKillSelf();

	CRect m_closebtn;
	CBitmap m_closebitmap;
	CSize m_closesize;
	CPoint m_picon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNDRAWDLG_H__9598C99F_D1DC_47DC_B1F3_2E5892337204__INCLUDED_)
