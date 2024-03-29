// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../Characters/Neco/MainCharacter.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"

void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();
	LoadingScreenMenu = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenMenuClass);
}

void AMainPlayerController::OnPossess(APawn* aPawn) {
	Super::OnPossess(aPawn);
	Character = Cast<AMainCharacter>(aPawn);
}

void AMainPlayerController::Client_OnInitiateLevelLoad_Implementation() {
	if (LoadingScreenMenu) {
		LoadingScreenMenu->AddToViewport();
	}
}

void AMainPlayerController::Client_OnCharacterStart_Implementation() {
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	AMainCharacter* character = Cast<AMainCharacter>(GetPawn());
	if (IsValid(character)) {
		character->OnCharacterStart();
	}
	if (LoadingScreenMenu) {
		LoadingScreenMenu->RemoveFromParent();
	}
}

void AMainPlayerController::Client_OnEnterLobbyMode_Implementation() {
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
