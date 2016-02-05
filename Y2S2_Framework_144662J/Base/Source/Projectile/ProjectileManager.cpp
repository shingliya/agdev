#include "ProjectileManager.h"

static int theMaxNumberOfProjectiles = 50;

/********************************************************************************
Constructor
********************************************************************************/
CProjectileManager::CProjectileManager(void)
	: theListOfProjectiles(NULL)
	, NumOfActiveProjectile(0)
	, CurrentEmptyProjectile(0)
{
	//theListOfProjectiles = new CProjectile*[theMaxNumberOfProjectiles];
	for (int i=0; i<theMaxNumberOfProjectiles; i++)
	{
		theListOfProjectiles.push_back(new CProjectile());
	}
	Boundary_TopLeft		= Vector3( 1000.0f, 1000.0f, 1000.0f);
	Boundary_BottomRight = Vector3(0, 0, 0);
}

/********************************************************************************
Destructor
********************************************************************************/
CProjectileManager::~CProjectileManager(void)
{
	if (theListOfProjectiles.size() > 0)
	{
		for (int i=0; i<theMaxNumberOfProjectiles; i++)
		{
			delete theListOfProjectiles[i];
		}
	}
}

/********************************************************************************
Add Projectile
********************************************************************************/
CProjectile* CProjectileManager::AddProjectile(Vector3 position, Vector3 direction, const float speed)
{
	// Find the next empty projectile
	// Loop until we find an empty slot
	CurrentEmptyProjectile = 0;
	for(int i = 0; i < theMaxNumberOfProjectiles; ++i)
	{
		if(theListOfProjectiles[CurrentEmptyProjectile]->GetStatus() == false)
		{
			break;
		}
		++CurrentEmptyProjectile;
	}

	if (CurrentEmptyProjectile >= theMaxNumberOfProjectiles)
	{
		CurrentEmptyProjectile += 1;
		theMaxNumberOfProjectiles += 10;
		for (int i=0; i < 10; i++)
		{
			theListOfProjectiles.push_back(new CProjectile());
		}
	}

	if (IsActive(CurrentEmptyProjectile) == false)
	{
		position += Vector3(0.0f, 0.5f, 0.0f);
		theListOfProjectiles[CurrentEmptyProjectile]->SetPosition(position);
		theListOfProjectiles[CurrentEmptyProjectile]->SetDirection(direction);
		theListOfProjectiles[CurrentEmptyProjectile]->SetSpeed(speed);
		theListOfProjectiles[CurrentEmptyProjectile]->SetActivate(true);
		NumOfActiveProjectile++;

		return theListOfProjectiles[CurrentEmptyProjectile];
	}
}

CProjectile* CProjectileManager::Add3rdPersonProjectile(Vector3 position, Vector3 direction, Vector3 hiddenPosition, Vector3 hiddenDirection, const float speed)
{
	// Find the next empty projectile
	// Loop until we find an empty slot
	CurrentEmptyProjectile = 0;
	for(int i = 0; i < theMaxNumberOfProjectiles; ++i)
	{
		if(theListOfProjectiles[CurrentEmptyProjectile]->GetStatus() == false)
		{
			break;
		}
		++CurrentEmptyProjectile;
	}

	if (CurrentEmptyProjectile >= theMaxNumberOfProjectiles)
	{
		CurrentEmptyProjectile += 1;
		theMaxNumberOfProjectiles += 10;
		for (int i=0; i < 10; i++)
		{
			theListOfProjectiles.push_back(new CProjectile());
		}
	}

	if (IsActive(CurrentEmptyProjectile) == false)
	{
		position += Vector3(0.0f, 0.5f, 0.0f);
		theListOfProjectiles[CurrentEmptyProjectile]->SetPosition(position);
		theListOfProjectiles[CurrentEmptyProjectile]->SetDirection(direction);
		theListOfProjectiles[CurrentEmptyProjectile]->SetHiddenPosition(hiddenPosition);
		theListOfProjectiles[CurrentEmptyProjectile]->SetHiddenDirection(hiddenDirection);
		theListOfProjectiles[CurrentEmptyProjectile]->SetSpeed(speed);
		theListOfProjectiles[CurrentEmptyProjectile]->SetActivate(true);
		NumOfActiveProjectile++;

		return theListOfProjectiles[CurrentEmptyProjectile];
	}
}

