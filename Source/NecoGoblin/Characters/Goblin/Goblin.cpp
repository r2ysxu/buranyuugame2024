// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "../../Pickups/HealthPickup.h"
#include "../../Gamemodes/GoblinGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"


AGoblin::AGoblin() {
	HeadBox->SetSphereRadius(3.f);
	HeadBox->AddLocalOffset(FVector(0.f, -2.f, 0.f));
	HeadBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AGoblin::BeginPlay() {
	Super::BeginPlay();
	GameMode = Cast<AGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AGoblin::Multicast_SpawnHealthPot_Implementation(FVector Location) {
	FTransform transform;
	transform.SetLocation(Location);
	AHealthPickup* healthPickup = GetWorld()->SpawnActor<AHealthPickup>(HealthPickupClass, transform);
}

bool AGoblin::CheckAlive() {
	if (CurrentHealth <= 0) {
		if (HasAuthority()) {
			Cast<AGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->DecrementEnemy();
			if (FMath::RandRange(0, 100) <= HealthPickupSpawnRate) {
				Multicast_SpawnHealthPot_Implementation(GetActorLocation());
				Multicast_SpawnHealthPot(GetActorLocation());
			}
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