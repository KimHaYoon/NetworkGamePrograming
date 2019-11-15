#pragma once
#include "Scene.h"
class CLobbyScene :
	public CScene
{
public:
	CLobbyScene();
	~CLobbyScene();


	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update() override;
	virtual void Render( HDC hDC ) override;
};

