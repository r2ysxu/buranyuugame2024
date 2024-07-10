// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../Characters/Ally/MainCharacter.h"
#include "../Widgets/HUDs/RoundHUD.h"
#include "../Widgets/Menus/MultiplayerLobbyMenuWidget.h"
#include "../GameInstance/NGGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"

void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();
	LoadingScreenMenu = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenMenuClass);

	MultiplayerLobbyMenu = CreateWidget<UMultiplayerLobbyMenuWidget>(GetWorld(), MultiplayerLobbyMenuClass);
	if (MultiplayerLobbyMenu) {
		MultiplayerLobbyMenu->CheckCanStart(HasAuthority());
		MultiplayerLobbyMenu->AddToViewport();
	}
}

void AMainPlayerController::OnPossess(APawn* aPawn) {
	Super::OnPossess(aPawn);
	Character = Cast<AMainCharacter>(aPawn);

}

void AMainPlayerController::QuitSession() {
	Cast<UNGGameInstance>(GetGameInstance())->QuitSession();
}

void AMainPlayerController::Client_OnPropagateLobbySettings_Implementation() {
	AMainCharacter* character = Cast<AMainCharacter>(GetPawn());
	if (IsValid(character)) {
		character->ChangeCharacterSkin(0);
	}
}

void AMainPlayerController::Client_OnInitiateLevelLoad_Implementation() {
	if (LoadingScreenMenu) {
		LoadingScreenMenu->AddToViewport();
	}
	if (MultiplayerLobbyMenu && MultiplayerLobbyMenu->IsInViewport()) {
		MultiplayerLobbyMenu->SetVisibility(ESlateVisibility::Hidden);
		MultiplayerLobbyMenu->RemoveFromParent();
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

	RoundHudWidget = CreateWidget<URoundHUD>(GetWorld(), RoundHudWidgetClass);
	if (RoundHudWidget) {
		RoundHudWidget->AddToViewport();
	}
}

void AMainPlayerController::Client_OnEnterLobbyMode_Implementation() {
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AMainPlayerController::Client_OnGameover_Implementation() {
	GameoverScreenMenu = CreateWidget<UUserWidget>(GetWorld(), GameoverScreenMenuClass);
	if (GameoverScreenMenu) {
		GameoverScreenMenu->AddToViewport();
	}
	// Exit session and return to menu
}

void AMainPlayerController::Client_RoundUpdate_Implementation(int RoundNumber) {
	if (RoundHudWidget) {
		RoundHudWidget->SetCurrentRound(RoundNumber);
	}
}