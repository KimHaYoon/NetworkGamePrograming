#pragma once
#include "Scene.h"
class CLogoScene :
	public CScene
{
public:
	CLogoScene();
	virtual ~CLogoScene();

	// CScene을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update() override;
	virtual void Render( HDC hDC ) override;
};

