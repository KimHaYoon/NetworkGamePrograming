#include "Item.h"



CItem::CItem()
{
}


CItem::~CItem()
{
}

bool CItem::Init()
{
	return false;
}

void CItem::Input()
{
}

void CItem::Update(const float & fTimeDelta)
{
}

void CItem::Render(HDC hDC)
{
}

void CItem::SetItemInfo(ITEMINFO tInfo)
{
	m_tInfo = tInfo;


	// Type == 'A' 체력회복
	// Type == 'B' 최대탄수 증가
	// Type == 'C' 시간정지
	if (m_tInfo.type == 'A')
	{

	}
	else if (m_tInfo.type == 'B')
	{

	}
	else if (m_tInfo.type == 'C')
	{

	}
	SetPos(m_tInfo.x, m_tInfo.y);
}