#include "EnterKeyObj.h"

CEnterKeyObj::CEnterKeyObj() :
	m_bShow(true)
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
		
	m_bShow = !m_bShow;
}

void CEnterKeyObj::Render( HDC hDC )
{
	if ( !m_bShow )
		return;

	CObj::Render( hDC );
}
