// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleplayerGameMode.h"
#include "../Widgets/HUDs/RoundHUD.h"
#include "../Characters/Ally/MainCharacter.h"
#include "../Spawners/GoblinSpawner.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ASingleplayerGameMode::ASingleplayerGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NecoGoblin/Blueprints/Characters/BP_MainSP"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	DelegateGameOver.AddDynamic(this, &ASingleplayerGameMode::RestartPlay);
}

void ASingleplayerGameMode::GameRestart() {
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void ASingleplayerGameMode::SetupEndlessMode() {
	MeleeEnemyPerRound = MAX_ENEMY;
	RangeEnemyPerRound = 35;
}

void ASingleplayerGameMode::NextRound() {
	CurrentRound++;
	if (CurrentRound > FINAL_ROUND) {
		OnVictoryAchieved();
	} else {
		MeleeEnemyPerRound = FMath::Min(MAX_ENEMY, CurrentRound * MELEE_ENEMY_PER_ROUND);
		RangeEnemyPerRound = FMath::Min(MAX_ENEMY, FMath::Max(CurrentRound - 1, 0) * RANGE_ENEMY_PER_ROUND);
		MeleeEnemySpawned = 0;
		RangeEnemySpawned = 0;
		EnemyCount = 0;
		RoundHudWidget->SetCurrentRound(CurrentRound);
		DelegateRoundChange.Broadcast(EnemySpawnRate, (CurrentRound / 2) * 25.f);
		if (NextRoundVoice && CurrentRound > 1) UGameplayStatics::PlaySound2D(GetWorld(), NextRoundVoice);
	}
}

void ASingleplayerGameMode::NextEndlessRound() {
	if (CurrentRound > 11) {
		GetWorldTimerManager().ClearTimer(NextRoundHandler);
		return;
	}
	CurrentRound++;
	DelegateRoundChange.Broadcast(EnemySpawnRate + ((10 - CurrentRound) * 2.f), (CurrentRound / 2) * 25.f);
	GetWorldTimerManager().SetTimer(NextRoundHandler, this, &ASingleplayerGameMode::NextEndlessRound, EndlessNextRoundTime, false);
	EndlessNextRoundTime += CurrentRound * 5.f;
}

void ASingleplayerGameMode::SpawnEnemy() {
	if (EnemyCount >= MAX_ENEMY) return;
	if (FMath::RandBool() && IncrementMeleeEnemy()) {
		if (!Spawners[CurrentSpawnerIndex]->SpawnEnemyType(ESpawnEnemyType::VE_Melee)) DecrementEnemy();
		MeleeEnemySpawned++;
	} else if (IncrementRangeEnemy()) {
		if (!Spawners[CurrentSpawnerIndex]->SpawnEnemyType(ESpawnEnemyType::VE_Range)) DecrementEnemy();
		RangeEnemySpawned++;
	}
	CurrentSpawnerIndex = (CurrentSpawnerIndex + 1) % Spawners.Num();
}

void ASingleplayerGameMode::StartPlay() {
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

	if (!bEndlessMode) {
		NextRound();
	} else {
		NextEndlessRound();
	}
}

void ASingleplayerGameMode::RestartPlay(float TimeDelay) {
	GameOverWidget->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(RestartHandler, this, &ASingleplayerGameMode::GameRestart, TimeDelay, false);
}

bool ASingleplayerGameMode::IncrementMeleeEnemy() {
	if (MeleeEnemySpawned >= MeleeEnemyPerRound) return false;
	MeleeEnemySpawned++;
	EnemyCount++;
	return true;
}

bool ASingleplayerGameMode::IncrementRangeEnemy() {
	if (RangeEnemySpawned >= RangeEnemyPerRound) return false;
	RangeEnemySpawned++;
	EnemyCount++;
	return true;
}

bool ASingleplayerGameMode::DecrementEnemy() {
	if (EnemyCount == 0 || bEndlessMode) return false;
	EnemyCount = FMath::Max(EnemyCount - 1, 0);
	if (EnemyCount == 0) {
		GetWorld()->GetTimerManager().SetTimer(NextRoundHandler, this, &ASingleplayerGameMode::NextRound, 5.f, false);
	}
	return true;
}

void ASingleplayerGameMode::SetEndlessMode(bool IsEndlessMode) {
	bEndlessMode = IsEndlessMode;
	if (IsEndlessMode) SetupEndlessMode();
}

void ASingleplayerGameMode::ShowHuds() {
	if (!bEndlessMode && RoundHudWidget) RoundHudWidget->SetVisibility(ESlateVisibility::Visible);
}

void ASingleplayerGameMode::HideHuds() {
	if (RoundHudWidget)	RoundHudWidget->SetVisibility(ESlateVisibility::Hidden);
}
