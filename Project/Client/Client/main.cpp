#include "main.h"
#include "GameFramework.h"
#include "Obj.h"
#include "ObjectManager.h"
#include "InputServerIP.h"

#define MAX_LOADSTRING 100
// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.

//�߰� ���� ����
HWND g_hWnd;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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

		// �������� �����Ѵ� 1�ʿ� �� 60
		if (GetTickCount64() - ullOldTime >= ullFrame)
		{
			float fTimeDelta = GetTickCount64() - ullOldTime;
			fTimeDelta = fTimeDelta / 1000.0f;
			//_cprintf(to_string(fTimeDelta).c_str());
			GameFramework.Logic(fTimeDelta);

			frame++;
			ullOldTime = GetTickCount64();
		}
		// 1�ʿ� �ѹ��� FPS ���� �����Ͽ� ȭ�鿡 ����Ѵ�
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
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

	// �������� Ŭ���̾�Ʈ ������ ���ϴ� ũ��� �����ϴ� ���

	SetWindowPos( hWnd, HWND_TOPMOST,
		0, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER );

	g_hWnd = hWnd;

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	return TRUE;
}
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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