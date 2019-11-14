#pragma once
class Player
{
	int				m_nID;						// 1P, 2P

	CImage			m_imgFace;					// �� �̹���
	CImage			m_imgAnimation[5];			// �ִϸ��̼�
	CImage			m_imgLife;					// ������ �̹���
	CImage			m_imgDeathLife;				// ���� ������ �̹���
	CImage			m_imgNumber;				// ������ �̹���
	CImage			m_imgBulletIcon;			// �ۻ� �̹���

	bool			m_bPlay;					// �����÷��� ����/�Ұ���		
	bool			m_bAttack;					// �ǰ� ����
	bool			m_bMove;					// �ִϸ��̼��� ������ ����
	bool			m_bDir;						// ĳ������ ����
	bool			m_bShoot;					// �ۻ� �߻��Ҷ� �ִϸ��̼��� ų����

	int				m_nLife;					// HP

	int				m_nInvincible_Time;			// �ǰ� ���� �����ð�
	int				m_nScore;					// ����
	int				m_nScoreImage[5];			// �������
	float			m_fSpeed;					// �̵� �Ÿ�

	RECT			m_rcPosition;				// ������

	// ---------------------------------------- //
	RECT			m_rcBullet[5];				// �ۻ��
	bool			m_bBullet_Shot[5];			// �ۻ� �߻� ����
	int				m_nBullet_Count;			// �ۻ� �ִ� ����
	int				m_nBullet_Height[5];		// �ۻ� ����
	int				m_nBulletImage_Height[5];	// �ۻ� �׸��� ����
	int				m_nBulletImage_Size;		// �ۻ� �׸��� �þ�� ũ��
	// ---------------------------------------- //

public:
	Player();
	~Player();

	HRESULT			Initalize(HINSTANCE g_hInst, int number);

	void			Player_Animation(HINSTANCE g_hInst, HDC memDC);

	int				Update(HINSTANCE g_hInst, HDC memDC);
	int				Render(HDC hDC);
	int				BulletRender(HDC hDC);

	void			IncreaseBulletCount();
	void			IncreaseInvincibleTime();

	void			SetPlay(bool play);
	void			SetLife(int life);
	void			SetShoot(bool shoot);
	void			SetPosition(RECT rt);
	void			SetAttack(bool attack);
	void			SetDir(bool dir);
	void			SetMove(bool move);
	void			SetBulletShot(bool bulletshot, int index);
	void			SetScore(int score);
	void			SetBulletPosition(RECT rt, int index);
	void			SetBulletHeight(int height, int index);
	void			SetBulletImageHeight(int height, int index);
	void			SetInvincibleTime(int time);

	bool			GetPlay();
	int				GetLife();
	bool			GetShoot();
	RECT			GetPosition();
	bool			GetAttack();
	bool			GetDir();
	bool			GetMove();
	bool			GetBulletShoot(int index);
	int				GetScore();
	int				GetScore(int index);
	RECT			GetBulletPosition(int index);
	int				GetBulletHeight(int index);
	int				GetBulletImageHeight(int index);
	int				GetInvincibleTime();
};

