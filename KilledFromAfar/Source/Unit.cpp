#include "Unit.h"
#include "AssaultRifle.h"
#include "Globals.h"
#include "Game.h"

Unit::Unit(int32_t aID)
	: m_id(aID)
	, m_pWeapon(std::make_unique<AssaultRifle>())
	, m_pPlayer(nullptr)
	, m_team(ETeam::Attackers)
	, m_pCamera(nullptr)
	, m_pTexture(nullptr)
	, m_pMaterial(nullptr)
	, m_canRender(false)
	, m_cachedYRot(0.f)
	, m_previousRotation(Vec3(0.f))
{
	m_base.health = 100;
	m_base.moveSpeed = 5.f;

	m_current = m_base;

	/*m_pPhysicsModel = Model::Cylinder();
	m_pPhysicsModel->SetScale(0.8f, 1.8f, 0.8f);
	m_pPhysicsModel->SetCollisionType(Collision::Character);
	m_pPhysicsModel->SetPhysicsMode(Entity::CharacterPhysics);
	m_pPhysicsModel->SetMass(10.f);
	m_pPhysicsModel->SetMaterial(Material::Load("Materials/Common/Invisible.mat"));*/

	m_pPhysicsModel = Model::Cylinder();
	m_pPhysicsModel->SetScale(0.8f, 1.8f, 0.8f);
	m_pPhysicsModel->SetCollisionType(Collision::Scene);
	m_pPhysicsModel->SetPhysicsMode(Entity::CharacterPhysics);
	//m_pPhysicsModel->SetPhysicsMode(Entity::RigidBodyPhysics);
	m_pPhysicsModel->SetMass(10.f);
	m_pPhysicsModel->SetMaterial(Material::Load("Materials/Common/Invisible.mat"));

	m_pModel = Prefab::Load("Prefabs/Unit.pfb");
	m_pModel->SetCollisionType(Collision::None);
	m_pModel->SetPosition(m_pPhysicsModel->GetPosition(true) + Vec3(0.f, s_cModelHeight, 0.f), true);
	m_pModel->SetRotation(m_pPhysicsModel->GetRotation(true), true);
	m_pModel->SetParent(m_pPhysicsModel);

	m_pAimPivot = m_pModel->FindChild("AimingPivot");

	m_rotUpPivots.push_back(m_pModel->FindChild("Pivot_ArmLeftUpper"));
	m_defaultRotations.push_back(m_rotUpPivots.back()->GetRotation(false).x);

	m_rotUpPivots.push_back(m_pModel->FindChild("Pivot_ArmRightUpper"));
	m_defaultRotations.push_back(m_rotUpPivots.back()->GetRotation(false).x);

	m_pHead = m_pModel->FindChild("Pivot_Head");
	m_rotUpPivots.push_back(m_pHead);
	m_defaultRotations.push_back(m_rotUpPivots.back()->GetRotation(false).x);

	m_pWeapon->Init(this, m_pAimPivot, m_pModel->FindChild("Pivot_Head"));
	Entity* pWeaponMount = m_pModel->FindChild("Pivot_ArmRightEquip");
	m_pWeapon->GetModel()->SetPosition(pWeaponMount->GetPosition(true) + Vec3(0.f, 0.16f, 0.f), true);
	m_pWeapon->GetModel()->SetRotation(pWeaponMount->GetRotation(true), true);
	m_pWeapon->GetModel()->SetParent(pWeaponMount);
}

Unit::~Unit()
{
}

void Unit::SetPlayer(std::unique_ptr<Player> apPlayer)
{
	m_pPlayer = std::move(apPlayer);
}

