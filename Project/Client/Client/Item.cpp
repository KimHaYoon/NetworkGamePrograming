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


	// Type == 'A' ü��ȸ��
	// Type == 'B' �ִ�ź�� ����
	// Type == 'C' �ð�����
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