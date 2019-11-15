#include "EnterKeyObj.h"



CEnterKeyObj::CEnterKeyObj() :
	m_bShow(true),
	m_fTime(0.f)
{
}


CEnterKeyObj::~CEnterKeyObj()
{
}

bool CEnterKeyObj::Init()
{
	SetTexture( "EnterKey", L"Texture/Logo/EnterKey.bmp" );

	SetPos( 280.f, 550.f );
	return true;
}

void CEnterKeyObj::Input()
{
	CObj::Input();
}

void CEnterKeyObj::Update( DWORD dwTime )
{
	CObj::Update( dwTime );

	m_fTime += dwTime;
	if ( m_fTime > 1000 )
	{
		m_fTime = 0.f;
	}
	m_bShow = !m_bShow;
}

void CEnterKeyObj::Render( HDC hDC )
{
	//if ( !m_bShow )
	//	return;

	CObj::Render( hDC );

	char str[128];
	wsprintf( str, "%d", m_fTime );
	TextOut( hDC, 100, 100, str, strlen( str ) );
}
