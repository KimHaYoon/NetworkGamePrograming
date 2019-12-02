#pragma once
#include "Scene.h"

class CObj;

class CStage1Scene :
	public CScene
{
	CObj* m_StageLimitTime;
public:
	CStage1Scene();
	virtual ~CStage1Scene();


	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update( const float& fTimeDelta ) override;
	virtual void Render( HDC hDC ) override;
};

