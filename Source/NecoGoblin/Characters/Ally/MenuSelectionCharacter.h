// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MenuSelectionCharacter.generated.h"

UCLASS()
class NECOGOBLIN_API AMenuSelectionCharacter : public ACharacter {
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AMenuSelectionCharacter();
};