void Unit::EnableRendering()
{
	// DON'T ALLOW RENDERERING FOR UNITS/PLAYERS THAT DON'T NEED IT - LIKE AI
	std::cout << "Enabling rendering for Unit..." << std::endl;
	m_canRender = true;
	m_pCamera = Camera::Create();
	m_pTexture = Texture::Create(RENDER_WIDTH, RENDER_HEIGHT);
	m_pCamera->SetClearColor(Vec4(163.f / 255.f, 205.f / 255.f, 224.f / 255.f, 1.f));

	Entity* pMount = m_pModel->FindChild("Head");
	m_pCamera->SetPosition(pMount->GetPosition(true), true);
	m_pCamera->SetRotation(pMount->GetRotation(true), true);
	m_pCamera->SetParent(pMount);
	m_pCamera->SetFOV(s_cNormalFOV);

	m_pMaterial = Material::Create();
	m_pMaterial->SetShader("Shaders/Model/Diffuse.shader");
	m_pMaterial->SetTexture(m_pTexture);

	m_pCamera->SetRenderTarget(m_pTexture);

	Model* pTest = Model::Box();
	pTest->SetMaterial(m_pMaterial);
	pTest->SetPosition(Vec3(7.f, 1.f, -26.f), true);
	pTest->SetCollisionType(Collision::None);
	pTest->SetScale(0.0001f);
}

void Unit::SetInput(const Player::Input& acInput)
{
	m_input = acInput;
}

void Unit::Update()
{
	if (!IsDead())
	{
		m_pWeapon->Update();
		m_pPlayer->Update(); // Don't nullcheck this - every unit MUST have a player
	}
	else
	{
		UpdateDeathAnimation();
	}
}

void Unit::PhysicsUpdate()
{
	ApplyInput();
}

void Unit::TakeDamage(uint8_t aDamage)
{
	if (IsDead())
	{
		return;
	}

	if (aDamage > m_current.health)
	{
		m_current.health = 0;
	}
	else
	{
		m_current.health -= aDamage;
	}

	std::cout << "OUCH! HP left: " << uint32_t(m_current.health) << std::endl;

	if (m_current.health == 0)
	{
		OnDeath();
	}
}

void Unit::Respawn(Entity* apSpawn)
{
	m_current = m_base;

	const Vec3& pos = apSpawn->GetPosition(true);
	m_pPhysicsModel->SetPosition(pos.x, pos.y, pos.z);
	m_pPhysicsModel->SetVelocity(0.f, 0.f, 0.f);
	m_pPhysicsModel->SetRotation(Vec3(0.f, apSpawn->GetRotation(true).y, 0.f), true);
	//m_pPhysicsModel->SetInput(apSpawn->GetRotation(true).y, 0.f, 0.f, 0.f, false);
	
	m_pWeapon->Reset();
}

void Unit::SetTeam(ETeam aTeam)
{
	m_team = aTeam;

	Material* pPrimary = nullptr;
	Material* pSecondary = nullptr;

	switch (aTeam)
	{
		case ETeam::Attackers:
		{
			pPrimary = Material::Load("Materials/Common/Body_AtkPrimary.mat");
			pSecondary = Material::Load("Materials/Common/Body_AtkSecondary.mat");
			break;
		}

		case ETeam::Defenders:
		{
			pPrimary = Material::Load("Materials/Common/Body_DefPrimary.mat");
			pSecondary = Material::Load("Materials/Common/Body_DefSecondary.mat");
			break;
		}
	}

	std::vector<std::string> primaries = {"LegRightUpper", "LegLeftUpper", "ArmLeftUpper", "ArmRightUpper"};
	std::vector<std::string> secondaries = {"Backpack", "Plating", "LegRightLower", "LegLeftLower",
											"ArmLeftLower", "ArmRightLower", "Helmet"};

	m_pModel->SetMaterial(pPrimary);
	for (const auto& name : primaries)
	{
		auto pEntity = m_pModel->FindChild(name);
		if (pEntity)
		{
			pEntity->SetMaterial(pPrimary);
		}
	}

	for (const auto& name : secondaries)
	{
		auto pEntity = m_pModel->FindChild(name);
		if (pEntity)
		{
			pEntity->SetMaterial(pSecondary);
		}
	}
}

Unit::ETeam Unit::GetTeam() const
{
	return m_team;
}

int32_t Unit::GetID() const
{
	return m_id;
}

Texture* Unit::GetRenderTexture()
{
	return m_pMaterial->GetTexture();
}

Player::EPlayerType Unit::GetPlayerType() const
{
	return m_pPlayer->GetType();
}

