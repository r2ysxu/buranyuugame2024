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
	const FName LOBBY_MENU_MAP = FName("MPLobbyWorld");

	bool GameStarted = false;

	virtual void StartPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* NewPlayer) override;
	virtual void SpawnEnemy() override;
	void OnInitiateLobbyLoadForPlayers();
	void OnInitiateLevelLoadForPlayers();

	
	UFUNCTION() void SetupPlayers();
	
public:
	AMultiplayerGameMode();

	void LoadIntoMPLevel(FName LevelName);
	UFUNCTION() void OnPlayerDead(float Delay);
};
