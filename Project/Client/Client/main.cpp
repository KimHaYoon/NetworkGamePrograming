#include "main.h"
#include "GameFramework.h"
#include "Obj.h"
#include "ObjectManager.h"
#include "InputServerIP.h"

#define MAX_LOADSTRING 100
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

//추가 전역 변수
HWND g_hWnd;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass( HINSTANCE hInstance );
BOOL                InitInstance( HINSTANCE, int );
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	MyRegisterClass( hInstance );

	// 응용 프로그램 초기화를 수행합니다.
	if ( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}

	CGameFramework GameFramework;
	GameFramework.Init(g_hWnd, hInstance);

	MSG msg;
	msg.message = NULL;

	ULONGLONG ullOldTime = GetTickCount64();
	ULONGLONG ullFrame = 10.0; // 1000.0 / 60.0

	int frame = 0;
	ULONGLONG ullOldTime2 = GetTickCount64();

	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// 프레임을 고정한다 1초에 약 60
		if (GetTickCount64() - ullOldTime >= ullFrame)
		{
			float fTimeDelta = GetTickCount64() - ullOldTime;
			fTimeDelta = fTimeDelta / 1000.0f;
			//_cprintf(to_string(fTimeDelta).c_str());
			GameFramework.Logic(fTimeDelta);

			frame++;
			ullOldTime = GetTickCount64();
		}
		// 1초에 한번씩 FPS 값을 산출하여 화면에 출력한다
		if (GetTickCount64() - ullOldTime2 >= 1000)
		{
			TCHAR szFPS[12]{};
			wsprintf(szFPS, "FPS : %d", frame);
			SetWindowText(g_hWnd, szFPS);
			ullOldTime2 = GetTickCount64();
			frame = 0;
		}

		/*if ( GetTickCount() - dwTime >= (1000 / 60) )
		{
			GameFramework.Logic( dwTime );
		
			dwTime = GetTickCount();
		}*/

	}

	return ( int )msg.wParam;
}
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY170825);
	wcex.lpszClassName = L"GPS_1";
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );

	return RegisterClassExW( &wcex );
}
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	RECT	rc = { 0, 0, WINX, WINY };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	HWND hWnd = CreateWindowW( L"GPS_1", L"GPS_1", WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr,
		hInstance, nullptr );

	if ( !hWnd )
	{
		return FALSE;
	}

	// 윈도우의 클라이언트 영역을 원하는 크기로 설정하는 방법

	SetWindowPos( hWnd, HWND_TOPMOST,
		0, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER );

	g_hWnd = hWnd;

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	return TRUE;
}
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static string strServerIP;
	HDC hdc;
	PAINTSTRUCT ps;

	switch ( message )
	{
	case WM_CHAR:
	{
		if ( VK_BACK == wParam )
		{
			strServerIP.pop_back();
		}

		else if ( VK_RETURN != wParam )
		{
			strServerIP += ( char* )&wParam;
		}

		else if ( VK_RETURN == wParam && strServerIP.empty())
		{
			return 0;
		}

		CObj* pObj = GET_SINGLE( CObjectManager )->FindObject( "InputServerIP" );

		if ( pObj )
			( ( CInputServerIP* )pObj )->SetServerIP( strServerIP );
	}
		break;
	case WM_KEYDOWN:
		switch ( wParam )
		{
		case VK_ESCAPE:
			DestroyWindow( hWnd );
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}