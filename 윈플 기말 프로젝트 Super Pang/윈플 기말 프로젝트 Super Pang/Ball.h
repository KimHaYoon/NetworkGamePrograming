#pragma once

typedef enum DirOne
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR1_END
}DIR_ONE;
typedef enum DirTwo
{
	DIR_UP,
	DIR_DOWN,
	DIR2_END
}DIR_TWO;

class Ball
{
	int					m_nX; // X��ǥ
	int					m_nY; // Y��ǥ

	int					m_nR; //������ 
	int					m_nType; // ����Ÿ��(0 R:15 / 1 R:30 / 2 R:50)
	int					m_nGravity;

	DIR_ONE				m_DirLR;
	DIR_TWO				m_DirUD;

	RECT				m_rcCollisionBox;

public:
	Ball();
	~Ball();

	HRESULT				Initialize();
	HRESULT				Initialize(int x, int y, int r, int type, DIR_ONE dirLR, DIR_TWO dirUD);
	int					Update();
	int					Render();

	void				SetPosition(int x, int y, int r, int type, int gravity, DIR_ONE dirLR, DIR_TWO dirUD);
	void				SetRect(); // Ÿ�̸ӿ��� ������ ������ �������ַ��� ��
	
	void				SetX(int x);
	void				SetY(int y);
	void				SetRadius(int r);
	void				SetType(int type);
	void				SetGravity(int gravity);
	void				SetDirLR(DIR_ONE dir);
	void				SetDirUD(DIR_TWO dir);

	int					GetX();
	int					GetY();
	int					GetRadius();
	int					GetType();
	int					GetGravity();
	DIR_ONE				GetDirLR();
	DIR_TWO				GetDirUD();
	RECT				GetCollisionBox();
};

