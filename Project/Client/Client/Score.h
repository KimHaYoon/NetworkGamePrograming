#pragma once
#include "Obj.h"
class CScore :
	public CObj
{
private:
	float m_fScore;

public:
	CScore();
	~CScore();

	virtual bool Init();
	virtual void Input();
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );

public:
	void SetScore( int iScore );
};

