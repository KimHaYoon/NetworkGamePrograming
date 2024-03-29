#pragma once
#include "Obj.h"
class CPlayerPortrait :
	public CObj
{
private:
	int		m_iNumber;

public:
	CPlayerPortrait();
	~CPlayerPortrait();


	virtual bool Init();
	virtual void Input();
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );

public:
	void SetNumber( int iNumber );
};

