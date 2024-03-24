// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GoblinGameMode.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMultiplayerGameMode : public AGoblinGameMode {
	GENERATED_BODY()

protected:
	FTimerHandle SpawnerHandler;
	int32 CurrentSpawnerIndex = 0;
	int32 TotalEnemySpawned = 0;

	int LoggedInPlayers = 0;

	virtual void StartSpawning() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	void SpawnEnemy();
	
public:
	AMultiplayerGameMode();

	void StartPlay() override;

};
