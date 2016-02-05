#pragma once

#include "Vector3.h"
#include <stdio.h>

class CProjectile
{
public:
	CProjectile(void);
	~CProjectile(void);

	enum PROJECTILE_TYPE
	{
		PROJ_TYPE_DISCRETE,
		PROJ_TYPE_RAY,
		NUM_PROJ_TYPE,
	};
private:
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool m_bActive;
public:
	// Update the status of this projectile
	void Update(const double dt);
	// Activate the projectile. true == active, false == inactive
	void SetActivate(bool m_bActivate);
	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void);
	// Get the position of the projectile
	Vector3 GetPosition();
	// Get the direction of the projectile
	Vector3 GetDirection(void);

	Vector3 GetHiddenPosition();
	Vector3 GetHiddenDirection();

	float rotateXZ, rotateY;
	Vector3 right;

	// Get the length of projectile
	float GetLength();
	// Get the type
	int GetType();
	// Set the position of the projectile
	void SetPosition(Vector3 theNewPosition);
	// Set the direction of the projectile
	void SetDirection(Vector3 theNewDirection);

	void SetHiddenPosition(Vector3);
	void SetHiddenDirection(Vector3);

	// set the length of the ray projectile
	void SetLength(const float theLength);
	// Get the projectile type
	void SetType(const int theType);
	// Set speed
	void SetSpeed(const float speed);
private:
	// The position of the projectile
	Vector3 thePosition;
	// The direction of the projectile
	Vector3 theDirection;
	// The position of the projectile
	Vector3 theHiddenPosition;
	// The direction of the projectile
	Vector3 theHiddenDirection;
	// thelength of the ray projectile
	float theLength;
	//type;
	int theType;
	// Speed
	float speed;
};
