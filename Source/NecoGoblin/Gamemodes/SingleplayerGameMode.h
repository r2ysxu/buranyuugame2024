// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoblinGameMode.h"
#include "SingleplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API ASingleplayerGameMode : public AGoblinGameMode {
	GENERATED_BODY()

private:
	const int MELEE_ENEMY_PER_ROUND = 10;
	const int RANGE_ENEMY_PER_ROUND = 3;
	const int FINAL_ROUND = 10;

	bool bEndlessMode = false;

	void GameRestart();
	void SetupEndlessMode();

protected:

	int CurrentRound = 0;
	int MeleeEnemyPerRound;
	int RangeEnemyPerRound;
	float EnemySpawnRate = 5.f;

	float EndlessNextRoundTime = 30.f;

	void NextRound();
	void NextEndlessRound();
	virtual void SpawnEnemy() override;

public:
	ASingleplayerGameMode();

	void StartPlay() override;

	bool IncrementMeleeEnemy();
	bool IncrementRangeEnemy();
	bool DecrementEnemy();
	void SetEndlessMode(bool IsEndlessMode);

	UFUNCTION(BlueprintCallable) void ShowHuds();
	UFUNCTION(BlueprintCallable) void HideHuds();
	UFUNCTION(BlueprintCallable) void RestartPlay(float TimeDelay);
	UFUNCTION(BlueprintImplementableEvent) void OnVictoryAchieved();
	FORCEINLINE int GetCurrentRound() { return CurrentRound; }
	FORCEINLINE int GetGoblinCount() { return EnemyCount; }
	FORCEINLINE int GetMeleeGoblinPerRound() { return MeleeEnemyPerRound; }
	FORCEINLINE int GetRangeEnemyPerRound() { return RangeEnemyPerRound; }
	FORCEINLINE float GetGoblinSpawnRate() { return EnemySpawnRate; }
	
};
