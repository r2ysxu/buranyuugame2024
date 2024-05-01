// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinSpawner.h"
#include "../Gamemodes/GoblinGameMode.h"
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
	AGoblinGameMode* gamemode = Cast<AGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gamemode) {
		gamemode->RegisterSpawner(this);
	}
}

void AGoblinSpawner::ChangeSpawnInfo(float EnemySpawnRate, float EnemyMovementSpeedMod) {
	MovementSpeedModifier = EnemyMovementSpeedMod;
	SpawnRate = EnemySpawnRate;
}

bool AGoblinSpawner::SpawnEnemyType(ESpawnEnemyType Type) {
	FTransform goblinTransform;
	goblinTransform.SetLocation(GetActorLocation());
	goblinTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	if (Type == ESpawnEnemyType::VE_Melee) {
		AMeleeGoblinCharacter* goblin = GetWorld()->SpawnActor<AMeleeGoblinCharacter>(MeleeGoblinClass, goblinTransform);
		if (IsValid(goblin)) {
			goblin->SetRunSpeed(MovementSpeedModifier);
			return true;
		} else return false;
	} else if (Type == ESpawnEnemyType::VE_Range) {
		ARangeGoblinCharacter* goblin = GetWorld()->SpawnActor<ARangeGoblinCharacter>(RangeGoblinClass, goblinTransform);
		return IsValid(goblin);
	}
	return false;
}
