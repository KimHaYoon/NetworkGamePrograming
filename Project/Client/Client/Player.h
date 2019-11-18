#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
private:
	PLAYERINFO m_tInfo;

public:
	CPlayer();
	virtual ~CPlayer();

	virtual bool Init();
	virtual void Input();
	virtual void Update( DWORD dwTime );
	virtual void Render( HDC hDC );

public:
	void SetPlayerInfo(const PLAYERINFO& tInfo);
};

