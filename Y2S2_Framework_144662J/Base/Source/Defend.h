#pragma once

#include "SceneGraph\SceneNode.h"

class Defend : public CSceneNode
{
public:
	Defend(void);
	~Defend(void);

	int getHealth();

	void Init(int maxHealth, float healCD);
	void Update(double dt);
	void GetHit();

private:
	int health, maxHealth;

	float healCD, curHealCD;
};