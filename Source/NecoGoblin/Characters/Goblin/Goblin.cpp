// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "Kismet/GameplayStatics.h"
#include "../../Pickups/HealthPickup.h"
#include "NecoGoblin/NecoGoblinGameMode.h"


void AGoblin::BeginPlay() {
	Super::BeginPlay();
	GameMode = Cast<ANecoGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool AGoblin::CheckAlive() {
	if (CurrentHealth <= 0 && FMath::RandRange(0, 100) <= HealthPickupSpawnRate) {
		GameMode->DecrementGoblin();

		AHealthPickup* healthPickup = GetWorld()->SpawnActor<AHealthPickup>(HealthPickupClass);
		healthPickup->SetActorLocation(GetActorLocation());
	}
	return Super::CheckAlive();
}

void AGoblin::OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}

void AGoblin::OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}