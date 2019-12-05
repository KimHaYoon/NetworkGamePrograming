#include "Item.h"



CItem::CItem()
{
}


CItem::~CItem()
{
}

bool CItem::Init()
{
	return true;
}

void CItem::Input()
{
	CObj::Input();
}

void CItem::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);
}

void CItem::Render(HDC hDC)
{
	CObj::Render(hDC);

	Die();
}

void CItem::SetItemInfo(ITEMINFO tInfo)
{
	m_tInfo = tInfo;

	if (m_tInfo.type == 0)
	{
		SetTexture("BulletItem", L"Texture/Item/DoubleShot.bmp", true, RGB(0, 0, 0));
	}
	else if (m_tInfo.type == 1)
	{
		SetTexture("TimerItem", L"Texture/Item/StopTime.bmp", true, RGB(0, 0, 0));
	}
	else if (m_tInfo.type == 2)
	{
		SetTexture("TimerItem", L"Texture/Item/Life.bmp", true, RGB(27, 24, 24));
	}

	SetSize(30.f, 30.f);
	SetPos(m_tInfo.x, m_tInfo.y);
}