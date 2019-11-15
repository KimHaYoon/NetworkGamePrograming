#pragma once
#include "Obj.h"
class CLobbyBackground :
	public CObj
{
private:
	int		m_iColorG;
public:
	CLobbyBackground();
	~CLobbyBackground();


	virtual bool Init();
	virtual void Input();
	virtual void Update( DWORD dwTime );
	virtual void Render( HDC hDC );
};

