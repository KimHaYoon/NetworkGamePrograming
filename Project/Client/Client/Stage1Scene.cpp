#include "Stage1Scene.h"
#include "Stage1Background.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Network.h"


CStage1Scene::CStage1Scene()
{
}


CStage1Scene::~CStage1Scene()
{
}

bool CStage1Scene::Init()
{
	CObj* pBG = GET_SINGLE( CObjectManager )->CreateObject<CStage1Background>( "Stage1" );
	PLAYERINFO tPlayerInfo = GET_SINGLE( CNetwork )->GetPlayerInfo();
	CObj* pPlayer = GET_SINGLE( CObjectManager )->CreateObject<CPlayer>( "Player" + to_string(tPlayerInfo.id));
	( ( CPlayer* )pPlayer )->SetPlayerInfo( tPlayerInfo );

	return true;
}

void CStage1Scene::Input()
{
}

void CStage1Scene::Update()
{
}

void CStage1Scene::Render( HDC hDC )
{
}
