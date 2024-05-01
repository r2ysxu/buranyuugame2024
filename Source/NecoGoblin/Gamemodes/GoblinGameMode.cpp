// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinGameMode.h"

void AGoblinGameMode::StartPlay() {
	Super::StartPlay();
}

void AGoblinGameMode::StartSpawning() {
	GetWorldTimerManager().SetTimer(SpawnerHandler, this, &AGoblinGameMode::SpawnEnemy, 1.0, true);
}

void AGoblinGameMode::RegisterSpawner(AGoblinSpawner* Spawner) {
	Spawners.Add(Spawner);
	if (Spawners.Num() == TotalSpawners) {
		StartSpawning();
	}
}

bool AGoblinGameMode::DecrementEnemy() {
	if (EnemyCount == 0) return false;
	EnemyCount = FMath::Max(EnemyCount - 1, 0);
	return true;
}
