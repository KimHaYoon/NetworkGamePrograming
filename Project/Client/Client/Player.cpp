#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include "ObjectManager.h"
#include "Network.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	PLAYERKEYINFO tKeys;
	tKeys.playerid = GET_SINGLE(CNetwork)->GetPlayerInfo().id;
	tKeys.left = false;
	tKeys.right = false;
	tKeys.space = false;

	m_bMoveAnimation = false;
	m_bShoot = false;
	m_binvincibile = false;
	m_bDir = rand() % 2;
	GET_SINGLE(CNetwork)->SetKeyInfo(tKeys);

	for (int i = 0; i < 5; ++i)
	{
		m_Bullet[i] = NULL;
	}

	return true;
}

void CPlayer::Input()
{
	if (m_tInfo.hp == 0)
		return;
	CObj::Input();

	if ( KEYDOWN( "MoveLeft" ) || KEYPUSH( "MoveLeft" ) )
	{
		if (GET_SINGLE(CNetwork)->GetPlayerInfo().id == m_tInfo.id)
			GET_SINGLE(CNetwork)->LeftKeyOn();
		m_bMoveAnimation = true;
		m_bDir = DIR_LEFT;
	}

	if ( KEYDOWN( "MoveRight" ) || KEYPUSH( "MoveRight" ) )
	{
		if (GET_SINGLE(CNetwork)->GetPlayerInfo().id == m_tInfo.id)
			GET_SINGLE(CNetwork)->RightKeyOn();
		m_bMoveAnimation = true;
		m_bDir = DIR_RIGHT;
	}

	if (KEYUP("MoveRight") || KEYUP("MoveLeft"))
	{
		nowFrame = 0;
		fFrame = 0.f;
		m_bMoveAnimation = false;
	}

	if ( KEYDOWN( "Space" ) )
	{
		if (GET_SINGLE(CNetwork)->GetPlayerInfo().id == m_tInfo.id)
			GET_SINGLE(CNetwork)->SpaceKeyOn();
		nowFrame = 0;
		fFrame = 0.f;
		m_bMoveAnimation = true;
		m_bShoot = true;
	}
	if (KEYUP("Space"))
	{
		nowFrame = 0;
		fFrame = 0.f;
		m_bMoveAnimation = false;
		m_bShoot = false;
	}

	if ( KEYDOWN( "Cheat" ) && m_tInfo.id == 1)
	{
		GET_SINGLE( CNetwork )->CheatKey();
	}
}

void CPlayer::Update( const float& fTimeDelta )
{
	if (m_tInfo.hp == 0)
		return;
	CObj::Update(fTimeDelta);

	if (GET_SINGLE(CNetwork)->GetPlayerInfo().id != m_tInfo.id)
	{
		m_tInfo = GET_SINGLE(CNetwork)->GetOtherPlayerInfo();
		SetPos(m_tInfo.x, m_tInfo.y);
	}
	else
	{
		m_tInfo = GET_SINGLE(CNetwork)->GetPlayerInfo();
		SetPos(m_tInfo.x, m_tInfo.y);
	}

	if (m_bMoveAnimation)
	{
		if (m_tInfo.id == 1)
		{
			if (m_bDir == DIR_LEFT)
			{
				wstring str = L"Texture/Player1/Player.left_" + to_wstring(nowFrame + 1) + L".bmp";
				SetTexture("Player_Left" + to_string(nowFrame + 1), str.c_str());
			}
			else if (m_bDir == DIR_RIGHT)
			{
				wstring str = L"Texture/Player1/Player.right_" + to_wstring(nowFrame + 1) + L".bmp";
				SetTexture("Player_Right" + to_string(nowFrame + 1), str.c_str());
			}
			if (m_bShoot)
			{
				wstring str = L"Texture/Player1/Player.stand_" + to_wstring(nowFrame + 1) + L".bmp";
				SetTexture("Player_Shoot" + to_string(nowFrame + 1), str.c_str());
			}
		}

		else if (m_tInfo.id == 2)
		{
			if (m_bDir == DIR_LEFT)
			{
				wstring str = L"Texture/Player2/player2.left_" + to_wstring(nowFrame + 1) + L".bmp";
				SetTexture("Player2_Left" + to_string(nowFrame + 1), str.c_str());
			}
			else if (m_bDir == DIR_RIGHT)
			{
				wstring str = L"Texture/Player2/player2.right_" + to_wstring(nowFrame + 1) + L".bmp";
				SetTexture("Player2_Right" + to_string(nowFrame + 1), str.c_str());
			}
			if (m_bShoot)
			{
				wstring str = L"Texture/Player2/player2.stand_" + to_wstring(nowFrame + 1) + L".bmp";
				SetTexture("Player2_Shoot" + to_string(nowFrame + 1), str.c_str());
			}
		}

		fFrame += (fTimeDelta * 15.f);
		if (fFrame > 1.f)
		{
			nowFrame++;
			nowFrame = nowFrame % m_tInfo.maxFrame;
			fFrame = 0.f;
		}
	}
}

