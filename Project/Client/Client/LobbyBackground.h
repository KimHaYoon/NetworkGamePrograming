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
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );
};

