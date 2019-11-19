#include "StageUI.h"



CStageUI::CStageUI()
{
}


CStageUI::~CStageUI()
{
}

bool CStageUI::Init()
{
	SetPos( 305, 440 );
	return true;
}

void CStageUI::Input()
{
	CObj::Input();
}

void CStageUI::Update( const float& fTimeDelta )
{
	CObj::Update( fTimeDelta );
}

void CStageUI::Render( HDC hDC )
{
	CObj::Render( hDC );

	SetBkMode( hDC, TRANSPARENT );
	SetTextColor( hDC, RGB( 255, 255, 255 ) );
	HFONT hFont = CreateFont( 60, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT( "µÕ±Ù¸ð²Ã" ) );
	HFONT oldFont = ( HFONT )SelectObject( hDC, hFont );
	TextOut( hDC, m_tPos.x, m_tPos.y, m_strTag.c_str(), m_strTag.size() );
	SelectObject( hDC, oldFont );
	DeleteObject( hFont );
}
