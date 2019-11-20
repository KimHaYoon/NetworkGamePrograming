#include "Tile.h"



CTile::CTile()
{
}


CTile::~CTile()
{
}

bool CTile::Init()
{
	return true;
}

void CTile::Input()
{
	CObj::Input();
}

void CTile::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);
}

void CTile::Render(HDC hDC)
{
	CObj::Render(hDC);
}

void CTile::SetTileInfo(TILEINFO tInfo)
{
	m_tInfo = tInfo;

	if (m_tInfo.type == 0) //
	{

	}
	else if (m_tInfo.type == 1) // ºÎ¼­Áü
	{
		SetTexture("BrokenBlock", L"Texture/Block/YellowBlock.bmp", true, RGB(0, 0, 0));
	}
	else if (m_tInfo.type == 2) // ¾ÈºÎ¼­Áü
	{
		SetTexture("UnBroken", L"Texture/Block/UnBroken.bmp", true, RGB(0, 0, 0));
	}
	SetRect();
	SetSize(m_tInfo.cx, m_tInfo.cy);
	SetPos(m_tInfo.x, m_tInfo.y);
}

void CTile::SetRect()
{
	m_rcCollisionBox.left = m_tInfo.x - m_tInfo.cx;
	m_rcCollisionBox.top = m_tInfo.y - m_tInfo.cy;
	m_rcCollisionBox.right = m_tInfo.x + m_tInfo.cx;
	m_rcCollisionBox.bottom = m_tInfo.y + m_tInfo.cy;
}

void CTile::SetCollisionBox(RECT rc)
{
	m_rcCollisionBox = rc;
}

TILEINFO CTile::GetTileInfo()
{
	return m_tInfo;
}

RECT CTile::GetCollisionBox()
{
	return m_rcCollisionBox;
}
