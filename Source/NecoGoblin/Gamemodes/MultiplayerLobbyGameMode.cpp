// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerLobbyGameMode.h"
#include "../Widgets/Menus/MultiplayerLobbyMenuWidget.h"

#include "Kismet/GameplayStatics.h"

void AMultiplayerLobbyGameMode::StartPlay() {
	Super::StartPlay();
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->bShowMouseCursor = true;

	if (HasAuthority()) {
		MultiplayerLobbyMenu = CreateWidget<UMultiplayerLobbyMenuWidget>(GetWorld(), MultiplayerLobbyMenuClass);
		if (MultiplayerLobbyMenu) {
			MultiplayerLobbyMenu->AddToViewport();
		}
	}
}

AMultiplayerLobbyGameMode::AMultiplayerLobbyGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NecoGoblin/Blueprints/Characters/BP_MenuSelectionCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
