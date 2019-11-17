#pragma once
#include "Obj.h"
class CEnterKeyObj :
	public CObj
{
private:
	bool m_bShow;

public:
	CEnterKeyObj();
	~CEnterKeyObj();


	virtual bool Init();
	virtual void Input();
	virtual void Update( DWORD dwTime );
	virtual void Render( HDC hDC );
};

