// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinSpawner.h"
#include "NecoGoblin/NecoGoblinGameMode.h"
#include "../Characters/Goblin/MeleeGoblinCharacter.h"
#include "../Characters/Goblin/RangeGoblinCharacter.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AGoblinSpawner::AGoblinSpawner() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void AGoblinSpawner::BeginPlay() {
	Super::BeginPlay();
	GameMode = Cast<ANecoGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode) {
		GetWorld()->GetTimerManager().SetTimer(OnSpawnHandler, this, &AGoblinSpawner::SpawnEnemy, GameMode->GetGoblinSpawnRate(), true);
	}
}

void AGoblinSpawner::SpawnEnemy() {
	if (GameMode) {
		if (FMath::RandBool() && GameMode->IncrementRangeEnemy()) {
			if (!SpawnEnemyType(2)) GameMode->DecrementEnemy();
		} else if (GameMode->IncrementMeleeEnemy()) {
			if (!SpawnEnemyType(1)) GameMode->DecrementEnemy();
		} else if (GameMode->IncrementRangeEnemy()) {
			if (SpawnEnemyType(2)) GameMode->DecrementEnemy();
		}
	}
}

bool AGoblinSpawner::SpawnEnemyType(uint8 Type) {
	FTransform goblinTransform;
	goblinTransform.SetLocation(GetActorLocation());
	goblinTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	if (Type == 1) {
		AMeleeGoblinCharacter* goblin = GetWorld()->SpawnActor<AMeleeGoblinCharacter>(MeleeGoblinClass, goblinTransform);
		if (IsValid(goblin)) {
			goblin->SetRunSpeed((GameMode->GetCurrentRound() / 2) * 25.f);
			return true;
		} else return false;
	} else if (Type == 2) {
		ARangeGoblinCharacter* goblin = GetWorld()->SpawnActor<ARangeGoblinCharacter>(RangeGoblinClass, goblinTransform);
		return IsValid(goblin);
	}
	return false;
}
