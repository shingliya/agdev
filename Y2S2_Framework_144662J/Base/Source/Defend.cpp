#include "Defend.h"


Defend::Defend(void)
{
	health = healCD = curHealCD = 0;
}

Defend::~Defend(void)
{
}

int Defend::getHealth()
{
	return health;
}

void Defend::Init(int maxHealth, float healCD)
{
	this->maxHealth = maxHealth;
	health = maxHealth;
	this->healCD = healCD;
	curHealCD = healCD;
}

void Defend::Update(double dt)
{
	if(active)
	{
		if(health < maxHealth)
		{
			curHealCD-=dt;
			if(curHealCD<0)
			{
				health++;
				curHealCD = healCD;
			}
		}

		this->ApplyRotate(health * dt, 1, 1, 1);
	}
}

void Defend::GetHit()
{
	health--;
	curHealCD = healCD;
	if(health == 0)
	{
		active = false;
	}
}