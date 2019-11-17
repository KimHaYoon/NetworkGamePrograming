#include "PlayerPortrait.h"



CPlayerPortrait::CPlayerPortrait() :
	m_iNumber( 1 )
{
}


CPlayerPortrait::~CPlayerPortrait()
{
}

bool CPlayerPortrait::Init()
{
	SetSize( 300, 300 );

	return true;
}

void CPlayerPortrait::Input()
{
	CObj::Input();
}

void CPlayerPortrait::Update( DWORD dwTime )
{
	CObj::Update( dwTime );
}

void CPlayerPortrait::Render( HDC hDC )
{
	CObj::Render( hDC );
}

void CPlayerPortrait::SetNumber( int iNumber )
{
	m_iNumber = iNumber;
	if ( m_iNumber == 1 )
	{
		SetPos( 50, 150 );
		SetTexture( "Player1_Face", L"Texture/Player1/Player1_Face.bmp", true );
	}
	else
	{
		SetPos( 450, 150 );
		SetTexture( "Player2_Face", L"Texture/Player2/Player2_Face.bmp", true );
	}
}
