#include "Bullet.h"
#include "Network.h"


CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	SetTexture("Bullet", L"Texture/Missle/Missle.bmp", true, RGB(0, 0, 0));
	return true;
}

void CBullet::Input()
{
	CObj::Input();
}

void CBullet::Update(const float& fTimeDelta)
{
	CObj::Update(fTimeDelta);
	//if (m_tInfo.id > 30 || m_tInfo.id < 10)
	//	return;
	if (m_tInfo.id / 10 != GET_SINGLE(CNetwork)->GetPlayerInfo().id)
	{
		m_tInfo = GET_SINGLE(CNetwork)->GetBulletInfo(m_tInfo.id / 10, m_tInfo.id % 10);
		SetPos(m_tInfo.x, m_tInfo.y);

		if (m_tInfo.shot)
		{
			m_tSize.y = m_tInfo.height;

			m_rcCollisionBox.top -= (200.f * fTimeDelta);
		}

		return;
	}
	else
	{
		m_tInfo.x = m_tPos.x;
		m_tInfo.y = m_tPos.y;
		m_tInfo.height = m_tSize.y;
		GET_SINGLE(CNetwork)->SetBulletInfo(m_tInfo.id / 10, m_tInfo.id % 10, m_tInfo);
	}

	if (m_tInfo.shot)
	{
		m_tPos.y -= (200.f * fTimeDelta);
		m_tSize.y += (200.f * fTimeDelta);

		m_rcCollisionBox.top -= (200.f * fTimeDelta);

		if (m_tPos.y < 25.f) 
		{
			m_tInfo.shot = false;
			m_tSize.y = 70.f;
			m_tSize.x = 30.f;
			m_tInfo.height = m_tSize.y;
		}

		m_tInfo.x = m_tPos.x;
		m_tInfo.y = m_tPos.y;
		m_tInfo.height = m_tSize.y;

		GET_SINGLE(CNetwork)->SetBulletInfo(m_tInfo.id / 10, m_tInfo.id % 10, m_tInfo);
	}
}

void CBullet::Render(HDC hDC)
{
	if(m_tInfo.shot)
		CObj::Render(hDC);
}

void CBullet::Shot(int x, int y)
{
	m_tInfo.x = x + 15;
	m_tInfo.y = y;
	SetPos(m_tInfo.x, m_tInfo.y);

	m_tInfo.shot = true;
}

void CBullet::SetShot(bool shot)
{
	m_tInfo.shot = shot;
}

void CBullet::SetBulletInfo(BULLETINFO tInfo)
{
	m_tInfo = tInfo;
	m_tSize.y = 70.f;
	m_tSize.x = 30.f;
	m_tInfo.height = m_tSize.y;

	m_rcCollisionBox.left = tInfo.x - 15.f;
	m_rcCollisionBox.right = tInfo.x + 15.f;
	m_rcCollisionBox.top = tInfo.y;
	m_rcCollisionBox.bottom = tInfo.y;

	SetPos(m_tInfo.x, m_tInfo.y);
}

bool CBullet::GetShot()
{
	return m_tInfo.shot;
}
