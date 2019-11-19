#pragma once
#include "Obj.h"
class CCircle :
	public CObj
{
public:
	CCircle();
	~CCircle();

	virtual bool Init();
	virtual void Input();
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );
};

