#include "LobbyBackground.h"



CLobbyBackground::CLobbyBackground() : 
	m_iColorG(150)
{
}


CLobbyBackground::~CLobbyBackground()
{
}

bool CLobbyBackground::Init()
{
	SetTexture( "Lobby", L"Texture/BackBuffer.bmp" );

	return true;
}

void CLobbyBackground::Input()
{
	CObj::Input();
}

void CLobbyBackground::Update( const float& fTimeDelta )
{
	CObj::Update( fTimeDelta );
}

void CLobbyBackground::Render( HDC hDC )
{
	CObj::Render( hDC );

	SetBkMode( hDC, TRANSPARENT );
	SetTextColor( hDC, RGB( 255, m_iColorG, 0 ) );
	HFONT hFont = CreateFont( 100, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT( "µÕ±Ù¸ð²Ã" ) );
	HFONT oldFont = ( HFONT )SelectObject( hDC, hFont );

	string str{ "Super Pang!" };
	TextOut( hDC, 120, 0, str.c_str(), str.size() );
	SelectObject( hDC, oldFont );
	DeleteObject( hFont );

	m_iColorG = ( ++m_iColorG ) % 256 + 150;
}
