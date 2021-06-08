﻿
// SysProg-6.SemDownView.cpp: CSysProg6SemDownView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SysProg-6.SemDown.h"
#endif

#include "SysProg-6.SemDownDoc.h"
#include "SysProg-6.SemDownView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSysProg6SemDownView

IMPLEMENT_DYNCREATE(CSysProg6SemDownView, CView)

BEGIN_MESSAGE_MAP(CSysProg6SemDownView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CSysProg6SemDownView 생성/소멸
HANDLE hSemDown;
CSysProg6SemDownView::CSysProg6SemDownView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	hSemDown = CreateSemaphore(NULL, 3, 3, NULL);
}

CSysProg6SemDownView::~CSysProg6SemDownView()
{
	CloseHandle(hSemDown);
}

BOOL CSysProg6SemDownView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSysProg6SemDownView 그리기

void CSysProg6SemDownView::OnDraw(CDC* /*pDC*/)
{
	CSysProg6SemDownDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CSysProg6SemDownView 인쇄

BOOL CSysProg6SemDownView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSysProg6SemDownView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSysProg6SemDownView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CSysProg6SemDownView 진단

#ifdef _DEBUG
void CSysProg6SemDownView::AssertValid() const
{
	CView::AssertValid();
}

void CSysProg6SemDownView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSysProg6SemDownDoc* CSysProg6SemDownView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSysProg6SemDownDoc)));
	return (CSysProg6SemDownDoc*)m_pDocument;
}
#endif //_DEBUG


// CSysProg6SemDownView 메시지 처리기
int Y;
DWORD WINAPI ThreadDownLoad(LPVOID p) {
	CSysProg6SemDownView* pView = (CSysProg6SemDownView*)p;
	CDC* pDC = pView->GetDC();

	srand(GetTickCount());
	int s = rand() % 5 + 1; // 난수사용
	Y += 20;
	int y = Y;
	pDC->TextOut(10, y, _T("대기중"));
	GdiFlush();
	//WaitForMultipleObjects(3, &hSemDown, FALSE, 1);

	WaitForSingleObject(hSemDown, INFINITE);

	//Critical Section
	CString strTemp;
	for (int i = 0; i < 100; i++)
	{
		strTemp.Format(_T("다운로드중 : %d%% 완료"), i);
		for (int j = 0; j < i; j++)
			strTemp += _T("|");

		pDC->TextOut(10, y, strTemp);
		Sleep(s * 20);
	}
	//Critical Section
	ReleaseSemaphore(hSemDown, 1, NULL);
	
	pDC->TextOut(10, y, _T("Complete!!"));
	//pDC->TextOut(100, 100, _T("다운로드를 완료했습니다"));

	pView->ReleaseDC(pDC);
	return 0;
}
void CSysProg6SemDownView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DWORD dwid;
	CloseHandle(CreateThread(
		NULL,
		0,
		ThreadDownLoad,
		this,
		0,
		&dwid
	));
	CView::OnLButtonDown(nFlags, point);
}
