#pragma once
#include "Obj.h"

class CPlayerHP :
	public CObj
{
	int m_iHP;
	int m_iID;
public:
	CPlayerHP();
	~CPlayerHP();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetPlayer(int ID);
};