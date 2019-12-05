#pragma once
#include "Scene.h"

class CStage2Scene : public CScene
{
private:
	class CObj* m_StageLimitTime;
	int m_iBallSize;
	int m_iBallTagCount;

public:
	CStage2Scene();
	virtual ~CStage2Scene();

public:
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update( const float& fTimeDelta ) override;
	virtual void Render( HDC hDC ) override;
};

