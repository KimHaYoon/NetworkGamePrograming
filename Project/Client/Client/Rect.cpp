#include "Rect.h"
#include "Texture.h"
#include "Network.h"

CRect::CRect()
{
	m_dwColorKey = RGB( 255, 255, 255 );
}

CRect::~CRect()
{
}

bool CRect::Init()
{
	SetTexture( "Rect", L"Texture/BackBuffer.bmp" );
	SetSize(70.f, 70.f);
	return true;
}

void CRect::Input()
{
	CObj::Input();
}

void CRect::Update( const float& fTimeDelta )
{
	CObj::Update( fTimeDelta );
}

void CRect::Render( HDC hDC )
{
	CObj::Render( hDC );

	//HBRUSH Brush, OldBrush;
	//Brush = CreateSolidBrush( m_dwColorKey );
	//OldBrush = ( HBRUSH )SelectObject( hDC, Brush );
	//Rectangle( hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y );

	//SelectObject( hDC, OldBrush );
	//DeleteObject( Brush );
}

void CRect::SetColor( COLORREF dwColorKey )
{
	m_dwColorKey = dwColorKey;
}
