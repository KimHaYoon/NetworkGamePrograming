#pragma once
#include "Obj.h"

typedef struct Tile
{
	int id;
	int x;
	int y;

	int cx; // ������
	int cy; // ������

	int type; // 0 ���� 1 �μ��� 2 �Ⱥμ���
	int color; // 0 ��� 1 ���ο�

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

	// CObj��(��) ���� ��ӵ�
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

