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
#include "StageTime.h"
#include "PlayerHP.h"
#include "PlayerBullet.h"


CStage1Scene::CStage1Scene()
{
	m_iBallTagCount = 2;
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
	( ( CPlayer* )pPlayer )->CreateBullets( tPlayerInfo.id );

	tPlayerInfo = GET_SINGLE( CNetwork )->GetOtherPlayerInfo();
	pPlayer = GET_SINGLE( CObjectManager )->CreateObject<CPlayer>( "Player" + to_string( tPlayerInfo.id ) );
	( ( CPlayer* )pPlayer )->SetPlayerInfo( tPlayerInfo );
	( ( CPlayer* )pPlayer )->CreateBullets( tPlayerInfo.id );
	
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
	((CScore*)pScore)->SetPlayer(1);

	pScore = GET_SINGLE( CObjectManager )->CreateObject<CScore>( "PlayerScore2" );
	pScore->SetPos( 530, 450 );
	((CScore*)pScore)->SetPlayer(2);

	CObj* pHPUI = GET_SINGLE(CObjectManager)->CreateObject<CPlayerHP>("PlayerHP1");
	pHPUI->SetPos(158, 500);
	((CPlayerHP*)pHPUI)->SetPlayer(1);

	pHPUI = GET_SINGLE(CObjectManager)->CreateObject<CPlayerHP>("PlayerHP2");
	pHPUI->SetPos(507, 500);
	((CPlayerHP*)pHPUI)->SetPlayer(2);

	CObj* pBulletUI = GET_SINGLE(CObjectManager)->CreateObject<CPlayerBullet>("PlayerBullet1");
	pBulletUI->SetPos(160, 550);
	((CPlayerBullet*)pBulletUI)->SetPlayer(1);

	pBulletUI = GET_SINGLE(CObjectManager)->CreateObject<CPlayerBullet>("PlayerBullet2");
	pBulletUI->SetPos(545, 550);
	((CPlayerBullet*)pBulletUI)->SetPlayer(2);

	CObj* pStageUI = GET_SINGLE( CObjectManager )->CreateObject<CStageUI>( "Stage1" );

	// Balls //
	// Type ===============================================//
	// Radius 25 = A
	// Radius 15 = B
	// Radius 8 = C
	/*CObj* pBall1 = GET_SINGLE( CObjectManager )->CreateObject<CBall>( "Ball1" );
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
	dynamic_cast<CBall*>(pBall2)->SetBallInfo(ballInfo);*/

	BALLINFO* pBallInfo = GET_SINGLE( CNetwork )->GetBallsInfo();
	m_iBallSize = GET_SINGLE( CNetwork )->GetBallsSize();
	for ( int i = 0; i < m_iBallSize; ++i )
	{
		CObj* pBalls = GET_SINGLE( CObjectManager )->CreateObject<CBall>( "Ball" + to_string( i ) );
		dynamic_cast< CBall* >( pBalls )->SetBallInfo( pBallInfo[i] );
	}
	// =====================================================//

	// Block ===============================================//		191128 ¼öÁ¤
	TILEINFO* pTiles = GET_SINGLE( CNetwork )->GetTilesInfo();
	int iTilesSize = GET_SINGLE( CNetwork )->GetTilesSize();
	for ( int i = 0; i < iTilesSize; ++i )
	{
		CObj* pBlock = GET_SINGLE( CObjectManager )->CreateObject<CTile>( "Block" + to_string( i ) );
		dynamic_cast< CTile* >( pBlock )->SetTileInfo( pTiles[i] );
	}


	// Stage1_LimitTime
	m_StageLimitTime = GET_SINGLE(CObjectManager)->CreateObject<CStageTime>("Time : ");
	dynamic_cast<CStageTime*>(m_StageLimitTime)->SetTime(70.f);

	return true;
}

void CStage1Scene::Input()
{
}

void CStage1Scene::Update( const float& fTimeDelta )
{
	int iCurBallSize = GET_SINGLE( CNetwork )->GetBallsSize();
	int iSubBallSize = abs( iCurBallSize - m_iBallSize );
	BALLINFO* pBallInfo = GET_SINGLE( CNetwork )->GetBallsInfo();

	for ( int i = 0; i < iSubBallSize; ++i )
	{
		CObj* pBalls = GET_SINGLE(CObjectManager)->CreateObject<CBall>( "Ball" + to_string( i + m_iBallSize ) );
		dynamic_cast< CBall* >( pBalls )->SetBallInfo( pBallInfo[i + m_iBallSize] );
	}

	for ( int i = 0; i < iCurBallSize; ++i )
	{
		CObj* pBalls = GET_SINGLE( CObjectManager )->FindObject( "Ball" + to_string( i ) );
		dynamic_cast< CBall* >( pBalls )->SetBallInfo( pBallInfo[i] );

		//_cprintf( "%s:%d, %d \n", pBalls->GetTag(), (int)pBalls->GetPos().x, (int)pBalls->GetPos().y );
	}

	m_iBallSize = iCurBallSize;

	int iTileSize = GET_SINGLE(CNetwork)->GetTilesSize();
	TILEINFO* pTiles = GET_SINGLE(CNetwork)->GetTilesInfo();
	
	for (int i = 0; i < iTileSize; ++i)
	{
		_cprintf("%d\n", i);
		CObj* pTile = GET_SINGLE(CObjectManager)->FindObject("Block" + to_string(i));
		dynamic_cast<CTile*>(pTile)->SetTileInfo(pTiles[i]);
	}
}

void CStage1Scene::Render( HDC hDC )
{
}
