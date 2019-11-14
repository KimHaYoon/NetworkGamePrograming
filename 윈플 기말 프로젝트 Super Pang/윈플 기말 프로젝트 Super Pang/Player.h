#pragma once
class Player
{
	int				m_nID;						// 1P, 2P

	CImage			m_imgFace;					// 얼굴 이미지
	CImage			m_imgAnimation[5];			// 애니메이션
	CImage			m_imgLife;					// 라이프 이미지
	CImage			m_imgDeathLife;				// 죽은 라이프 이미지
	CImage			m_imgNumber;				// 점수판 이미지
	CImage			m_imgBulletIcon;			// 작살 이미지

	bool			m_bPlay;					// 게임플레이 가능/불가능		
	bool			m_bAttack;					// 피격 판정
	bool			m_bMove;					// 애니메이션을 돌릴지 말지
	bool			m_bDir;						// 캐릭터의 방향
	bool			m_bShoot;					// 작살 발사할때 애니메이션을 킬건지

	int				m_nLife;					// HP

	int				m_nInvincible_Time;			// 피격 판정 무적시간
	int				m_nScore;					// 점수
	int				m_nScoreImage[5];			// 점수출력
	float			m_fSpeed;					// 이동 거리

	RECT			m_rcPosition;				// 포지션

	// ---------------------------------------- //
	RECT			m_rcBullet[5];				// 작살들
	bool			m_bBullet_Shot[5];			// 작살 발사 여부
	int				m_nBullet_Count;			// 작살 최대 개수
	int				m_nBullet_Height[5];		// 작살 길이
	int				m_nBulletImage_Height[5];	// 작살 그림의 길이
	int				m_nBulletImage_Size;		// 작살 그림이 늘어나는 크기
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

