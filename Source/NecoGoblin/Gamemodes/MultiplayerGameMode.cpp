// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "States/MultiplayerGameState.h"
#include "../Characters/Neco/MainCharacter.h"
#include "../Controllers/MainPlayerController.h"
#include "../Spawners/GoblinSpawner.h"
#include "../Widgets/HUDs/RoundHUD.h"
#include "../Widgets/Menus/MultiplayerLobbyMenuWidget.h"

#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"

AMultiplayerGameMode::AMultiplayerGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NecoGoblin/Blueprints/Characters/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	GameStateClass = AMultiplayerGameState::StaticClass();
	/*static ConstructorHelpers::FClassFinder<APlayerController> PlayerContollerBPClass(TEXT("/Game/NecoGoblin/Blueprints/Controllers/BP_MainPlayerController"));
	if (PlayerContollerBPClass.Class != NULL) {
		PlayerControllerClass = PlayerContollerBPClass.Class;
	}*/

	UGameplayStatics::LoadStreamLevel(GetWorld(), LOBBY_MENU_MAP, true, true, FLatentActionInfo());
}

void AMultiplayerGameMode::StartPlay() {
	Super::StartPlay();
	if (HasAuthority()) {
		MultiplayerLobbyMenu = CreateWidget<UMultiplayerLobbyMenuWidget>(GetWorld(), MultiplayerLobbyMenuClass);
		if (MultiplayerLobbyMenu) {
			MultiplayerLobbyMenu->AddToViewport();
		}
	}

	/*RoundHudWidget = CreateWidget<URoundHUD>(GetWorld(), RoundHudWidgetClass);
	if (RoundHudWidget) {
		RoundHudWidget->AddToViewport();
		RoundHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	if (GameOverWidget) {
		GameOverWidget->AddToViewport();
		GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
	}*/
	ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(GetWorld(), "PersistentLevel_MP");
}

void AMultiplayerGameMode::StartSpawning() {
	Super::StartSpawning();
	GetWorldTimerManager().SetTimer(SpawnerHandler, this, &AMultiplayerGameMode::SpawnEnemy, 1.0, true);
}

void AMultiplayerGameMode::OnPostLogin(AController* NewPlayer) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("OnPostLogin"));
	APlayerController* controller = Cast<APlayerController>(NewPlayer);
	if (IsValid(controller)) {
		controller->SetInputMode(FInputModeUIOnly());
		controller->bShowMouseCursor = true;
	}
}

void AMultiplayerGameMode::SpawnEnemy() {
	if (TotalEnemySpawned >= MAX_ENEMY) return;
	if (FMath::RandBool()) {
		Spawners[CurrentSpawnerIndex]->SpawnEnemyType(ESpawnEnemyType::VE_Melee);
		MeleeEnemySpawned++;
	} else {
		Spawners[CurrentSpawnerIndex]->SpawnEnemyType(ESpawnEnemyType::VE_Range);
		RangeEnemySpawned++;
	}
	CurrentSpawnerIndex = (CurrentSpawnerIndex + 1) % TotalSpawners;
}

void AMultiplayerGameMode::SetupPlayers() {
	for (FConstPlayerControllerIterator controllerIt = GetWorld()->GetPlayerControllerIterator(); controllerIt; controllerIt++) {
		if (APlayerController* controller = controllerIt->Get()) {
			controller->SetInputMode(FInputModeGameOnly());
			controller->bShowMouseCursor = false;
			AMainCharacter* character = Cast<AMainCharacter>(controller->GetPawn());
			if (IsValid(character)) {
				character->OnCharacterStart();
			}
		}
	}
}

void AMultiplayerGameMode::LoadIntoMPLevel(FName LevelName) {
	FLatentActionInfo latentInfo;
	latentInfo.UUID = 123;
	latentInfo.Linkage = 0;
	latentInfo.CallbackTarget = this;
	latentInfo.ExecutionFunction = "SetupPlayers";
	UGameplayStatics::LoadStreamLevel(GetWorld(), LevelName, true, true, latentInfo);
	UGameplayStatics::UnloadStreamLevel(GetWorld(), LOBBY_MENU_MAP, FLatentActionInfo(), false);
}