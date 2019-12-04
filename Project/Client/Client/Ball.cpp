#include "Ball.h"
#include "Network.h"


CBall::CBall()
{
}


CBall::~CBall()
{
}

bool CBall::Init()
{
	return true;
}

void CBall::Input()
{
	CObj::Input();
}

void CBall::Update(const float & fTimeDelta)
{

	CObj::Update(fTimeDelta);

	if ( GET_SINGLE( CNetwork )->GetServerOn() )
		return;

	m_nGravity = (m_tPos.y + 80) * 1.5 * fTimeDelta; // 낙하속도
	if (m_nGravity <= 1)
		m_bDirUD = DIR_DOWN;


	if (m_bDirLR == DIR_LEFT)
	{
		m_tPos.x -= 70 * fTimeDelta;

		if (m_tPos.x - m_tInfo.radius < -10) // 왼쪽 벽에 충돌
			m_bDirLR = DIR_RIGHT;
	}
	else if (m_bDirLR == DIR_RIGHT)
	{
		m_tPos.x += 70 * fTimeDelta;

		if (m_tPos.x + m_tInfo.radius > WINX - 35) // 오른쪽 벽에 충돌
			m_bDirLR = DIR_LEFT;
	}

	if (m_bDirUD == DIR_DOWN)
	{
		m_tPos.y += m_nGravity;

		if (m_tPos.y + m_tInfo.radius > 400) // 바닥에 충돌
			m_bDirUD = DIR_UP;
	}
	else if (m_bDirUD == DIR_UP)
	{
		m_tPos.y -= m_nGravity;

		if (m_tInfo.y - m_tInfo.radius < 150) // 최대 높이 값
			m_bDirUD = DIR_DOWN;
	}
}

void CBall::Render(HDC hDC)
{
	string strType = to_string( m_tInfo.type );
	char type = strType[1];

	if ( type == '0' )
		return;

	CObj::Render(hDC);
}

void CBall::SetBallInfo(BALLINFO tInfo)
{
	m_tInfo = tInfo;
	m_tSize.x = m_tSize.y = m_tInfo.radius * 2;

	string strType = to_string( m_tInfo.type );
	char type = strType[1];

	if (type == '1')
	{
		SetTexture("LargeBall", L"Texture/Ball/Ball1.bmp", true, RGB(0, 0, 0));
		//m_tSize.x = m_tSize.y = 50;
	}
	else if (type == '2')
	{
		SetTexture("MidiumBall", L"Texture/Ball/Ball2.bmp", true, RGB(0, 0, 0));
		//m_tSize.x = m_tSize.y = 30;
	}
	else if (type == '3')
	{
		SetTexture("SmallBall", L"Texture/Ball/Ball3.bmp", true, RGB(0, 0, 0));
		//m_tSize.x = m_tSize.y = 15;
	}
	
	SetPos(m_tInfo.x, m_tInfo.y);
}

void CBall::SetGravity(int nGravity)
{
	m_nGravity = nGravity;
}

void CBall::SetDirLR(bool LR)
{
	m_bDirLR = LR;
}

void CBall::SetDirUD(bool UD)
{
	m_bDirUD = UD;
}

BALLINFO CBall::GetBallInfo()
{
	return m_tInfo;
}

int CBall::GetGravity()
{
	return m_nGravity;
}

bool CBall::GetDirLR()
{
	return m_bDirLR;
}

bool CBall::GetDirUD()
{
	return m_bDirUD;
}
