#include "Player.h"
#include "Input.h"


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	return true;
}

void CPlayer::Input()
{
	CObj::Input();

	if ( KEYDOWN( "MoveLeft" ) )
	{
		if ( m_tPos.x > 10 )
			m_tPos.x -= 10;
	}

	if ( KEYDOWN( "MoveRight" ) )
	{
		if ( m_tPos.x < 780 )
			m_tPos.x += 10;
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
		SetTexture( "Player2", L"Texture/Player2/player2.left_1.bmp", true, RGB( 0, 255, 0 ) );
	}

	SetPos( m_tInfo.x, m_tInfo.y );
}