CProjectile* CProjectileManager::AddRayProjectile(Vector3 position, Vector3 direction, const float speed, const float length)
{
	// Find the next empty projectile
	// Loop until we find an empty slot
	CurrentEmptyProjectile = 0;
	for(int i = 0; i < theMaxNumberOfProjectiles; ++i)
	{
		if(theListOfProjectiles[CurrentEmptyProjectile]->GetStatus() == false)
		{
			break;
		}
		++CurrentEmptyProjectile;
	}

	if (CurrentEmptyProjectile >= theMaxNumberOfProjectiles)
	{
		CurrentEmptyProjectile += 1;
		theMaxNumberOfProjectiles += 10;
		for (int i=0; i < 10; i++)
		{
			theListOfProjectiles.push_back(new CProjectile());
		}
	}

	if (IsActive(CurrentEmptyProjectile) == false)
	{
		position += Vector3(0.0f, 0.5f, 0.0f);
		theListOfProjectiles[CurrentEmptyProjectile]->SetPosition(position);
		theListOfProjectiles[CurrentEmptyProjectile]->SetDirection(direction);
		theListOfProjectiles[CurrentEmptyProjectile]->SetSpeed(speed);
		theListOfProjectiles[CurrentEmptyProjectile]->SetLength(length);
		theListOfProjectiles[CurrentEmptyProjectile]->SetActivate(true);
		NumOfActiveProjectile++;

		return theListOfProjectiles[CurrentEmptyProjectile];
	}
}

/********************************************************************************
Set Projectile
********************************************************************************/
void CProjectileManager::SetProjectile(const int index, Vector3 position, Vector3 direction)
{
	theListOfProjectiles[index]->SetPosition( position );
	theListOfProjectiles[index]->SetDirection( direction );
	theListOfProjectiles[index]->SetActivate( true );
}

/********************************************************************************
Remove Projectile
********************************************************************************/
void CProjectileManager::RemoveProjectile(const int index)
{
	if (IsValid( index ))
	{
		theListOfProjectiles[index]->SetActivate( false );
		NumOfActiveProjectile--;
	}
}

/********************************************************************************
Get Maximum Number of Projectiles
********************************************************************************/
int CProjectileManager::GetMaxNumberOfProjectiles(void)
{
	return theMaxNumberOfProjectiles;
}

/********************************************************************************
Get Number of Active Projectile
********************************************************************************/
int CProjectileManager::GetNumberOfActiveProjectiles(void)
{
	return NumOfActiveProjectile;
}

/********************************************************************************
Get Projectile
********************************************************************************/
CProjectile* CProjectileManager::GetProjectile(const int index)
{
	if (IsValid( index ))
		return theListOfProjectiles[index];
	return NULL;
}

/********************************************************************************
Check methods
********************************************************************************/
bool CProjectileManager::IsValid(const int index)
{
	if ((index<0) || (index>theMaxNumberOfProjectiles))
		return false;
	return true;
}

/********************************************************************************
Check if a projectile is active
********************************************************************************/
bool CProjectileManager::IsActive(const int index)
{
	if (IsValid(index))
		return theListOfProjectiles[index]->GetStatus();
	else
		return true;	//	return true as we do not wish to use this index
}

/********************************************************************************
Update
********************************************************************************/
void CProjectileManager::Update(const double dt)
{
	for (int i=0; i<theMaxNumberOfProjectiles; i++)
	{
		// Skip this projectile if it is not active
		if (theListOfProjectiles[i]->GetStatus() == false)
			continue;

		// Update the projectile
		if(theListOfProjectiles[i]->GetType() == theListOfProjectiles[i]->PROJ_TYPE_DISCRETE && (!theListOfProjectiles[i]->GetHiddenPosition().IsZero() || theListOfProjectiles[i]->GetHiddenDirection().IsZero()))
		{
			if(theListOfProjectiles[i]->GetHiddenPosition() != theListOfProjectiles[i]->GetPosition() && theListOfProjectiles[i]->GetHiddenDirection() != theListOfProjectiles[i]->GetDirection())
				theListOfProjectiles[i]->SetDirection((theListOfProjectiles[i]->GetHiddenPosition() - theListOfProjectiles[i]->GetPosition()).Normalized());
		}
		theListOfProjectiles[i]->Update(dt);

		// Check if it is out of the boundaries
		Vector3 CheckThisPosition = theListOfProjectiles[i]->GetPosition();
		if (((CheckThisPosition.x > Boundary_TopLeft.x) || 
			(CheckThisPosition.y > Boundary_TopLeft.y) ||
			(CheckThisPosition.z > Boundary_TopLeft.z)) ||
			((CheckThisPosition.x < Boundary_BottomRight.x) ||
			(CheckThisPosition.y < Boundary_BottomRight.y) ||
			(CheckThisPosition.z < Boundary_BottomRight.z)) )
		{
			// Since it is out of the boundaries, then set it to inactive
			RemoveProjectile( i );
		}
	}
}