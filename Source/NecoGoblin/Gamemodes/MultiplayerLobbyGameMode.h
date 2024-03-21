// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMultiplayerLobbyGameMode : public AGameModeBase {
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UMultiplayerLobbyMenuWidget> MultiplayerLobbyMenuClass;

	class UMultiplayerLobbyMenuWidget* MultiplayerLobbyMenu;

	virtual void StartPlay() override;

public:
	AMultiplayerLobbyGameMode();
};
