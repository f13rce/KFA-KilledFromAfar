#pragma once

#include "Player.h"
#include "Weapon.h"

#include <memory>

#define RENDER_WIDTH 320
#define RENDER_HEIGHT 240

class Unit
{
public:
	Unit(int32_t aID);
	~Unit();

	void SetPlayer(std::unique_ptr<Player> apPlayer);

	void EnableRendering();

	void SetInput(const Player::Input& aInput);

	void Update();
	void PhysicsUpdate();

	void TakeDamage(uint8_t aDamage);
	void Respawn(Entity* apSpawn);

	enum ETeam
	{
		Attackers,
		Defenders,
	};
	void SetTeam(ETeam aTeam);
	ETeam GetTeam() const;

	int32_t GetID() const;

	Texture* GetRenderTexture();
	Player::EPlayerType GetPlayerType() const;
	Player* GetPlayer();

	Entity* GetPhysicsModel();
	Entity* GetAppearanceModel();
	Entity* GetHead();

	Weapon* GetWeapon();

	uint8_t GetHealth() const;
	bool IsDead() const;

protected:
	void ApplyInput();

	void OnDeath();
	void UpdateDeathAnimation();

	int32_t m_id;

	std::unique_ptr<Player> m_pPlayer;
	Player::Input m_input;
	Vec3 m_previousRotation;

	Entity* m_pPhysicsModel;
	Entity* m_pModel;
	Entity* m_pHead;
	Entity* m_pAimPivot;
	std::vector<Entity*> m_rotUpPivots;
	std::vector<float> m_defaultRotations;
	static constexpr float s_cModelHeight = 1.28f;

	float m_deathTime;
	static constexpr float s_cRespawnTime = 3.f;

	ETeam m_team;

	std::unique_ptr<Weapon> m_pWeapon;
	
	struct Stats
	{
		uint8_t health;
		float moveSpeed; // Units per second
	};

	Stats m_base;
	Stats m_current;

	float m_cachedYRot;

	std::vector<Stats> m_modifiers;

	bool m_canRender;
	Texture* m_pTexture;
	static constexpr float s_cNormalFOV = 65.f;
	static constexpr float s_cZoomFOV = 45.f;

};