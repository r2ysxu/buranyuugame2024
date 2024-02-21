// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMainPlayerController : public APlayerController {
	GENERATED_BODY()

protected:
	class AMainCharacter* Character;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

public:


};
