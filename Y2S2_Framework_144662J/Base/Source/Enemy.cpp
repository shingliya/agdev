#include "Enemy.h"


Enemy::Enemy(void)
{
	state = Moving;
	curPosition = Vector3(0, 0, 0);
	curDirection = Vector3(0, 0, 0);
	speed = 0;
	minDistanceFromTarget = 0;
	curShootCooldown = 0;
	shootCooldown = 0;
}

Enemy::~Enemy(void)
{
}

Vector3 Enemy::GetPos()
{
	return curPosition;
}

Vector3 Enemy::GetDir()
{
	return curDirection;
}

void Enemy::Init(Vector3 curPosition, Vector3 target, float speed, float minDistanceFromTarget, float shootCooldown)
{
	this->curPosition = curPosition;
	this->target = target;
	this->curDirection = (target - curPosition).Normalized();
	this->speed = speed;
	this->minDistanceFromTarget = minDistanceFromTarget;
	this->shootCooldown = shootCooldown;
	curShootCooldown = shootCooldown;
}

void Enemy::Update(double dt)
{
	if(dt < 0.2)
	{
		UpdateStates(dt);

		switch(state)
		{
		case Moving:
			{
				Vector3 vel = curDirection.Normalized() * (speed * dt);
				this->ApplyTranslate(vel.x * dt, vel.y * dt , vel.z * dt);
				this->getTransform()->GetOffset(curPosition.x, curPosition.y, curPosition.z);
			}
			break;
		case Loading:
			{
				curShootCooldown -= dt;
				if(curShootCooldown < 0)
				{
					state = Shooting;
				}
			}
			break;
		case Shooting:
			{
				curShootCooldown = shootCooldown;
				state = Loading;
			}
			break;
		}
	}
}

void Enemy::UpdateStates(double dt)
{
	if((target - curPosition).Length() > minDistanceFromTarget)
	{
		state = Moving;
	}
	else if(state != Loading && state != Shooting && (target - curPosition).Length() < minDistanceFromTarget)
	{
		state = Loading;
	}
}