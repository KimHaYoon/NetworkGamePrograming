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

void CStage1Background::Update( const float& fTimeDelta )
{
	CObj::Update( fTimeDelta );
}

void CStage1Background::Render( HDC hDC )
{
	CObj::Render( hDC );
}
