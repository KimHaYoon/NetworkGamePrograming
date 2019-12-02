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

	BULLETINFO tInfo = GET_SINGLE(CNetwork)->GetBulletInfo(m_tInfo.id / 10, m_tInfo.id % 10);

	m_tInfo = tInfo;

	SetSize(30.f, m_tInfo.height);
	SetPos(m_tInfo.x, m_tInfo.y);
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
	m_tSize.y = tInfo.height;
	m_tSize.x = 30.f;
	m_tInfo.height = m_tSize.y;

	SetPos(m_tInfo.x, m_tInfo.y);
}

bool CBullet::GetShot()
{
	return m_tInfo.shot;
}
