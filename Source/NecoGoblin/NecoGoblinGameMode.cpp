// Copyright Epic Games, Inc. All Rights Reserved.

#include "NecoGoblinGameMode.h"
#include "Characters/Neco/MainCharacter.h"

#include "UObject/ConstructorHelpers.h"

ANecoGoblinGameMode::ANecoGoblinGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NecoGoblin/Blueprints/Characters/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ANecoGoblinGameMode::NextRound() {
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
	}
}

void ANecoGoblinGameMode::StartPlay() {
	Super::StartPlay();
	RoundHudWidget = CreateWidget<URoundHUD>(GetWorld(), RoundHudWidgetClass);
	if (RoundHudWidget) {
		RoundHudWidget->AddToViewport();
		RoundHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	NextRound();
}

bool ANecoGoblinGameMode::IncrementMeleeEnemy() {
	if (MeleeEnemySpawned >= MeleeEnemyPerRound) return false;
	MeleeEnemySpawned++;
	EnemyCount++;
	return true;
}

bool ANecoGoblinGameMode::IncrementRangeEnemy() {
	if (RangeEnemySpawned >= RangeEnemyPerRound) return false;
	RangeEnemySpawned++;
	EnemyCount++;
	return true;
}

bool ANecoGoblinGameMode::DecrementEnemy() {
	if (EnemyCount == 0) return false;
	EnemyCount = FMath::Max(EnemyCount - 1, 0);
	if (EnemyCount == 0) {
		GetWorld()->GetTimerManager().SetTimer(NextRoundHandler, this, &ANecoGoblinGameMode::NextRound, 5.f, false);
	}
	return true;
}

void ANecoGoblinGameMode::ShowHuds() {
	if (RoundHudWidget)	RoundHudWidget->SetVisibility(ESlateVisibility::Visible);
}

void ANecoGoblinGameMode::HideHuds() {
	if (RoundHudWidget)	RoundHudWidget->SetVisibility(ESlateVisibility::Hidden);
}
