#include "LogoScene.h"
#include "ObjectManager.h"
#include "Obj.h"
#include "LogoObj.h"
#include "EnterKeyObj.h"
#include "InputServerIP.h"
#include "Input.h"
#include "LobbyScene.h"
#include "SceneManager.h"

CLogoScene::CLogoScene()
{
}


CLogoScene::~CLogoScene()
{
}

bool CLogoScene::Init()
{
	CObj* pLogo = GET_SINGLE( CObjectManager )->CreateObject<CLogoObj>( "LogoObj" );
	CObj* pEnterKey = GET_SINGLE( CObjectManager )->CreateObject<CEnterKeyObj>( "EnterKeyObj" );
	CObj* pInputServerIP = GET_SINGLE( CObjectManager )->CreateObject<CInputServerIP>( "InputServerIP" );

	return true;
}

void CLogoScene::Input()
{
	if ( KEYDOWN( "Enter" ) )
	{
		CScene* pLobby = new CLobbyScene;
		GET_SINGLE( CSceneManager )->SceneChange( pLobby );
	}
}

void CLogoScene::Update()
{
}

void CLogoScene::Render( HDC hDC )
{
}
