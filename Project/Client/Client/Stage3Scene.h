#pragma once
#include "Scene.h"

class CStage3Scene : public CScene
{
private:
	class CObj* m_StageLimitTime;
	int m_iBallSize;
	int m_iBallTagCount;

public:
	CStage3Scene();
	virtual ~CStage3Scene();

public:
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update( const float& fTimeDelta ) override;
	virtual void Render( HDC hDC ) override;
};

