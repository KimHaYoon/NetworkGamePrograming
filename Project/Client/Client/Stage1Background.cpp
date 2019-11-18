#include "Stage1Background.h"



CStage1Background::CStage1Background()
{
}


CStage1Background::~CStage1Background()
{
}

bool CStage1Background::Init()
{
	SetTexture( "Stage1", L"Texture/Stage/Stage1_Background.bmp" );

	return true;
}

void CStage1Background::Input()
{
	CObj::Input();
}

void CStage1Background::Update( DWORD dwTime )
{
	CObj::Update( dwTime );
}

void CStage1Background::Render( HDC hDC )
{
	CObj::Render( hDC );
}
