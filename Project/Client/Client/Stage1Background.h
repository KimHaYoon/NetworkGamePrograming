#pragma once
#include "Obj.h"
class CStage1Background :
	public CObj
{
public:
	CStage1Background();
	virtual ~CStage1Background();

	virtual bool Init();
	virtual void Input();
	virtual void Update( DWORD dwTime );
	virtual void Render( HDC hDC );
};

