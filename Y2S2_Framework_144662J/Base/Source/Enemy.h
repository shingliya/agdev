#pragma once

#include "SceneGraph\SceneNode.h"

class Enemy : public CSceneNode
{
public:
	enum States
	{
		Moving,
		Loading,
		Shooting,
		NUM_GEOMETRY,
	};

	int state;

	Enemy(void);
	~Enemy(void);

	void Init(Vector3 curPosition, Vector3 target, float speed, float minDistanceFromTarget, float shootCooldown);

	void Update(double dt);
	void UpdateStates(double dt);

	Vector3 GetPos();
	Vector3 GetDir();

private:
	// Hero's information
	Vector3 curPosition;
	Vector3 curDirection;
	Vector3 target;

	float speed;
	float minDistanceFromTarget;

	float curShootCooldown, shootCooldown;
};

