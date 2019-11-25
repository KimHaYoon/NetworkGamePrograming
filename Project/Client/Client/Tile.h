#pragma once
#include "Obj.h"

typedef struct Tile
{
	int id;
	int x;
	int y;

	int cx; // °¡·ÎÆø
	int cy; // ¼¼·ÎÆø

	int type; // 0 ¾øÀ½ 1 ºÎ¼­Áü 2 ¾ÈºÎ¼­Áü
	int color; // 0 ºí·ç 1 ¿»·Î¿ì

	int nowFrame;
	int maxFrame;
}TILEINFO;

class CTile :
	public CObj
{
	TILEINFO m_tInfo;

	RECT m_rcCollisionBox;
public:
	CTile();
	virtual ~CTile();

	// CObjÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetTileInfo(TILEINFO tInfo);
	void SetCollisionBox(RECT rc);
	void SetRect();
	
	TILEINFO GetTileInfo();
	RECT GetCollisionBox();
};

