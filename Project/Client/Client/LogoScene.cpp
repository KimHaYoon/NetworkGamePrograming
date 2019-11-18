#include "LogoScene.h"
#include "ObjectManager.h"
#include "Obj.h"
#include "LogoObj.h"
#include "EnterKeyObj.h"
#include "InputServerIP.h"
#include "Input.h"
#include "LobbyScene.h"
#include "SceneManager.h"
#include "Network.h"

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
		CObj* pInputServerIP = GET_SINGLE( CObjectManager )->FindObject( "InputServerIP" );
		string strIP = ( ( CInputServerIP* )pInputServerIP )->GetServerIP();
		if ( !GET_SINGLE( CNetwork )->Init( strIP ) )
			return;
		CScene* pLobby = GET_SINGLE( CSceneManager )->CreateScene<CLobbyScene>("Lobby");
	}
}

void CLogoScene::Update()
{
}

void CLogoScene::Render( HDC hDC )
{
}
