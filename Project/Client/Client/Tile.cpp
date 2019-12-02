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
	m_fFrame = 0.f;

	if (m_tInfo.color == 0) // Blue
	{
		SetTexture("BlueBlock1", L"Texture/Block/BlueBlock/BlueBlock_1.bmp", true, RGB(0, 0, 0));
		SetTexture("BlueBlock2", L"Texture/Block/BlueBlock/BlueBlock_2.bmp", true, RGB(0, 0, 0));
		SetTexture("BlueBlock3", L"Texture/Block/BlueBlock/BlueBlock_3.bmp", true, RGB(0, 0, 0));
		SetTexture("BlueBlock4", L"Texture/Block/BlueBlock/BlueBlock_4.bmp", true, RGB(0, 0, 0));
		SetTexture("BlueBlock0", L"Texture/Block/BlueBlock/BlueBlock_0.bmp", true, RGB(0, 0, 0));
	}
	else if (m_tInfo.color == 1) // Yellow
	{
		SetTexture("YellowBlock1", L"Texture/Block/YellowBlock/YellowBlock_1.bmp", true, RGB(0, 0, 0));
		SetTexture("YellowBlock2", L"Texture/Block/YellowBlock/YellowBlock_2.bmp", true, RGB(0, 0, 0));
		SetTexture("YellowBlock3", L"Texture/Block/YellowBlock/YellowBlock_3.bmp", true, RGB(0, 0, 0));
		SetTexture("YellowBlock4", L"Texture/Block/YellowBlock/YellowBlock_4.bmp", true, RGB(0, 0, 0));
		SetTexture("YellowBlock0", L"Texture/Block/YellowBlock/YellowBlock_0.bmp", true, RGB(0, 0, 0));
	}
	SetSize(m_tInfo.cx, m_tInfo.cy);
	SetPos(m_tInfo.x, m_tInfo.y);
}

TILEINFO CTile::GetTileInfo()
{
	return m_tInfo;
}
