#pragma once
#include "main.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual bool Init() = 0;
	virtual void Input() = 0;
	virtual void Update( const float& fTimeDelta ) = 0;
	virtual void Render( HDC hDC ) = 0;
};

