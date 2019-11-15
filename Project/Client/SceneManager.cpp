#include "SceneManager.h"
#include "Scene.h"
#include "LogoScene.h"
#include "ObjectManager.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager():
	m_pCurScene(NULL)
{
}


CSceneManager::~CSceneManager()
{
	SAFE_DELETE( m_pCurScene );
}


bool CSceneManager::Init()
{
	CScene* pLogo = new CLogoScene;

	pLogo->Init();

	m_pCurScene = pLogo;

	return true;
}

void CSceneManager::Input()
{
	m_pCurScene->Input();
}

void CSceneManager::Update()
{
	m_pCurScene->Update();
}

void CSceneManager::Render( HDC hDC )
{
	m_pCurScene->Render( hDC );
}

void CSceneManager::SceneChange( CScene * pScene )
{
	SAFE_DELETE( m_pCurScene );
	GET_SINGLE( CObjectManager )->ClearObject();
	m_pCurScene = pScene;
	m_pCurScene->Init();
}
