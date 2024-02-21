// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../Characters/Neco/MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"

void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();
}

void AMainPlayerController::OnPossess(APawn* aPawn) {
	Character = Cast<AMainCharacter>(aPawn);
}