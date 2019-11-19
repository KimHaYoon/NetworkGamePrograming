#pragma once
#include "Obj.h"
class CInputServerIP :
	public CObj
{
private:
	string m_strServerIP;

public:
	CInputServerIP();
	~CInputServerIP();

	virtual bool Init();
	virtual void Input();
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );

public:
	void SetServerIP( const string& strServerIP );
	string GetServerIP() const;
};

