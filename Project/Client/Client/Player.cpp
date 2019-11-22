#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include "ObjectManager.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{

	for (int i = 0; i < 5; ++i)
	{
		m_Bullet[i] = GET_SINGLE(CObjectManager)->CreateObject<CBullet>("Bullet" + to_string(i));
	}
	m_nMaxFrame = 5;
	m_nNowFrame = 0;

	return true;
}

void CPlayer::Input()
{
	CObj::Input();

	if ( KEYDOWN( "MoveLeft" ) || KEYPUSH( "MoveLeft" ) )
	{
		if ( m_tPos.x > 10 )
			m_tPos.x -= 10;
		m_tInfo.moveAnimation = true;
	}

	if ( KEYDOWN( "MoveRight" ) || KEYPUSH( "MoveRight" ) )
	{
		if ( m_tPos.x < 730 )
			m_tPos.x += 10;
		m_tInfo.moveAnimation = true;

	}

	if (KEYUP("MoveRight") || KEYUP("MoveLeft"))
	{
		m_tInfo.moveAnimation = false;
	}

	if ( KEYDOWN( "Space" ) )
	{
		BulletShot();
		m_tInfo.shoot = true;
	}
}

void CPlayer::Update( DWORD dwTime )
{
	CObj::Update( dwTime );

	if (m_tInfo.moveAnimation)
	{
		if (m_tInfo.id == 1)
		{
			if (m_tInfo.dir == DIR_LEFT)
			{
				wstring str = L"Texture/Player1/Player.left_" + to_wstring(m_nNowFrame) + L".bmp";
				SetTexture("Player_Left" + to_string(m_nNowFrame), str.c_str());
			}
			else if (m_tInfo.dir == DIR_RIGHT)
			{
				wstring str = L"Texture/Player1/Player.right_" + to_wstring(m_nNowFrame) + L".bmp";
				SetTexture("Player_Right" + to_string(m_nNowFrame), str.c_str());
			}
		}

		else if (m_tInfo.id == 2)
		{
			if (m_tInfo.dir == DIR_LEFT)
			{
				wstring str = L"Texture/Player2/player2.left_" + to_wstring(m_nNowFrame) + L".bmp";
				SetTexture("Player2_Left" + to_string(m_nNowFrame), str.c_str());
			}
			else if (m_tInfo.dir == DIR_RIGHT)
			{
				wstring str = L"Texture/Player2/player2.right_" + to_wstring(m_nNowFrame) + L".bmp";
				SetTexture("Player2_Right" + to_string(m_nNowFrame), str.c_str());
			}
		}

		m_nNowFrame++;
		m_nNowFrame = m_nNowFrame % m_nMaxFrame;
	}

}

void CPlayer::Render( HDC hDC )
{
	CObj::Render( hDC );
}

void CPlayer::SetPlayerInfo( const PLAYERINFO & tInfo )
{
	m_tInfo = tInfo;

	if ( m_tInfo.id == 1 )
	{
		SetTexture("Player_Shoot1", L"Texture/Player1/Player.stand_1.bmp", true, RGB(0, 255, 0));
		SetTexture("Player_Shoot2", L"Texture/Player1/Player.stand_2.bmp", true, RGB(0, 255, 0));

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
						  
		SetTexture("Player2_Left1", L"Texture/Player2/player2.right_1.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left2", L"Texture/Player2/player2.right_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left3", L"Texture/Player2/player2.right_3.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left4", L"Texture/Player2/player2.right_4.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Left5", L"Texture/Player2/player2.right_5.bmp", true, RGB(0, 255, 0));
						  
		SetTexture("Player2_Right2", L"Texture/Player2/player2.right_2.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right3", L"Texture/Player2/player2.right_3.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right4", L"Texture/Player2/player2.right_4.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right5", L"Texture/Player2/player2.right_5.bmp", true, RGB(0, 255, 0));
		SetTexture("Player2_Right1", L"Texture/Player2/player2.right_1.bmp", true, RGB(0, 255, 0));
	}

	SetPos( m_tInfo.x, m_tInfo.y );
}

void CPlayer::BulletShot()
{
	for (int i = 0; i < 5; ++i)
	{
		if (dynamic_cast<CBullet *>(m_Bullet[i])->GetShot() == false)
		{
			BULLETINFO bulletInfo;
			bulletInfo.id = i;
			bulletInfo.x = m_tPos.x;
			bulletInfo.y = m_tPos.y;
			dynamic_cast<CBullet *>(m_Bullet[i])->SetBulletInfo(bulletInfo);
			dynamic_cast<CBullet *>(m_Bullet[i])->Shot();
			break;
		}
	}
}
