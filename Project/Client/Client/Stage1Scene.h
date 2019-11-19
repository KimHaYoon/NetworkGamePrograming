#pragma once
#include "Scene.h"
class CStage1Scene :
	public CScene
{
public:
	CStage1Scene();
	virtual ~CStage1Scene();


	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update( const float& fTimeDelta ) override;
	virtual void Render( HDC hDC ) override;
};

