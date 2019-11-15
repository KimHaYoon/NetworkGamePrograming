#include "GameFramework.h"
#include "Input.h"
#include "ObjectManager.h"
#include "Texture.h"
#include "ResourcesManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Texture.h"


CGameFramework::CGameFramework() :
	m_pBackBuffer(NULL)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(281);
}


CGameFramework::~CGameFramework()
{
	GET_SINGLE( CObjectManager )->DestroyInst();
	GET_SINGLE( CResourcesManager )->DestroyInst();
	GET_SINGLE( CInput )->DestroyInst();
	GET_SINGLE( CSceneManager )->DestroyInst();
	ReleaseDC( m_hWnd, m_hDC );
}

void CGameFramework::Logic( DWORD dwTime )
{
	Input();
	Update(dwTime);
	Render();
}

bool CGameFramework::Init(HWND hWnd, HINSTANCE  hInst)
{
	m_hWnd = hWnd;
	m_hDC = GetDC( hWnd );

	if ( !GET_SINGLE( CInput )->Init( m_hWnd ) )
		return false;

	if ( !GET_SINGLE( CResourcesManager)->Init( hInst, m_hDC) )
		return false;

	if ( !GET_SINGLE( CObjectManager )->Init() )
		return false;

	if ( !GET_SINGLE( CSceneManager )->Init() )
		return false;

	m_pBackBuffer = GET_SINGLE( CResourcesManager )->FindTexture( "BackBuffer" );
}

void CGameFramework::Input()
{
	GET_SINGLE( CInput )->Update();
	GET_SINGLE( CObjectManager )->Input();
	GET_SINGLE( CSceneManager )->Input();
}

void CGameFramework::Update( DWORD dwTime )
{
	GET_SINGLE( CObjectManager )->Update( dwTime );
	GET_SINGLE( CSceneManager )->Update();
}

void CGameFramework::Render()
{
	GET_SINGLE( CObjectManager )->Render(m_pBackBuffer->GetMemDC());
	GET_SINGLE( CSceneManager )->Render(m_pBackBuffer->GetMemDC());

	// 마지막으로 백버퍼를 화면에 그린다.
	BitBlt( m_hDC, 0, 0, WINX, WINY, m_pBackBuffer->GetMemDC(),
		0, 0, SRCCOPY );
}
