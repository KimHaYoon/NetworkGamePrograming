#include "EndingLogo.h"



CEndingLogo::CEndingLogo()
{
}


CEndingLogo::~CEndingLogo()
{
}

bool CEndingLogo::Init()
{
	SetPos(10, 200);
	return true;
}

void CEndingLogo::Input()
{
	CObj::Input();
}

void CEndingLogo::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);
}

void CEndingLogo::Render(HDC hDC)
{
	CObj::Render(hDC);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(rand()%256, rand() % 256, rand() % 256));
	HFONT hFont = CreateFont(100, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("µÕ±Ù¸ð²Ã"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	TextOut(hDC, m_tPos.x, m_tPos.y, m_strTag.c_str(), m_strTag.size());
	SelectObject(hDC, oldFont);
	DeleteObject(hFont);
}
