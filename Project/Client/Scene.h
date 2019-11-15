#pragma once
#include "main.h"

class CScene
{
public:
	CScene();
	~CScene();

public:
	virtual bool Init() = 0;
	virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Render( HDC hDC ) = 0;
};

