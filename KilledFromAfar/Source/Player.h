#pragma once

class Unit;

#define MOVEMENT_MIN -1.f
#define MOVEMENT_MAX 1.f

#define ROTY_MIN -85.f
#define ROTY_MAX 85.f

class Player
{
public:
	Player();
	~Player();

	virtual void OnRespawn() = 0;

	virtual void Update() final;

	struct Input
	{
		float move;
		float strafe;
		float rotX;
		float rotY;
		bool crouch;
		bool shoot;
		bool aimDownSight;
		bool reload;
	};

	virtual void SetInput(const Input& acInput) final;

	enum EPlayerType
	{
		AI,
		Remote,
#if USE_LOCAL_PLAYER
		Local,
#endif
	};

	virtual EPlayerType GetType() = 0;

protected:
	virtual void CollectInput() = 0;

	Unit* m_pParent;

	Input m_input;

};