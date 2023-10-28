// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "NecoGoblin/NecoGoblinGameMode.h"
#include "../Characters/Goblin/MeleeGoblinCharacter.h"

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
			SpawnEnemyType(2);
		} else if (GameMode->IncrementMeleeEnemy()) {
			SpawnEnemyType(1);
		} else if (GameMode->IncrementRangeEnemy()) {
			SpawnEnemyType(2);
		}
	}
}

void AGoblinSpawner::SpawnEnemyType(uint8 Type) {
	FTransform goblinTransform;
	goblinTransform.SetLocation(GetActorLocation());
	goblinTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	if (Type == 1) {
		AMeleeGoblinCharacter* goblin = GetWorld()->SpawnActor<AMeleeGoblinCharacter>(MeleeGoblinClass, goblinTransform);
		goblin->SetRunSpeed((GameMode->GetCurrentRound() / 3) * 20.f);
	} else if (Type == 2) {
		ARangeGoblinCharacter* goblin = GetWorld()->SpawnActor<ARangeGoblinCharacter>(RangeGoblinClass, goblinTransform);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Spawn Range"));
	}
}
