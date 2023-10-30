// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "../../Pickups/HealthPickup.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "NecoGoblin/NecoGoblinGameMode.h"
#include "Components/CapsuleComponent.h"


AGoblin::AGoblin() {
	HeadBox->SetSphereRadius(3.f);
	HeadBox->AddLocalOffset(FVector(0.f, -2.f, 0.f));
}

void AGoblin::BeginPlay() {
	Super::BeginPlay();
	GameMode = Cast<ANecoGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool AGoblin::CheckAlive() {
	if (CurrentHealth <= 0) {
		GameMode->DecrementEnemy();
		if (FMath::RandRange(0, 100) <= HealthPickupSpawnRate) {
			AHealthPickup* healthPickup = GetWorld()->SpawnActor<AHealthPickup>(HealthPickupClass);
			healthPickup->SetActorLocation(GetActorLocation());
		}
	}
	if (!Super::CheckAlive()) {
		GetCapsuleComponent()->DestroyComponent();
		return false;
	} else return true;
}

void AGoblin::SetRunSpeed(float MovementSpeedModiifer) {}

void AGoblin::OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

void AGoblin::OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}