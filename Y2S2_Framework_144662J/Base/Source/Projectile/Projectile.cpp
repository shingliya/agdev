#include "Projectile.h"

/********************************************************************************
 Constructor
 ********************************************************************************/
CProjectile::CProjectile(void)
: m_bActive(false)
, thePosition(0, 0, 0)
, theDirection(0, 0, 0)
, theLength(0.0f)
, theType(PROJ_TYPE_DISCRETE)
, speed(0.0f)
,rotateXZ(0)
, rotateY(0)
,right(0, 0, 0)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
CProjectile::~CProjectile(void)
{
}

/********************************************************************************
 Update the status of this projectile
 ********************************************************************************/
void CProjectile::Update(const double dt)
{
	if (m_bActive == false)
		return;

	// Update Position
	thePosition.Set(	thePosition.x + theDirection.x * speed * dt, 
						thePosition.y + theDirection.y * speed * dt, 
						thePosition.z + theDirection.z * speed * dt);

	if(!theHiddenPosition.IsZero() || !theHiddenDirection.IsZero())
	{
		theHiddenPosition.Set(	theHiddenPosition.x + theHiddenDirection.x * speed * dt, 
						theHiddenPosition.y + theHiddenDirection.y * speed * dt, 
						theHiddenPosition.z + theHiddenDirection.z * speed * dt);
	}
}

/********************************************************************************
 Activate the projectile. true == active, false == inactive
 ********************************************************************************/
void CProjectile::SetActivate(bool m_bActivate)
{
	if (m_bActivate == true)
		m_bActive = true;
	else
		m_bActive = false;
}

/********************************************************************************
 get status of the projectile. true == active, false == inactive
 ********************************************************************************/
bool CProjectile::GetStatus(void)
{
	return m_bActive;
}

/********************************************************************************
 Get the position of the projectile
 ********************************************************************************/
Vector3 CProjectile::GetPosition(void)
{
	return thePosition;
}

/********************************************************************************
 Get the direction of the projectile
 ********************************************************************************/
Vector3 CProjectile::GetDirection(void)
{
	return theDirection;
}

float CProjectile::GetLength()
{
	return theLength;
}

int CProjectile::GetType(void)
{
	return theType;
}

/********************************************************************************
 Set the position of the projectile
 ********************************************************************************/
void CProjectile::SetPosition(Vector3 theNewPosition)
{
	thePosition.Set( theNewPosition.x, theNewPosition.y, theNewPosition.z);
}

/********************************************************************************
 Set the direction of the projectile
 ********************************************************************************/
void CProjectile::SetDirection(Vector3 theNewDirection)
{
	theDirection.Set( theNewDirection.x, theNewDirection.y, theNewDirection.z);
}

/********************************************************************************
 Set speed
 ********************************************************************************/
void CProjectile::SetSpeed(const float speed)
{
	this->speed = speed;
}

void CProjectile::SetLength(const float theLength)
{
	this->theLength = theLength;
	theType = PROJ_TYPE_RAY;
}

void CProjectile::SetType(const int theType)
{
	this->theType = theType;
}

Vector3 CProjectile::GetHiddenPosition()
{
	return theHiddenPosition;
}
Vector3 CProjectile::GetHiddenDirection()
{
	return theHiddenDirection;
}

void CProjectile::SetHiddenPosition(Vector3 theNewDirection)
{
	theHiddenPosition.Set( theNewDirection.x, theNewDirection.y, theNewDirection.z);
}

void CProjectile::SetHiddenDirection(Vector3 theNewDirection)
{
	theHiddenDirection.Set( theNewDirection.x, theNewDirection.y, theNewDirection.z);
}