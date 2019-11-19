#pragma once
#include "main.h"

class CGameFramework
{
private:
	HDC		m_hDC;
	HWND m_hWnd;
	class CTexture* m_pBackBuffer;

public:
	CGameFramework();
	~CGameFramework();

public:
	void Logic(const float& fTimeDelta );
	bool Init( HWND hWnd, HINSTANCE  hInst );

private:
	void Input();
	void Update( const float& fTimeDelta );
	void Render();
};

