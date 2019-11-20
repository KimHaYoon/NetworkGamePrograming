#include "Ball.h"



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

	m_nGravity = (m_tPos.y + 80) * 1.5 * fTimeDelta; // ���ϼӵ�
	if (m_nGravity <= 1)
		m_bDirUD = DIR_DOWN;


	if (m_bDirLR == DIR_LEFT)
	{
		m_tPos.x -= 70 * fTimeDelta;

		if (m_tPos.x - m_tInfo.radius < -10) // ���� ���� �浹
			m_bDirLR = DIR_RIGHT;
	}
	else if (m_bDirLR == DIR_RIGHT)
	{
		m_tPos.x += 70 * fTimeDelta;

		if (m_tPos.x + m_tInfo.radius > WINX - 35) // ������ ���� �浹
			m_bDirLR = DIR_LEFT;
	}

	if (m_bDirUD == DIR_DOWN)
	{
		m_tPos.y += m_nGravity;

		if (m_tPos.y + m_tInfo.radius > 400) // �ٴڿ� �浹
			m_bDirUD = DIR_UP;
	}
	else if (m_bDirUD == DIR_UP)
	{
		m_tPos.y -= m_nGravity;

		if (m_tInfo.y - m_tInfo.radius < 150) // �ִ� ���� ��
			m_bDirUD = DIR_DOWN;
	}

	SetRect();
}

void CBall::Render(HDC hDC)
{
	CObj::Render(hDC);
}






void CBall::SetBallInfo(BALLINFO tInfo)
{
	m_tInfo = tInfo;
	m_tSize.x = m_tSize.y = m_tInfo.radius * 2;
	if (m_tInfo.type == 'A')
	{
		SetTexture("LargeBall", L"Texture/Ball/Ball1.bmp", true, RGB(0, 0, 0));
		//m_tSize.x = m_tSize.y = 50;
	}
	else if (m_tInfo.type == 'B')
	{
		SetTexture("MidiumBall", L"Texture/Ball/Ball2.bmp", true, RGB(0, 0, 0));
		//m_tSize.x = m_tSize.y = 30;
	}
	else if (m_tInfo.type == 'C')
	{
		SetTexture("SmallBall", L"Texture/Ball/Ball3.bmp", true, RGB(0, 0, 0));
		//m_tSize.x = m_tSize.y = 15;
	}
	SetRect();
	SetPos(m_tInfo.x, m_tInfo.y);
}

void CBall::SetGravity(int nGravity)
{
	m_nGravity = nGravity;
}

void CBall::SetCollisionBox(RECT rc)
{
	m_rcCollisionBox = rc;
}

void CBall::SetRect()
{
	m_rcCollisionBox.left = m_tInfo.x - (m_tInfo.radius - 3);
	m_rcCollisionBox.top = m_tInfo.y - (m_tInfo.radius - 3);
	m_rcCollisionBox.right = m_tInfo.x + (m_tInfo.radius - 3);
	m_rcCollisionBox.bottom = m_tInfo.y + (m_tInfo.radius - 3);
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

RECT CBall::GetCollisionBox()
{
	return m_rcCollisionBox;
}

bool CBall::GetDirLR()
{
	return m_bDirLR;
}

bool CBall::GetDirUD()
{
	return m_bDirUD;
}
