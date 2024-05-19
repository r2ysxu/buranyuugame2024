// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletCasing.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletCasing::ABulletCasing() {
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	MeshComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	MeshComponent->SetupAttachment(GetRootComponent());
	SetActorEnableCollision(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
}

// Called when the game starts or when spawned
void ABulletCasing::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) MeshComponent->SetStaticMesh(Mesh);
	ProjectileMovement->SetVelocityInLocalSpace(FVector(-100.f + FMath::RandRange(0.f, 10.f), 0.f, 0.f));
	ProjectileMovement->Activate();
	GetWorldTimerManager().SetTimer(FlightHandler, this, &ABulletCasing::DestoryAfterSeconds, 5.f, false);
}

void ABulletCasing::DestoryAfterSeconds() {
	Destroy();
}
