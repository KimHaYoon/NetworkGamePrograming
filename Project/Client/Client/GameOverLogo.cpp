#include "GameOverLogo.h"



CGameOverLogo::CGameOverLogo()
{
}


CGameOverLogo::~CGameOverLogo()
{
}

bool CGameOverLogo::Init()
{
	SetPos(20, 220);
	return true;
}

void CGameOverLogo::Input()
{
	CObj::Input();
}

void CGameOverLogo::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);
}

void CGameOverLogo::Render(HDC hDC)
{
	CObj::Render(hDC);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(200, 100, 100));
	HFONT hFont = CreateFont(120, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("µÕ±Ù¸ð²Ã"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	TextOut(hDC, m_tPos.x, m_tPos.y, m_strTag.c_str(), m_strTag.size());
	SelectObject(hDC, oldFont);
	DeleteObject(hFont);
}
