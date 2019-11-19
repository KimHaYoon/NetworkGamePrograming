#pragma once
#include "Obj.h"
class CLogoObj :
	public CObj
{
public:
	CLogoObj();
	~CLogoObj();

	virtual bool Init();
	virtual void Input();
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );
};

