﻿
// SysProg-12.MultiThreadCliDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SysProg-12.MultiThreadCli.h"
#include "SysProg-12.MultiThreadCliDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSysProg12MultiThreadCliDlg 대화 상자



CSysProg12MultiThreadCliDlg::CSysProg12MultiThreadCliDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SYSPROG12MULTITHREADCLI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSysProg12MultiThreadCliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSysProg12MultiThreadCliDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON, &CSysProg12MultiThreadCliDlg::OnBnClickedButton)
END_MESSAGE_MAP()


// CSysProg12MultiThreadCliDlg 메시지 처리기

BOOL CSysProg12MultiThreadCliDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSysProg12MultiThreadCliDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSysProg12MultiThreadCliDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSysProg12MultiThreadCliDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD WINAPI UserThread(LPVOID p)
{
	CWnd* pEdit = (CWnd*)p;
	//srand(GetTickCount());
	TCHAR arNumber[5][10] = { _T("one"),_T("two"), _T("three"), _T("four"), _T("five"), };

	HANDLE hPipe = (HANDLE)p;
	TCHAR szInput[225], szOutput[255];
	DWORD dwRead, dwWritten;

	
	BOOL bIsConnect = WaitNamedPipe(_T("\\\\.\\pipe\\mymultipipe"), 0);

	if (bIsConnect == 0 | GetLastError() == ERROR_ABANDONED_WAIT_0)
	{
		AfxMessageBox(_T("인원꽉찼다"));
	}
	while (true) //서버 접속 무한 시도
	{
		hPipe = CreateFile(_T("\\\\.\\pipe\\mymultipipe"),
			GENERIC_READ | GENERIC_WRITE,
			0, NULL,
			OPEN_EXISTING, 0, NULL);
		
		if (hPipe != INVALID_HANDLE_VALUE) break;
	}

	while (true) //무한 번역 요청
	{
		int s = rand() % 5;
		lstrcpy(szOutput, arNumber[s]);

		BOOL bSuccess = WriteFile(hPipe, szOutput,
			(lstrlen(szOutput) + 1) * sizeof(TCHAR), &dwWritten, NULL);
		if ((bSuccess == FALSE) || (dwWritten == 0)) break;

		bSuccess = ReadFile(hPipe, szInput, 255, &dwRead, NULL);
		if ((bSuccess == FALSE) || (dwRead == 0)) break;

		TCHAR Mes[255];
		wsprintf(Mes, _T("%s => %s"), szOutput, szInput);
		pEdit->SetWindowText(Mes);
		Sleep(500);
	}
	AfxMessageBox(_T("서버가 종료되었습니다"));
	CloseHandle(hPipe);

	return 0;
}

void CSysProg12MultiThreadCliDlg::OnBnClickedButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWnd* pEdit = GetDlgItem(IDC_EDIT1);
	CloseHandle(CreateThread(
		NULL,
		0,
		UserThread,
		pEdit,
		0,
		0
	));
	
}
