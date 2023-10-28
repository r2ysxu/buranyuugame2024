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
	MeleeEnemyPerRound = FMath::Min(MAX_ENEMY, CurrentRound * MELEE_ENEMY_PER_ROUND);
	RangeEnemyPerRound = FMath::Min(MAX_ENEMY, FMath::Max(CurrentRound - 2, 0) * RANGE_ENEMY_PER_ROUND);
	MeleeEnemySpawned = 0;
	RangeEnemySpawned = 0;
	EnemyCount = 0;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, GetRoundText());
}

void ANecoGoblinGameMode::StartPlay() {
	Super::StartPlay();
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

FString ANecoGoblinGameMode::GetRoundText() {
	return FString::Printf(TEXT("Round %d"), CurrentRound);
}
