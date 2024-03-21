// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMenuGameMode : public AGameModeBase {
	GENERATED_BODY()

protected:
	virtual void StartPlay() override;
	
public:
	AMenuGameMode();
};
