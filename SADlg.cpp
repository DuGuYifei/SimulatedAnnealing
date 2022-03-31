// SADlg.cpp : implementation file
//

#include "stdafx.h"
#include "SA.h"
#include "SADlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSADlg dialog

CSADlg::CSADlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSADlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSADlg)
	DDX_Control(pDX, IDC_CITY_FRAME, m_CityFrame);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSADlg, CDialog)
	//{{AFX_MSG_MAP(CSADlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSADlg message handlers

BOOL CSADlg::OnInitDialog()
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
	
	
	srand((unsigned)time(NULL));

	CRect r;
	m_CityFrame.GetClientRect(&r);

	m_memDC.CreateCompatibleDC(GetDC());
	m_bmp.CreateCompatibleBitmap(GetDC(), r.Width(), r.Height()); 
	m_font.CreatePointFont(110, "Arial");
	m_pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	
	m_memDC.SelectObject(&m_bmp);
	m_memDC.SelectObject(&m_font);
	m_memDC.SelectObject(&m_pen);
	m_memDC.SelectObject(&m_brush);

	m_CityFrame.GetClientRect(&r);
	m_memDC.Rectangle(&r);

	m_memDC.SetBkMode(TRANSPARENT); 
	
	COLORREF oldTxtCol = (m_memDC.SetTextColor(RGB (0, 100, 10)));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSADlg::OnPaint() 
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
		CPaintDC dc(this); // device context for painting

		CRect r;
		m_CityFrame.GetWindowRect(&r);
		ScreenToClient(r);		

		dc.BitBlt(r.left, r.top, r.right - r.left, r.bottom - r.top, &m_memDC, 0, 0, SRCCOPY);
	
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSADlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/**************************************************************************}
{READING THE MAP FROM A FILE, SETTING THE INITIAL TEMPERATURE              }
{**************************************************************************/
BOOL CSADlg::InitializeMap()
{

	int i, j, k;
	
	int stop;

	CFile f;
	if (!f.Open("tsp-c.dat", CFile::modeRead | CFile::shareDenyWrite))
	{
		MessageBox("Please put the tsp-c.dat file in the directory containig the program.");
		return FALSE;
	}
	
	float fBuf;

	for (i = 0; i < NCITIES; i++)
	{
		f.Read(&fBuf, sizeof(float));
		loc[i][0] = (double)fBuf;
		f.Read(&fBuf, sizeof(float));
		loc[i][1] = (double)fBuf;
	};

	f.Close();
	for (j = 0; j < NCITIES; j++)
	{
		do
		{
			stop = 1;
			route[j] = rand() % NCITIES;
			k=0;
		 
			while (k<j)
			{
				if (route[k] == route[j]) stop = 0;
				k++;
			};
		}
		while (!stop);
	 };

	 T = 15.0;
	 energy = 0;

	 return TRUE;
}

/**************************************************************************}
{DRAWING THE CURRENT ROUTE, DISPLAYING THE CURRENT ENERGY                  }
{**************************************************************************/
void CSADlg::Draw()
{

	int i, x, y, x1, y1;
	char s[10];
	CString sEn = "Energy: ";

	CRect r;
	m_CityFrame.GetClientRect(&r);
	m_memDC.Rectangle(&r);

	if (energy == 0.0)
	{
		sEn = "Calculate the energy!";
	}
	else
	{
		_gcvt_s(s, sizeof(s), energy,5);
		sEn += CString(s);
	}

	m_memDC.TextOut(10, r.Height()-20, sEn);

	for (i = 0; i < NCITIES; i++)
	{
		x = 10 + (int)((r.Width()-20)*loc[i][0]);
		y = 10 + (int)((r.Height()-20)*loc[i][1]);
		m_memDC.Ellipse(x-5, y-5, x+5, y+5);
	}

	for (i = 0; i < NCITIES-1; i++)
	{
		x = 10 + (int)((r.Width()-20)*loc[route[i]][0]);
		y = 10 + (int)((r.Height()-20)*loc[route[i]][1]);
		x1 = 10 + (int)((r.Width()-20)*loc[route[i+1]][0]);
		y1 = 10 + (int)((r.Height()-20)*loc[route[i+1]][1]);
		m_memDC.MoveTo(x, y);
		m_memDC.LineTo(x1, y1);
	};

	x = 10 + (int)((r.Width()-20)*loc[route[NCITIES-1]][0]);
	y = 10 + (int)((r.Height()-20)*loc[route[NCITIES-1]][1]);
	x1 = 10 + (int)((r.Width()-20)*loc[route[0]][0]);
	y1 = 10 + (int)((r.Height()-20)*loc[route[0]][1]);
	m_memDC.MoveTo(x, y);
	m_memDC.LineTo(x1, y1);

	CDC* pDC = GetDC();
	m_CityFrame.GetWindowRect(&r);
	ScreenToClient(r);

	pDC->BitBlt(r.left, r.top, r.right - r.left, r.bottom - r.top, &m_memDC, 0, 0, SRCCOPY);

	ReleaseDC(pDC);
}

