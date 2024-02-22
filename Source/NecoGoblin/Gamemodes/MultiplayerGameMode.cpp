// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "States/MultiplayerGameState.h"
#include "../Characters/Neco/MainCharacter.h"
#include "../Widgets/HUDs/RoundHUD.h"
#include "../Controllers/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

AMultiplayerGameMode::AMultiplayerGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NecoGoblin/Blueprints/Characters/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	GameStateClass = AMultiplayerGameState::StaticClass();
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerContollerBPClass(TEXT("/Game/NecoGoblin/Blueprints/Controllers/BP_MainPlayerController"));
	//if (PlayerContollerBPClass.Class != NULL) {
	//	PlayerControllerClass = PlayerContollerBPClass.Class;
	//}
}

void AMultiplayerGameMode::StartPlay() {
	Super::StartPlay();
	RoundHudWidget = CreateWidget<URoundHUD>(GetWorld(), RoundHudWidgetClass);
	if (RoundHudWidget) {
		RoundHudWidget->AddToViewport();
		RoundHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	if (GameOverWidget) {
		GameOverWidget->AddToViewport();
		GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