void CPlayer::Render( HDC hDC )
{
	if (m_tInfo.hp == 0)
		return;

	if ( m_tInfo.invincibile )
	{
		m_binvincibile = !m_binvincibile;
	}

	else
		m_binvincibile = false;

	if ( m_binvincibile )
		return;

	CObj::Render( hDC );
}

void CPlayer::SetPlayerInfo( const PLAYERINFO & tInfo )
{
	m_tInfo = tInfo;

	if ( m_tInfo.id == 1 )
	{
		SetTexture("Player_Shoot1", L"Texture/Player1/Player.stand_1.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Shoot2", L"Texture/Player1/Player.stand_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Shoot3", L"Texture/Player1/Player.stand_3.bmp", true, RGB(0, 255, 0));

		SetTexture("Player_Left1", L"Texture/Player1/Player.left_1.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Left2", L"Texture/Player1/Player.left_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Left3", L"Texture/Player1/Player.left_3.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Left4", L"Texture/Player1/Player.left_4.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Left5", L"Texture/Player1/Player.left_5.bmp", true, RGB(0, 255, 0));

		SetTexture("Player_Right2", L"Texture/Player1/Player.right_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Right3", L"Texture/Player1/Player.right_3.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Right4", L"Texture/Player1/Player.right_4.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Right5", L"Texture/Player1/Player.right_5.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Right1", L"Texture/Player1/Player.right_1.bmp", true, RGB(0, 255, 0));
	}

	else
	{
		SetTexture("Player2_Shoot1", L"Texture/Player2/player2.stand_1.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Shoot2", L"Texture/Player2/player2.stand_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Shoot3", L"Texture/Player2/player2.stand_3.bmp", true, RGB(0, 255, 0));

		SetTexture("Player2_Left1", L"Texture/Player2/player2.left_1.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left2", L"Texture/Player2/player2.left_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left3", L"Texture/Player2/player2.left_3.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left4", L"Texture/Player2/player2.left_4.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left5", L"Texture/Player2/player2.left_5.bmp", true, RGB(0, 255, 0));
						  
		SetTexture("Player2_Right2", L"Texture/Player2/player2.right_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right3", L"Texture/Player2/player2.right_3.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right4", L"Texture/Player2/player2.right_4.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right5", L"Texture/Player2/player2.right_5.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right1", L"Texture/Player2/player2.right_1.bmp", true, RGB(0, 255, 0));
	}

	SetPos( m_tInfo.x, m_tInfo.y );
}

void CPlayer::CreateBullets(int id)
{
	for (int i = 0; i < 5; ++i)
	{
		BULLETINFO tInfo;
		tInfo = GET_SINGLE(CNetwork)->GetBulletInfo(id, i);
		m_Bullet[i] = GET_SINGLE(CObjectManager)->CreateObject<CBullet>("Bullet" + to_string(i + 1 + ((id - 1) * 5)));
		dynamic_cast<CBullet*>(m_Bullet[i])->SetBulletInfo(tInfo);
	}
}

void CPlayer::BulletShot()
{
	for (int i = 0; i < 5; ++i)
	{
		if (dynamic_cast<CBullet *>(m_Bullet[i])->GetShot() == false)
		{
			dynamic_cast<CBullet *>(m_Bullet[i])->Shot(m_tPos.x, m_tPos.y);
			break;
		}
	}
}
