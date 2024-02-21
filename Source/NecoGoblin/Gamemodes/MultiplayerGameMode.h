// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMultiplayerGameMode : public AGameModeBase {
	GENERATED_BODY()

protected:
	FTimerHandle NextRoundHandler;
	FTimerHandle RestartHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class URoundHUD> RoundHudWidgetClass;
	URoundHUD* RoundHudWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UUserWidget* GameOverWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voices", meta = (AllowPrivateAccess = "true"))
	USoundBase* NextRoundVoice;
	
public:
	AMultiplayerGameMode();

	void StartPlay() override;

};