void CSADlg::OnStart() 
{

	if (!InitializeMap())
		return;
 
	Draw();

	maxTrialN=NCITIES*100;
	maxAcceptN=NCITIES*10;
	stopTolerance=0.005;
	
	minE=1e20;
	maxE=-1;

  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!}
  {PUT THE ANNEALING LOOP HERE                                               }
  {!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!}
  The map has been read from a file. NCITIES indicates the number of cities.
  The coordinates of the cities are stored in the table loc[NCITIES][2]. 
  Each row of this table contains the coordinates (x,y) of one city.

  Randomly generated route has been placed in route[NCITIES] table.
  A route is a sequence of integers identifying the cities and at the same time
  indicating the rows in 'loc' table (so that the coordinates of each city can be retrieved easily). 
  
  'Draw' procedure displays the route stored in 'route' variable and the value of 'energy' (when calculated).
  The initial temperature is set and stored in 'T' variable.
  
  The variables 'minE' and 'maxE' are supposed to represent the energy of the best and the worst solution 
  accepted during one iteration of the loop (i.e. at the same temperature).
  By checking the difference maxE-minE one can decide if the annealing should stop.
    
  The variables are defined in SADlg.h file.

  ---Simulated annealing algorithm---

  THe following steps should be repeated until (maxE-minE)/maxE falls below the value defined by 'stopTolerance'.
  
1. Set the counter of trials and the counter of accepted trials to 0.
2. Calculate the energy E of the current solution. 
3. Generate the new solution (e.g. by switching the cities in the route).
4. Calculate the energy En of the new solution.
5. Accept the new solution with probability defined as 1/(1+exp(dE/T)), where 
   (dE = En-E). In the case of the acceptance, increment the counter of accepted trials.
6. Update maxE and minE if neccessary.
7. Increment the counter of trials. If the counter of trials or the counter of accepted trials has reached the maximum value
   (maxTrialN, maxAcceptN), go to step 8. Otherwise go back to 3.
8. Update the contents of 'route' and 'energy' variables, so that they represent the last accepted route.
9. Call 'Draw' to visualize the current solution.
10.Lower the temperature.

  {!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	int TrialN = 0, AcceptN = 0;

	energy = NewEnergy(route);

	while ((maxE - minE) / maxE > stopTolerance)
	{
		minE = 1e20;
		maxE = -1;
		TrialN = 0; 
		AcceptN = 0;
		while (TrialN < maxTrialN && AcceptN < maxAcceptN)
		{
			int switchIndex1 = rand() % NCITIES;
			int switchIndex2 = rand() % NCITIES;
			int new_route[NCITIES];
			for (int i = 0;i < NCITIES;i++)
			{
				new_route[i] = route[i];
			}
			int middle = abs(switchIndex1 - switchIndex2)/2;
			int temp1 = switchIndex1 < switchIndex2 ? switchIndex1 : switchIndex2;
			int temp2 = switchIndex1 < switchIndex2 ? switchIndex2 : switchIndex1;
			for (int i = 0;i < middle;i++)
			{
				new_route[temp1 + i] = route[temp2 -i];
				new_route[temp2 - i] = route[temp1 + i];
			}
			double new_energy = NewEnergy(new_route);
			double randP = rand()/(double)RAND_MAX;
			if (randP < (1 / (1 + exp((new_energy - energy) / T))))
			{
				energy = new_energy;
				for (int i = 0;i < NCITIES;i++)
				{
					route[i] = new_route[i];
				}
				minE = min(minE, energy);
				maxE = max(maxE, energy);
				AcceptN = AcceptN + 1;
			}
			TrialN = TrialN + 1;
			//Draw();
		}
		T = 0.5 * T;
		Draw();
	}
	

}

double CSADlg::NewEnergy(int tr[NCITIES])
{
	double new_energy = 0;
	for (int i = 1;i < NCITIES;i++)
	{
		new_energy += sqrt((loc[tr[i]][0] - loc[tr[i - 1]][0]) * (loc[tr[i]][0] - loc[tr[i - 1]][0]) + (loc[tr[i]][1] - loc[tr[i - 1]][1]) * (loc[tr[i]][1] - loc[tr[i - 1]][1]));
	}
	new_energy += sqrt((loc[tr[0]][0] - loc[tr[NCITIES - 1]][0]) * (loc[tr[0]][0] - loc[tr[NCITIES - 1]][0]) + (loc[tr[0]][1] - loc[tr[NCITIES - 1]][1]) * (loc[tr[0]][1] - loc[tr[NCITIES - 1]][1]));

	return new_energy;
}
