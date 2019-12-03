#include "PlayerBullet.h"
#include "Network.h"


CPlayerBullet::CPlayerBullet()
{
}


CPlayerBullet::~CPlayerBullet()
{

}

bool CPlayerBullet::Init()
{
	m_iCount = 5;
	return true;
}

void CPlayerBullet::Input()
{
	CObj::Input();
}

void CPlayerBullet::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);

	if(m_iID == 1)
		m_iCount = GET_SINGLE(CNetwork)->GetPlayerInfo().bulletCount;
	else
		m_iCount = GET_SINGLE(CNetwork)->GetOtherPlayerInfo().bulletCount;
}

void CPlayerBullet::Render(HDC hDC)
{
	CObj::Render(hDC);

	string strHP = "";
	if (m_iID == 1)
	{
		for (int i = 0; i < m_iCount; ++i)
		{
			strHP += "¡á";
		}
		for (int i = m_iCount; i < 5; ++i)
		{
			strHP += "¡à";
		}
	}
	else
	{
		for (int i = m_iCount; i < 5; ++i)
		{
			strHP += "¡à";
		}
		for (int i = 0; i < m_iCount; ++i)
		{
			strHP += "¡á";
		}
	}

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(0, 200, 200));
	HFONT hFont = CreateFont(20, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("µÕ±Ù¸ð²Ã"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	TextOut(hDC, m_tPos.x, m_tPos.y, strHP.c_str(), strHP.size());
	SelectObject(hDC, oldFont);
	DeleteObject(hFont);
}

void CPlayerBullet::SetPlayer(int ID)
{
	m_iID = ID;
}

void CPlayerBullet::SetCount(int count)
{
	m_iCount = count;
}
