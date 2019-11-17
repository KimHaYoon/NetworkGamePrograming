#pragma once

#include "Obj.h"
class CRect :
	public CObj
{
private:
	COLORREF m_dwColorKey;

public:
	CRect();
	~CRect();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init();
	virtual void Input();
	virtual void Update( DWORD dwTime );
	virtual void Render( HDC hDC );

public:
	void SetColor( COLORREF dwColorKey );
};

