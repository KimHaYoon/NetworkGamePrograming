#pragma once
#include "Obj.h"
class CMarker :
	public CObj
{
public:
	CMarker();
	virtual ~CMarker();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);
};

