#pragma once
#include "Obj.h"
class CPlayerBullet :
	public CObj
{
	int m_iID;
	int m_iCount;
public:
	CPlayerBullet();
	virtual ~CPlayerBullet();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetPlayer(int ID);
	void SetCount(int count);
};

