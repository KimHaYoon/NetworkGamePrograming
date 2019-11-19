#include "Bullet.h"



CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	SetTexture("Bullet", L"Texture/Missle/Missle.bmp", true, RGB(0, 0, 0));
	//SetTexture("Player1", L"Texture/Player1/Player.right_1.bmp", true, RGB(0, 255, 0));
	return true;
}

void CBullet::Input()
{
	CObj::Input();
}

void CBullet::Update(const float& fTimeDelta)
{
	if (m_bShot)
	{
		CObj::Update(fTimeDelta);
		m_tPos.y -= (200.f * fTimeDelta);
		m_tSize.y += (200.f * fTimeDelta);

		m_rcCollisionBox.top -= (200.f * fTimeDelta);

		if (m_tPos.y < 25.f)
			m_bShot = false;
	}
}

void CBullet::Render(HDC hDC)
{
	if(m_bShot)
		CObj::Render(hDC);
}

void CBullet::Shot()
{
	m_bShot = true;
}

void CBullet::SetShot(bool shot)
{
	m_bShot = shot;
}

void CBullet::SetBulletInfo(BULLETINFO tInfo)
{
	m_tInfo = tInfo;
	m_tInfo.x += 15.f;
	m_tSize.y = 70.f;
	m_tSize.x = 30.f;

	m_rcCollisionBox.left = tInfo.x - 15.f;
	m_rcCollisionBox.right = tInfo.x + 15.f;
	m_rcCollisionBox.top = tInfo.y;
	m_rcCollisionBox.bottom = tInfo.y;

	SetPos(m_tInfo.x, m_tInfo.y);
}

bool CBullet::GetShot()
{
	return m_bShot;
}