Player* Unit::GetPlayer()
{
	return m_pPlayer.get();
}

Entity* Unit::GetPhysicsModel()
{
	return m_pPhysicsModel;
}

Entity* Unit::GetAppearanceModel()
{
	return m_pModel;
}

Entity* Unit::GetHead()
{
	return m_pHead;
}

Weapon* Unit::GetWeapon()
{
	return m_pWeapon.get();
}

uint8_t Unit::GetHealth() const
{
	return m_current.health;
}

bool Unit::IsDead() const
{
	return (m_current.health == 0);
}

void Unit::ApplyInput()
{
	//m_pPhysicsModel->SetInput(m_input.rotX, m_input.move * m_base.moveSpeed, m_input.strafe * m_base.moveSpeed, 0.f, m_input.crouch);
	//m_pPhysicsModel->SetInput(m_input.rotX, m_input.move * m_base.moveSpeed, m_input.strafe * m_base.moveSpeed, 0.f, m_input.crouch);

	Vec3 curRot = m_pPhysicsModel->GetRotation(true);
	curRot.y = m_input.rotX;

	if (curRot != m_previousRotation)
	{
		m_pPhysicsModel->SetRotation(Vec3(0.f, m_input.rotX, 0.f), true);
		m_previousRotation = curRot;
	}
	
	//m_pPhysicsModel->Move(Vec3(m_input.strafe * m_base.moveSpeed * g_deltaTime, 0.f, m_input.move * m_base.moveSpeed * g_deltaTime), false);
	m_pPhysicsModel->AddForce(Vec3(m_input.strafe * m_base.moveSpeed * 10.f, 0.f, m_input.move * m_base.moveSpeed * 10.f), false);
	Vec3 vel = m_pPhysicsModel->GetVelocity(true);
	m_pPhysicsModel->SetVelocity(Vec3(Math::Clamp(vel.x, -m_base.moveSpeed, m_base.moveSpeed), vel.y, Math::Clamp(vel.z, -m_base.moveSpeed, m_base.moveSpeed)), true);

	if (m_cachedYRot != m_input.rotY)
	{
		m_cachedYRot = m_input.rotY;

		for (size_t i = 0; i < m_rotUpPivots.size(); ++i)
		{
			m_rotUpPivots[i]->SetRotation(Vec3(m_defaultRotations[i] + -m_input.rotY, 0.f, 0.f), false);
		}
	}

	if (m_input.shoot)
	{
		m_pWeapon->Shoot();
	}

	if (m_input.reload)
	{
		m_pWeapon->Reload();
	}

	if (m_pCamera)
	{
		if (m_input.aimDownSight)
		{
			m_pCamera->SetFOV(s_cZoomFOV);
		}
		else
		{
			m_pCamera->SetFOV(s_cNormalFOV);
		}
	}
}

void Unit::OnDeath()
{
	std::cout << "Oh no, I died!" << std::endl;
	m_deathTime = 0.f;

	Player::Input input;
	input.aimDownSight = false;
	input.crouch = false;
	input.move = 0.f;
	input.strafe = 0.f;
	input.reload = false;
	input.rotX = m_pHead->GetRotation(true).y;
	input.rotY = m_pHead->GetRotation(true).x;
	SetInput(input);

	m_pWeapon->Reset();
}

void Unit::UpdateDeathAnimation()
{
	m_deathTime += g_deltaTime;

	m_pModel->SetRotation(Vec3(Math::Clamp(m_deathTime * 30.f, 0.f, 90.f), 0.f, 0.f), false);
	m_pModel->SetPosition(m_pPhysicsModel->GetPosition(true) + Vec3(0.f, s_cModelHeight - s_cModelHeight / s_cRespawnTime * m_deathTime, 0.f), true);

	if (m_deathTime >= s_cRespawnTime)
	{
		m_pModel->SetRotation(Vec3(0.f), false);
		m_pModel->SetPosition(m_pPhysicsModel->GetPosition(true) + Vec3(0.f, s_cModelHeight, 0.f), true);
		g_pGame->GetUnitManager()->RespawnUnit(this);
	}
}
