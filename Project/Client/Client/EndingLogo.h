#pragma once
#include "Obj.h"
class CEndingLogo :
	public CObj
{
public:
	CEndingLogo();
	virtual ~CEndingLogo();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);
};

