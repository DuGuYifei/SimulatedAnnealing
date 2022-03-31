// SADlg.h : header file
//

#if !defined(AFX_SADLG_H__438856B2_C48B_477A_8281_7A2F42FF546D__INCLUDED_)
#define AFX_SADLG_H__438856B2_C48B_477A_8281_7A2F42FF546D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NCITIES 50             /*liczba miast - nie zmieniac!*/

/////////////////////////////////////////////////////////////////////////////
// CSADlg dialog

class CSADlg : public CDialog
{
// Construction
public:
	CSADlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSADlg)
	enum { IDD = IDD_SA_DIALOG };
	CStatic	m_CityFrame;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Draw();
	BOOL InitializeMap();
	double NewEnergy(int tr[NCITIES]);
	HICON m_hIcon;

	double	loc[NCITIES][2];			/*the map - coordinates of the cities*/
	int		route[NCITIES];				/*current route*/
	double	T;							/*current temperature*/
	double	energy;						/*energy of the current solution*/
	long	maxTrialN;					/*maximum number of trials in one iteration*/
	long	maxAcceptN;					/*maximum number of accepted trials*/
	double	stopTolerance;				/*the algorithm should stop when the difference between the best and the worst soulution at the current temperature falls below this value*/
	double	minE, maxE;					/*maximum and minimum energy in one iteration*/

	CDC m_memDC;
	CBitmap m_bmp;
	CFont m_font;
	CPen m_pen;
	CBrush m_brush;

	// Generated message map functions
	//{{AFX_MSG(CSADlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SADLG_H__438856B2_C48B_477A_8281_7A2F42FF546D__INCLUDED_)
