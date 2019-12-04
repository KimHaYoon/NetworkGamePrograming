#include "Tile.h"



CTile::CTile()
{
}


CTile::~CTile()
{
}

bool CTile::Init()
{
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

	m_fFrame = 0.f;
	m_iNowFrame = 0.f;
	m_bRender = true;
	return true;
}

void CTile::Input()
{
	CObj::Input();
}

void CTile::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);

	if (m_tInfo.animation && m_bRender)
	{
		m_fFrame += (fTimeDelta * 50.f);
		if (m_fFrame > 1.f)
		{
			m_iNowFrame++;
			m_fFrame = 0.f;
		}
		if (m_iNowFrame == m_tInfo.maxFrame)
		{
			m_bRender = false;
			m_iNowFrame = 0;
			m_fFrame = 0.f;
		}

		if (m_tInfo.color == 0) // blue
		{
			wstring str = L"Texture/Block/BlueBlock/BlueBlock_" + to_wstring(m_iNowFrame) + L".bmp";
			SetTexture("BlueBlock" + to_string(m_iNowFrame), str.c_str());
		}
		else if (m_tInfo.color == 1) // yellow
		{
			wstring str = L"Texture/Block/YellowBlock/YellowBlock_" + to_wstring(m_iNowFrame) + L".bmp";
			SetTexture("YellowBlock" + to_string(m_iNowFrame), str.c_str());
		}
	}
}

void CTile::Render(HDC hDC)
{
	if (m_bRender == false)
		return;

	CObj::Render(hDC);
}

void CTile::SetTileInfo(TILEINFO tInfo)
{
	m_tInfo = tInfo;
	_cprintf("SetTileInfo : %d\n", m_tInfo.animation);

	SetSize(m_tInfo.cx, m_tInfo.cy);
	SetPos(m_tInfo.x, m_tInfo.y);
}

TILEINFO CTile::GetTileInfo()
{
	return m_tInfo;
}
