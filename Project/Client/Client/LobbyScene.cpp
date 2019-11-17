#include "LobbyScene.h"
#include "ObjectManager.h"
#include "LobbyBackground.h"
#include "PlayerPortrait.h"
#include "Stage1Scene.h"
#include "SceneManager.h"

CLobbyScene::CLobbyScene() : 
	m_bGameStart(false)
{
}


CLobbyScene::~CLobbyScene()
{
}

bool CLobbyScene::Init()
{
	CObj* pLobbyBK = GET_SINGLE( CObjectManager )->CreateObject< CLobbyBackground>( "LobbyBK" );
	CObj* pPlayer1Portrait = GET_SINGLE( CObjectManager )->CreateObject<CPlayerPortrait>( "Player1Portrait" );
	( ( CPlayerPortrait* )pPlayer1Portrait )->SetNumber( 1 );

	CObj* pPlayer2Portrait = GET_SINGLE( CObjectManager )->CreateObject<CPlayerPortrait>( "Player2Portrait" );
	( ( CPlayerPortrait* )pPlayer2Portrait )->SetNumber( 2 );

	return true;
}

void CLobbyScene::Input()
{
}

void CLobbyScene::Update()
{
	if( m_bGameStart )
		CScene* pStage1 = GET_SINGLE( CSceneManager )->CreateScene<CStage1Scene>( "Stage1" );
}

void CLobbyScene::Render( HDC hDC )
{
	// 내 캐릭터에 테두리 반짝거리기
}
