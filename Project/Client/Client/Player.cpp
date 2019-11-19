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
	return true;
}

void CPlayer::Input()
{
	CObj::Input();

	if ( KEYDOWN( "MoveLeft" ) || KEYPUSH( "MoveLeft" ) )
	{
		if ( m_tPos.x > 10 )
			m_tPos.x -= 10;
	}

	if ( KEYDOWN( "MoveRight" ) || KEYPUSH( "MoveRight" ) )
	{
		if ( m_tPos.x < 730 )
			m_tPos.x += 10;
	}

	if ( KEYDOWN( "Space" ) )
	{
		BulletShot();
	}
}

void CPlayer::Update( DWORD dwTime )
{
	CObj::Update( dwTime );

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
		SetTexture( "Player1", L"Texture/Player1/Player.right_1.bmp", true, RGB(0, 255, 0) );
	}

	else
	{
		SetTexture( "Player2", L"Texture/Pslayer2/player2.left_1.bmp", true, RGB( 0, 255, 0 ) );
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
