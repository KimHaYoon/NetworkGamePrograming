#include "InputServerIP.h"



CInputServerIP::CInputServerIP()
{
}


CInputServerIP::~CInputServerIP()
{
}

bool CInputServerIP::Init()
{
	SetTexture( "InputServerIP", L"Texture/Logo/InputServerIP.bmp" );
	SetPos( 200.f, 500.f );
	return true;
}

void CInputServerIP::Input()
{
	CObj::Input();
}

void CInputServerIP::Update( const float& fTimeDelta )
{
	CObj::Update( fTimeDelta );
}

void CInputServerIP::Render( HDC hDC )
{
	CObj::Render( hDC );
	
	SetBkMode( hDC, TRANSPARENT );
	SetTextColor( hDC, RGB( 255, 255, 255 ) );
	HFONT hFont = CreateFont( 40, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT( "µÕ±Ù¸ð²Ã" ) );
	HFONT oldFont = ( HFONT )SelectObject( hDC, hFont );
	TextOut( hDC, 220, 505, m_strServerIP.c_str(), m_strServerIP.size() );
	SelectObject( hDC, oldFont );
	DeleteObject( hFont );
}

void CInputServerIP::SetServerIP( const string & strServerIP )
{
	m_strServerIP = strServerIP;
}

string CInputServerIP::GetServerIP() const
{
	return m_strServerIP;
}
