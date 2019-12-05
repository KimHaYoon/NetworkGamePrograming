#pragma once
#include "Obj.h"
class CMarker :
	public CObj
{
public:
	CMarker();
	virtual ~CMarker();

	// CObj을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);
};

