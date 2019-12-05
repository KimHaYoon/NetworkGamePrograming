#include "EndingScene.h"
#include "EndingLogo.h"
#include "ObjectManager.h"


CEndingScene::CEndingScene()
{
}


CEndingScene::~CEndingScene()
{
}

bool CEndingScene::Init()
{
	CObj* pGameOver = GET_SINGLE(CObjectManager)->CreateObject<CEndingLogo>("Congratulation!!");
	return true;
}

void CEndingScene::Input()
{
}

void CEndingScene::Update(const float & fTimeDelta)
{
}

void CEndingScene::Render(HDC hDC)
{
}
