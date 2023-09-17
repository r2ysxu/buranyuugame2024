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

void AGoblinSpawner::SpawnGoblin() {
	if (GameMode) {
		if (GameMode->IncrementGoblin()) {
			FTransform goblinTransform;
			goblinTransform.SetLocation(GetActorLocation());
			goblinTransform.SetRotation(FQuat(FRotator::ZeroRotator));

			AMeleeGoblinCharacter* goblin = GetWorld()->SpawnActor<AMeleeGoblinCharacter>(GoblinClass, goblinTransform);
		}
	}
}

// Called when the game starts or when spawned
void AGoblinSpawner::BeginPlay() {
	Super::BeginPlay();
	GameMode = Cast<ANecoGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode) {
		GetWorld()->GetTimerManager().SetTimer(OnSpawnHandler, this, &AGoblinSpawner::SpawnGoblin, GameMode->GetGoblinSpawnRate(), true);
	}
}

