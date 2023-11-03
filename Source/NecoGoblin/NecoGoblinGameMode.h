// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUDs/RoundHUD.h"
#include "NecoGoblinGameMode.generated.h"

UCLASS(minimalapi)
class ANecoGoblinGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	const int MELEE_ENEMY_PER_ROUND = 20;
	const int RANGE_ENEMY_PER_ROUND = 5;
	const int MAX_ENEMY = 100;

protected:
	FTimerHandle NextRoundHandler;

	int CurrentRound = 0;
	int RangeEnemySpawned = 0;
	int MeleeEnemySpawned = 0;
	int MeleeEnemyPerRound;
	int RangeEnemyPerRound;
	int EnemyCount;
	float EnemySpawnRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class URoundHUD> RoundHudWidgetClass;
	URoundHUD* RoundHudWidget;

	void NextRound();

public:
	ANecoGoblinGameMode();

	void StartPlay() override;
	
	bool IncrementMeleeEnemy();
	bool IncrementRangeEnemy();
	bool DecrementEnemy();
	UFUNCTION(BlueprintCallable) void ShowHuds();
	FORCEINLINE int GetCurrentRound() { return CurrentRound; }
	FORCEINLINE int GetGoblinCount() { return EnemyCount; }
	FORCEINLINE int GetMeleeGoblinPerRound() { return MeleeEnemyPerRound; }
	FORCEINLINE int GetRangeEnemyPerRound() { return RangeEnemyPerRound; }
	FORCEINLINE float GetGoblinSpawnRate() { return EnemySpawnRate; }
};
