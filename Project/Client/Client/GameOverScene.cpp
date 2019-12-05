#include "GameOverScene.h"
#include "GameOverLogo.h"
#include "ObjectManager.h"


CGameOverScene::CGameOverScene()
{
}


CGameOverScene::~CGameOverScene()
{
}

bool CGameOverScene::Init()
{
	CObj* pGameOver = GET_SINGLE(CObjectManager)->CreateObject<CGameOverLogo>("GAME OVER");
	return true;
}

void CGameOverScene::Input()
{
}

void CGameOverScene::Update(const float & fTimeDelta)
{
}

void CGameOverScene::Render(HDC hDC)
{
}
