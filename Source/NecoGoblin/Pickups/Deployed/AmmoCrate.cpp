// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoCrate.h"
#include "../../Characters/Neco/MainCharacter.h"

#include "Blueprint/UserWidget.h"

void AAmmoCrate::RegenBullets() {
	AmmoAvailable = FMath::Min(MAX_AMMO, AmmoAvailable + AmmoRegenAmount);
}

// Sets default values
AAmmoCrate::AAmmoCrate() {
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("CrateMesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAmmoCrate::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) MeshComponent->SetStaticMesh(Mesh);
	GetWorld()->GetTimerManager().SetTimer(OnAmmoRegenHandler, this, &AAmmoCrate::RegenBullets, AmmoRegenRate, true, FMath::FRandRange(0.f, 1.f));
}

void AAmmoCrate::OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(actor);
	if (IsValid(mainCharacter)) {
		AmmoAvailable -= mainCharacter->RefillAmmo(AmmoAvailable);
	}
}

void AAmmoCrate::OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {}

int AAmmoCrate::GetAmmoAvailable() {
	return AmmoAvailable;
}

