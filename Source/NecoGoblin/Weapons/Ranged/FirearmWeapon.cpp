// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "../../Characters/Humanoid.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AFirearmWeapon::AFirearmWeapon() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FirearmWeaponMesh");
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMeshComponent);
}

void AFirearmWeapon::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) WeaponMeshComponent->SetSkeletalMesh(Mesh);
}

void AFirearmWeapon::WeaponFireStart() {
	IsFiring = true;
}

void AFirearmWeapon::WeaponFireStop() {
	IsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(InitiateFireHandler);
}

bool AFirearmWeapon::FireWeapon(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult& OutResult) {
	FVector endLocation = startLocation + (forwardVector * MaxRange);
	collisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(OUT OutResult, startLocation, endLocation, ECollisionChannel::ECC_Pawn, collisionParams)) {
		WeaponFireStart();
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Emerald, false, 3.0f);
		AHumanoid* targetActor = Cast<AHumanoid>(OutResult.GetActor());
		if (targetActor && targetActor->GetTeam() != GetWeaponTeam()) {
			targetActor->TakeHitDamage(GetWeaponDamage(), this);
		}
	} else return false;
	return true;
}

void AFirearmWeapon::EquipWeapon(FName SocketName) {
	AttachToComponent(Wielder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

float AFirearmWeapon::GetWeaponDamage() {
	return 50.0f;
}

uint8 AFirearmWeapon::GetWeaponTeam() {
	return Wielder->GetTeam();
}

bool AFirearmWeapon::OnFire(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult &OutResult) {
	if (!IsFiring) {
		FireWeapon(startLocation, forwardVector, collisionParams, OutResult);
		GetWorld()->GetTimerManager().SetTimer(InitiateFireHandler, this, &AFirearmWeapon::WeaponFireStop, FireRate, false);
	}

	return true;
}
