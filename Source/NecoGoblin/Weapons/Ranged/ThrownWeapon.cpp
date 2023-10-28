// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrownWeapon.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AThrownWeapon::AThrownWeapon() {
	PrimaryActorTick.bCanEverTick = false;

	MeleeAttackWeaponBox = CreateDefaultSubobject<UBoxComponent>("OneHandWeaponAttackBox");
	MeleeAttackWeaponBox->SetBoxExtent(FVector(50.f, 1.f, 0));
	MeleeAttackWeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeAttackWeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	MeleeAttackWeaponBox->SetupAttachment(GetRootComponent());
	MeleeAttackWeaponBox->bHiddenInGame = false;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	WeaponMeshComponent->SetupAttachment(MeleeAttackWeaponBox);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComponent->AddLocalRotation(FRotator(0, 90.f, 0));
	WeaponMeshComponent->AddLocalOffset(FVector(-50.f, 0.f, 0));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
}

void AThrownWeapon::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) WeaponMeshComponent->SetStaticMesh(Mesh);
	ProjectileMovement->Deactivate();
	MeleeAttackWeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AThrownWeapon::OnHitBeginOverlap);
}

void AThrownWeapon::EquipWeapon(FName SocketName) {
	AttachToComponent(Wielder->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

float AThrownWeapon::GetWeaponDamage() {
	return 10.f;
}

uint8 AThrownWeapon::GetWeaponTeam() {
	return Wielder->GetTeam();
}

bool AThrownWeapon::OnFire(FVector TossVelocity) {
	if (!IsInFlight) {
		IsInFlight = true;
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		WeaponMeshComponent->AddLocalOffset(FVector(50.f, -80.f, 0));
		WeaponMeshComponent->AddLocalRotation(FRotator(0, 90.f, 0));
		ProjectileMovement->Velocity = TossVelocity;
		ProjectileMovement->Activate();
	}
	return true;
}

void AThrownWeapon::OnHitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == Wielder || !IsInFlight) return;
	if (actor != Wielder) IsInFlight = false;
	if (Cast<AThrownWeapon>(actor)) return;
	ProjectileMovement->Deactivate();
	AttachToActor(actor, FAttachmentTransformRules::KeepWorldTransform);
	AHumanoid* hitTarget = Cast<AHumanoid>(actor);
	if (IsValid(hitTarget)) {
		hitTarget->TakeHitDamage(GetWeaponDamage(), Wielder);
	}
	GetWorld()->GetTimerManager().SetTimer(FlightResetHandler, this, &AThrownWeapon::OnResetProjectile, 10.f, false);
}

void AThrownWeapon::OnResetProjectile() {
	GetWorld()->GetTimerManager().ClearTimer(FlightResetHandler);
	Destroy();
}
