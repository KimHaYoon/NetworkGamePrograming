#include "PlayerHP.h"
#include "Network.h"

#include "Input.h"

CPlayerHP::CPlayerHP()
{
}


CPlayerHP::~CPlayerHP()
{
}

bool CPlayerHP::Init()
{
	m_iHP = 3;
	return true;
}

void CPlayerHP::Input()
{
	CObj::Input();

	/*if (KEYDOWN("MoveLeft"))
	{
		if (m_iHP > 0)
			m_iHP--;
	}

	if (KEYDOWN("MoveRight"))
	{
		if (m_iHP < 3)
			m_iHP++;
	}*/
}

void CPlayerHP::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);

	if ( m_iID == 1 )
		m_iHP = GET_SINGLE( CNetwork )->GetPlayerInfo().hp;
	else
		m_iHP = GET_SINGLE( CNetwork )->GetOtherPlayerInfo().hp;
}

void CPlayerHP::Render(HDC hDC)
{
	CObj::Render(hDC);

	string strHP = "";
	if (m_iID == 1)
	{
		for (int i = 0; i < m_iHP; ++i)
		{
			strHP += "¡á";
		}
		for (int i = m_iHP; i < 3; ++i)
		{
			strHP += "¡à";
		}
	}
	else
	{
		for (int i = m_iHP; i < 3; ++i)
		{
			strHP += "¡à";
		}
		for (int i = 0; i < m_iHP; ++i)
		{
			strHP += "¡á";
		}
	}

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(200, 0, 0));
	HFONT hFont = CreateFont(50, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("µÕ±Ù¸ð²Ã"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	TextOut(hDC, m_tPos.x, m_tPos.y, strHP.c_str(), strHP.size());
	SelectObject(hDC, oldFont);
	DeleteObject(hFont);
}

void CPlayerHP::SetPlayer(int ID)
{
	m_iID = ID;
}






