#pragma once
#include "Obj.h"
class CStageUI :
	public CObj
{
public:
	CStageUI();
	virtual ~CStageUI();

	virtual bool Init();
	virtual void Input();
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );
};

