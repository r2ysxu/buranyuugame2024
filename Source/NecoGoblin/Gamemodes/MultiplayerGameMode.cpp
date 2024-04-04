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

	GameStateClass = AMultiplayerGameState::StaticClass();
	static ConstructorHelpers::FClassFinder<AMainPlayerController> PlayerContollerBPClass(TEXT("/Game/NecoGoblin/Blueprints/Controllers/BP_MainPlayerController"));
	if (PlayerContollerBPClass.Class != NULL) {
		PlayerControllerClass = PlayerContollerBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NecoGoblin/Blueprints/Characters/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	UGameplayStatics::LoadStreamLevel(GetWorld(), LOBBY_MENU_MAP, true, true, FLatentActionInfo());
	DelegateGameOver.AddDynamic(this, &AMultiplayerGameMode::OnPlayerDead);
}

void AMultiplayerGameMode::StartPlay() {
	Super::StartPlay();
	MultiplayerLobbyMenu = CreateWidget<UMultiplayerLobbyMenuWidget>(GetWorld(), MultiplayerLobbyMenuClass);
	if (MultiplayerLobbyMenu) {
		MultiplayerLobbyMenu->AddToViewport();
	}
}

void AMultiplayerGameMode::StartSpawning() {
	Super::StartSpawning();
	GetWorldTimerManager().SetTimer(SpawnerHandler, this, &AMultiplayerGameMode::SpawnEnemy, 1.0, true);
}

void AMultiplayerGameMode::OnPostLogin(AController* NewPlayer) {
	Super::OnPostLogin(NewPlayer);
	AMainPlayerController* controller = Cast<AMainPlayerController>(NewPlayer);
	if (IsValid(controller)) {
		controller->Client_OnEnterLobbyMode();
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

void AMultiplayerGameMode::OnInitiateLevelLoadForPlayers() {
	for (FConstPlayerControllerIterator controllerIt = GetWorld()->GetPlayerControllerIterator(); controllerIt; controllerIt++) {
		if (AMainPlayerController* controller = Cast<AMainPlayerController>(controllerIt->Get())) {
			controller->Client_OnInitiateLevelLoad();
		}
	}
}

void AMultiplayerGameMode::OnPlayerDead(float Delay) {
	bool allPlayerDead = true;
	for (FConstPlayerControllerIterator controllerIt = GetWorld()->GetPlayerControllerIterator(); controllerIt; controllerIt++) {
		if (AMainPlayerController* controller = Cast<AMainPlayerController>(controllerIt->Get())) {
			if (Cast<AMainCharacter>(controller->GetPawn())->GetIsAlive()) {
				allPlayerDead = false;
				break;
			}
		}
	}
	if (allPlayerDead) {
		for (FConstPlayerControllerIterator controllerIt = GetWorld()->GetPlayerControllerIterator(); controllerIt; controllerIt++) {
			if (AMainPlayerController* controller = Cast<AMainPlayerController>(controllerIt->Get())) {
				controller->Client_OnGameover();
			}
		}
	}
}

void AMultiplayerGameMode::SetupPlayers() {
	for (FConstPlayerControllerIterator controllerIt = GetWorld()->GetPlayerControllerIterator(); controllerIt; controllerIt++) {
		if (AMainPlayerController* controller = Cast<AMainPlayerController>(controllerIt->Get())) {
			controller->Client_OnCharacterStart();
		}
	}
}

void AMultiplayerGameMode::LoadIntoMPLevel(FName LevelName) {
	OnInitiateLevelLoadForPlayers();
	FLatentActionInfo latentInfo;
	latentInfo.UUID = 123;
	latentInfo.Linkage = 0;
	latentInfo.CallbackTarget = this;
	latentInfo.ExecutionFunction = "SetupPlayers";
	UGameplayStatics::LoadStreamLevel(GetWorld(), LevelName, true, true, latentInfo);
	UGameplayStatics::UnloadStreamLevel(GetWorld(), LOBBY_MENU_MAP, FLatentActionInfo(), false);
}