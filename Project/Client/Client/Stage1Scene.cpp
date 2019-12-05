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
#include "Item.h"
#include "SceneManager.h"
#include "GameOverScene.h"
#include "Marker.h"

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
	CObj* pPlayer = GET_SINGLE( CObjectManager )->CreateObject<CPlayer>( "Player" + to_string( tPlayerInfo.id ) );
	( ( CPlayer* )pPlayer )->SetPlayerInfo( tPlayerInfo );
	( ( CPlayer* )pPlayer )->CreateBullets( tPlayerInfo.id );

	CObj* pPlayerRect = GET_SINGLE(CObjectManager)->CreateObject<CMarker>("PlayerRect");

	CObj* pHPUI = GET_SINGLE( CObjectManager )->CreateObject<CPlayerHP>( "MyPlayerHP" );
	( ( CPlayerHP* )pHPUI )->SetPlayer( tPlayerInfo.id );
	_cprintf( "MyPlayerID : %d\n", tPlayerInfo.id );

	tPlayerInfo = GET_SINGLE( CNetwork )->GetOtherPlayerInfo();
	pPlayer = GET_SINGLE( CObjectManager )->CreateObject<CPlayer>( "Player" + to_string( tPlayerInfo.id ) );
	( ( CPlayer* )pPlayer )->SetPlayerInfo( tPlayerInfo );
	( ( CPlayer* )pPlayer )->CreateBullets( tPlayerInfo.id );

	pHPUI = GET_SINGLE( CObjectManager )->CreateObject<CPlayerHP>( "OtherPlayerHP" );
	( ( CPlayerHP* )pHPUI )->SetPlayer( tPlayerInfo.id );
	_cprintf( "OtherPlayerID : %d\n", tPlayerInfo.id );

	CObj* pPortrait = GET_SINGLE( CObjectManager )->CreateObject<CPlayerPortrait>( "PlayerPortrait1" );
	( ( CPlayerPortrait* )pPortrait )->SetNumber( 1 );
	pPortrait->SetPos( 0, 450 );
	pPortrait->SetSize( 0, 0 );

	pPortrait = GET_SINGLE( CObjectManager )->CreateObject<CPlayerPortrait>( "PlayerPortrait2" );
	( ( CPlayerPortrait* )pPortrait )->SetNumber( 2 );
	pPortrait->SetPos( 650, 450 );
	pPortrait->SetSize( 0, 0 );

	CObj* pScore = GET_SINGLE( CObjectManager )->CreateObject<CScore>( "PlayerScore1" );
	( ( CScore* )pScore )->SetPlayer(GET_SINGLE(CNetwork)->GetPlayerInfo().id);

	pScore = GET_SINGLE( CObjectManager )->CreateObject<CScore>( "PlayerScore2" );
	( ( CScore* )pScore )->SetPlayer(GET_SINGLE(CNetwork)->GetOtherPlayerInfo().id);



	CObj* pBulletUI = GET_SINGLE( CObjectManager )->CreateObject<CPlayerBullet>( "PlayerBullet1" );
	( ( CPlayerBullet* )pBulletUI )->SetPlayer(GET_SINGLE(CNetwork)->GetPlayerInfo().id);

	pBulletUI = GET_SINGLE( CObjectManager )->CreateObject<CPlayerBullet>( "PlayerBullet2" );
	( ( CPlayerBullet* )pBulletUI )->SetPlayer(GET_SINGLE(CNetwork)->GetOtherPlayerInfo().id);

	CObj* pStageUI = GET_SINGLE( CObjectManager )->CreateObject<CStageUI>( "Stage1" );

	BALLINFO* pBallInfo = GET_SINGLE( CNetwork )->GetBallsInfo();
	m_iBallSize = GET_SINGLE( CNetwork )->GetBallsSize();
	for ( int i = 0; i < m_iBallSize; ++i )
	{
		CObj* pBalls = GET_SINGLE( CObjectManager )->CreateObject<CBall>( "Ball" + to_string( i ) );
		dynamic_cast< CBall* >( pBalls )->SetBallInfo( pBallInfo[i] );
	}
	TILEINFO* pTiles = GET_SINGLE( CNetwork )->GetTilesInfo();
	int iTilesSize = GET_SINGLE( CNetwork )->GetTilesSize();
	for ( int i = 0; i < iTilesSize; ++i )
	{
		CObj* pBlock = GET_SINGLE( CObjectManager )->CreateObject<CTile>( "Block" + to_string( i ) );
		dynamic_cast< CTile* >( pBlock )->SetTileInfo( pTiles[i] );
	}

	m_StageLimitTime = GET_SINGLE( CObjectManager )->CreateObject<CStageTime>( "Time : " );
	dynamic_cast< CStageTime* >( m_StageLimitTime )->SetTime( 70.f );

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
		CObj* pTile = GET_SINGLE(CObjectManager)->FindObject("Block" + to_string(i));
		dynamic_cast<CTile*>(pTile)->SetTileInfo(pTiles[i]);
	}

	int iItemSize = GET_SINGLE(CNetwork)->GetItemsSize();
	ITEMINFO* pItems = GET_SINGLE(CNetwork)->GetItemsInfo();

	for (int i = 0; i < iItemSize; ++i)
	{
		CObj* pItem = GET_SINGLE(CObjectManager)->CreateObject<CItem>("Item" + to_string(i));
		dynamic_cast<CItem*>(pItem)->SetItemInfo(pItems[i]);
	}

	int iGameState = GET_SINGLE(CNetwork)->GetGameState();
	if (iGameState == GAME_GAMEOVER)
	{
		GET_SINGLE(CSceneManager)->CreateScene<CGameOverScene>("GameOver");
	}
}

void CStage1Scene::Render( HDC hDC )
{
}
