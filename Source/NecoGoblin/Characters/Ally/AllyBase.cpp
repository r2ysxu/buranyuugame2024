// Fill out your copyright notice in the Description page of Project Settings.


#include "AllyBase.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


AAllyBase::AAllyBase() {
	HeadBox->SetSphereRadius(1.f);
}

void AAllyBase::OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this) return;
	OnMeleeHit(actor, 1.f);
}

void AAllyBase::OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this) return;
	OnMeleeHit(actor, 1.f);
}

void AAllyBase::OnStopFlinching() {
	IsFlinching = false;
}

void AAllyBase::BeginPlay() {
	Super::BeginPlay();
	SetupFirearmWeapon();
}

void AAllyBase::SetupFirearmWeapon() {
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform weaponTransform;
	weaponTransform.SetLocation(FVector::ZeroVector);
	weaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	Firearm = GetWorld()->SpawnActor<AFirearmWeapon>(GetFirearmWeaponClass(), weaponTransform, spawnParams);
	if (Firearm) {
		Firearm->AttachToWielder(this, FirearmSocketName);
		Firearm->SetOwner(this);
	}
}

void AAllyBase::OnFireWeapon() {}

void AAllyBase::HealthPot(float HealAmount) {}
