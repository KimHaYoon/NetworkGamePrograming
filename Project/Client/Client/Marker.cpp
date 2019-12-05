#include "Texture.h"
#include "Network.h"
#include "Marker.h"

CMarker::CMarker()
{
	//m_dwColorKey = RGB( 100, 100, 255 );
}

CMarker::~CMarker()
{
}

bool CMarker::Init()
{
	//SetTexture( "Rect", L"Texture/BackBuffer.bmp" );
	//SetSize(70.f, 70.f);
	return true;
}

void CMarker::Input()
{
	CObj::Input();
}

void CMarker::Update(const float& fTimeDelta)
{
	CObj::Update(fTimeDelta);
	PLAYERINFO tInfo = GET_SINGLE(CNetwork)->GetPlayerInfo();
	SetPos(tInfo.x + 20, tInfo.y - 20);
}

void CMarker::Render(HDC hDC)
{
	CObj::Render(hDC);

	//HBRUSH Brush, OldBrush;
	//Brush = CreateSolidBrush( m_dwColorKey );
	//OldBrush = ( HBRUSH )SelectObject( hDC, Brush );
	//Rectangle( hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y );

	//SelectObject( hDC, OldBrush );
	//DeleteObject( Brush );

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(0, 255, 255));
	HFONT hFont = CreateFont(20, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("µÕ±Ù¸ð²Ã"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	string strPlayerTag = "Me";
	TextOut(hDC, m_tPos.x, m_tPos.y, strPlayerTag.c_str(), strPlayerTag.size());
	SelectObject(hDC, oldFont);
	DeleteObject(hFont);
}