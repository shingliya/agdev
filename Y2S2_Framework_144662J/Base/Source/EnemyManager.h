#pragma once

#include "Enemy.h"
#include "SpatialPartition\SpatialPartition.h"
#include "Projectile\ProjectileManager.h"

class EnemyManager
{
public:
	int numOfEnemiesPerWave;

	EnemyManager();
	~EnemyManager(void);
	
	vector<Enemy*>* getEnemyList();
	int getWave();

	void Init(Vector3 target, CSpatialPartition* SP, CProjectileManager* PM);
	void update(double dt);

private:
	vector<Enemy*> enemyList;

	Vector3 target;

	CSpatialPartition* SP;
	CProjectileManager* PM;
	
	int wave, numOfSpawnsPerWave, curNumOfSpawnsPerWave, numOfEnemiesPerSpawn;

	float timeTillNextSpawn, curTimeTillNextSpawn;
	
	int getNumOfActiveEnemies();
	void spawnEnemy();
	Vector3 RandPosSpawn(float higherOutterRing, float lowerOutterRing, float higherInnerRing, float lowerInnerRing);
};