// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GoblinGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class NECOGOBLIN_API AGoblinGameMode : public AGameModeBase {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_Gameover, float, TimeDelay);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_GameRoundChange, float, EnemySpawnRate, float, EnemyMovementSpeedMod);
	
protected:
	const int32 MAX_ENEMY = 100;

	FTimerHandle NextRoundHandler;
	FTimerHandle RestartHandler;
	int RangeEnemySpawned = 0;
	int MeleeEnemySpawned = 0;
	int EnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AGoblinSpawner*> Spawners;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class URoundHUD> RoundHudWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voices", meta = (AllowPrivateAccess = "true"))
	USoundBase* NextRoundVoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TotalSpawners = 1;

	FTimerHandle SpawnerHandler;
	int32 CurrentSpawnerIndex = 0;
	int32 TotalEnemySpawned = 0;

	class URoundHUD* RoundHudWidget;
	class UUserWidget* GameOverWidget;

	virtual void StartSpawning();
	virtual void SpawnEnemy() {}

public:
	FDelegate_Gameover DelegateGameOver;
	FDelegate_GameRoundChange DelegateRoundChange;

	virtual void StartPlay() override;
	void RegisterSpawner(class AGoblinSpawner* Spawner);
	virtual bool DecrementEnemy();
};
