#include "Stage1Scene.h"
#include "Stage1Background.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Network.h"
#include "PlayerPortrait.h"
#include "Score.h"
#include "Rect.h"
#include "StageUI.h"
#include "Ball.h"
#include "Tile.h"


CStage1Scene::CStage1Scene()
{
}


CStage1Scene::~CStage1Scene()
{
}

bool CStage1Scene::Init()
{
	CObj* pBack = GET_SINGLE( CObjectManager )->CreateObject<CRect>( "Rect" );
	CObj* pBG = GET_SINGLE( CObjectManager )->CreateObject<CStage1Background>( "Stage1BG" );
	PLAYERINFO tPlayerInfo = GET_SINGLE( CNetwork )->GetPlayerInfo();
	CObj* pPlayer = GET_SINGLE( CObjectManager )->CreateObject<CPlayer>( "Player" + to_string(tPlayerInfo.id));
	( ( CPlayer* )pPlayer )->SetPlayerInfo( tPlayerInfo );

	tPlayerInfo = GET_SINGLE( CNetwork )->GetOtherPlayerInfo();
	pPlayer = GET_SINGLE( CObjectManager )->CreateObject<CPlayer>( "Player" + to_string( tPlayerInfo.id ) );
	( ( CPlayer* )pPlayer )->SetPlayerInfo( tPlayerInfo );
	
	CObj* pPortrait = GET_SINGLE( CObjectManager )->CreateObject<CPlayerPortrait>( "PlayerPortrait1");
	( ( CPlayerPortrait* )pPortrait )->SetNumber( 1 );
	pPortrait->SetPos( 0, 450 );
	pPortrait->SetSize( 0, 0 );

	pPortrait = GET_SINGLE( CObjectManager )->CreateObject<CPlayerPortrait>( "PlayerPortrait2" );
	( ( CPlayerPortrait* )pPortrait )->SetNumber( 2 );
	pPortrait->SetPos( 650, 450 );
	pPortrait->SetSize( 0, 0 );

	CObj* pScore = GET_SINGLE( CObjectManager )->CreateObject<CScore>( "PlayerScore1" );
	pScore->SetPos( 150, 450 );

	pScore = GET_SINGLE( CObjectManager )->CreateObject<CScore>( "PlayerScore2" );
	pScore->SetPos( 550, 450 );

	CObj* pStageUI = GET_SINGLE( CObjectManager )->CreateObject<CStageUI>( "Stage1" );

	// Balls //
	// Type ===============================================//
	// Radius 25 = A
	// Radius 15 = B
	// Radius 8 = C
	CObj* pBall1 = GET_SINGLE( CObjectManager )->CreateObject<CBall>( "Ball1" );
	BALLINFO ballInfo;
	ballInfo.x = 200;
	ballInfo.y = 250;
	ballInfo.radius = 25;
	ballInfo.type = 'A';
	dynamic_cast<CBall*>(pBall1)->SetDirLR(DIR_RIGHT);
	dynamic_cast<CBall*>(pBall1)->SetDirUD(DIR_DOWN);
	dynamic_cast<CBall*>(pBall1)->SetBallInfo(ballInfo);

	CObj* pBall2 = GET_SINGLE(CObjectManager)->CreateObject<CBall>("Ball2");
	ballInfo.x = 600;
	ballInfo.y = 250;
	dynamic_cast<CBall*>(pBall2)->SetDirLR(DIR_RIGHT);
	dynamic_cast<CBall*>(pBall2)->SetDirUD(DIR_DOWN);
	dynamic_cast<CBall*>(pBall2)->SetBallInfo(ballInfo);
	// =====================================================//

	// Block ===============================================//
	CObj *pBlock1 = GET_SINGLE(CObjectManager)->CreateObject<CTile>("Block1");
	TILEINFO blockInfo;
	blockInfo.id = 0;
	blockInfo.x = 100;
	blockInfo.y = 250;
	blockInfo.maxFrame = 5;
	blockInfo.nowFrame = 0;
	blockInfo.type = 2;
	blockInfo.color = 0;
	blockInfo.cx = 100;
	blockInfo.cy = 40;
	dynamic_cast<CTile*>(pBlock1)->SetTileInfo(blockInfo);

	CObj *pBlock2 = GET_SINGLE(CObjectManager)->CreateObject<CTile>("Block2");
	blockInfo.id = 1;
	blockInfo.x = 600;
	blockInfo.y = 250;
	dynamic_cast<CTile*>(pBlock2)->SetTileInfo(blockInfo);

	CObj *pBlock3 = GET_SINGLE(CObjectManager)->CreateObject<CTile>("Block3");
	blockInfo.id = 2;
	blockInfo.x = 350;
	blockInfo.y = 150;
	dynamic_cast<CTile*>(pBlock3)->SetTileInfo(blockInfo);
	// =====================================================//

	return true;
}

void CStage1Scene::Input()
{
}

void CStage1Scene::Update( const float& fTimeDelta )
{
}

void CStage1Scene::Render( HDC hDC )
{
}
