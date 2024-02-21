// Fill out your copyright notice in the Description page of Project Settings.


#include "NecoSpirit.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


ANecoSpirit::ANecoSpirit() {
	HeadBox->SetSphereRadius(1.f);
}

void ANecoSpirit::OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this) return;
	OnMeleeHit(actor, 1.f);
}

void ANecoSpirit::OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this) return;
	OnMeleeHit(actor, 1.f);
}

void ANecoSpirit::OnStopFlinching() {
	IsFlinching = false;
}

void ANecoSpirit::BeginPlay() {
	Super::BeginPlay();
	SetupFirearmWeapon();
}

void ANecoSpirit::SetupFirearmWeapon() {
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform weaponTransform;
	weaponTransform.SetLocation(FVector::ZeroVector);
	weaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	Firearm = GetWorld()->SpawnActor<AFirearmWeapon>(GetFirearmWeaponClass(), weaponTransform, spawnParams);
	if (Firearm) {
		Firearm->AttachToWielder(this, FirearmSocketName);
	}
}

void ANecoSpirit::OnFireWeapon() {}

void ANecoSpirit::HealthPot(float HealAmount) {}
