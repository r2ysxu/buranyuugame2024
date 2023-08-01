// Copyright Epic Games, Inc. All Rights Reserved.

#include "NecoGoblinGameMode.h"
#include "Characters/Neco/MainCharacter.h"
#include "UObject/ConstructorHelpers.h"

const int GOBLIN_PER_ROUND = 10;
const int MAX_GOBLIN = 200;

ANecoGoblinGameMode::ANecoGoblinGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NecoGoblin/Blueprints/Characters/BP_MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ANecoGoblinGameMode::NextRound() {
	CurrentRound++;
	GoblinPerRound = std::min(MAX_GOBLIN, CurrentRound * GOBLIN_PER_ROUND);
	GoblinKilled = 0;
	GoblinCount = 0;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Round: %d, Goblins: %d"), CurrentRound, GoblinPerRound));
}

void ANecoGoblinGameMode::StartPlay() {
	Super::StartPlay();
	NextRound();
}

bool ANecoGoblinGameMode::IncrementGoblin() {
	if (GoblinCount >= GoblinPerRound) return false;
	GoblinCount = std::min(GoblinCount + 1, GoblinPerRound);
	return true;
}

bool ANecoGoblinGameMode::DecrementGoblin() {
	if (GoblinCount == 0) return false;
	GoblinCount = std::max(GoblinCount - 1, 0);
	GoblinKilled++;
	if (GoblinKilled == GoblinPerRound) {
		GetWorld()->GetTimerManager().SetTimer(NextRoundHandler, this, &ANecoGoblinGameMode::NextRound, 5.f, false);
	}
	return true;
}
