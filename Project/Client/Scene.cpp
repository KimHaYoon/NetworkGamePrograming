#include "Scene.h"
#include "ObjectManager.h"
#include "Obj.h"
#include "LogoObj.h"
#include "EnterKeyObj.h"
#include "InputServerIP.h"

DEFINITION_SINGLE( CScene )

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::Init()
{
	CObj* pLogo = GET_SINGLE( CObjectManager )->CreateObject<CLogoObj>( "LogoObj" );
	CObj* pEnterKey = GET_SINGLE( CObjectManager )->CreateObject<CEnterKeyObj>( "EnterKeyObj" );
	CObj* pInputServerIP = GET_SINGLE( CObjectManager )->CreateObject<CInputServerIP>( "InputServerIP" );
	return true;
}