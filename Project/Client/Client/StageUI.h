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
	virtual void Update( DWORD dwTime );
	virtual void Render( HDC hDC );
};

