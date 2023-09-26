// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"
#include "Components/SphereComponent.h"

// Sets default values
APickupable::APickupable() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	class USceneComponent* sceneRoot = CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot");
	SetRootComponent(sceneRoot);

	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("OneHandWeaponMesh");
	PickupMeshComponent->SetupAttachment(GetRootComponent());
	PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupRangeBox = CreateDefaultSubobject<USphereComponent>(TEXT("ShopRangeBox"));
	PickupRangeBox->SetSphereRadius(30.f);
	PickupRangeBox->AddLocalOffset(FVector(0.f, 15.f, 0.f));
	PickupRangeBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupRangeBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PickupRangeBox->SetupAttachment(GetRootComponent());
	// PickupRangeBox->bHiddenInGame = false;
}

void APickupable::OnWithinPickupRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	OnPickup(Cast<ANecoSpirit>(actor));
}

// Called when the game starts or when spawned
void APickupable::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) PickupMeshComponent->SetStaticMesh(Mesh);
}