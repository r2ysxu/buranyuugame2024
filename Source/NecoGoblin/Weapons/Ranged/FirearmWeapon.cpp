// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "../../Characters/Humanoid.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"

const FName MUZZLE = FName("Muzzle");

// Sets default values
AFirearmWeapon::AFirearmWeapon() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	class USceneComponent* sceneRoot = CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot");
	SetRootComponent(sceneRoot);

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FirearmWeaponMesh");
	WeaponMeshComponent->SetupAttachment(GetRootComponent());
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	UBoxComponent* box = CreateDefaultSubobject<UBoxComponent>("Box");
	box->SetBoxExtent(FVector(50.f, 50.f, 0));
	box->SetupAttachment(GetRootComponent());
}

void AFirearmWeapon::ChangeWeapon(FName WeaponKey) {
	WeaponData = WeaponDataTable->FindRow<FFirearmWeaponData>(WeaponKey, FString("MainFirearm"), true);
	if (WeaponData) {
		WeaponReloadStop();
		if (WeaponData->WeaponMesh) {
			WeaponMeshComponent->SetSkeletalMesh(WeaponData->WeaponMesh);
		}
	}
}

void AFirearmWeapon::BeginPlay() {
	Super::BeginPlay();
	ChangeWeapon(FName("AK-47"));
	WeaponMeshComponent->SetRelativeScale3D(FVector(0.7, 0.7, 0.7));
	WeaponMeshComponent->AddLocalRotation(FRotator(0.f, 180, 20.f));
	RefillAmmo(30 * 10);
}

void AFirearmWeapon::WeaponFireStart() {
	IsFiring = true;
}

void AFirearmWeapon::WeaponFireStop() {
	IsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(InitiateFireHandler);
}

void AFirearmWeapon::WeaponReloadStop() {
	WeaponReloaded = true;
	const int reloadedAmmo = MaxAmmoInMagazine() - CurrentAmmoInMagazine;
	CurrentAmmoInMagazine = FMath::Min(ReserveAmmo, MaxAmmoInMagazine());
	ReserveAmmo -= FMath::Min(ReserveAmmo, reloadedAmmo);
}

FireType AFirearmWeapon::FireWeapon(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult& OutResult, float FireRateModifier, float HeadshotDmgModifier) {
	if (!WeaponData) return FireType::VE_NotFired;
	if (CurrentAmmoInMagazine > 0 && WeaponReloaded) {
		WeaponFireStart();
		CurrentAmmoInMagazine--;
		if (WeaponData->FireAnimation) WeaponMeshComponent->PlayAnimation(WeaponData->FireAnimation, false);
		if (MuzzleFX) UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMeshComponent, MUZZLE, FVector(), FRotator(), EAttachLocation::SnapToTargetIncludingScale, true);
		if (WeaponData->ShotSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData->ShotSound, GetActorLocation());
		FVector endLocation = startLocation + (forwardVector * MaxRange);
		collisionParams.AddIgnoredActor(this);
		if (GetWorld()->LineTraceSingleByChannel(OUT OutResult, startLocation, endLocation, ECollisionChannel::ECC_Pawn, collisionParams)) {
			// DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Emerald, false, 3.0f);
			AHumanoid* targetActor = Cast<AHumanoid>(OutResult.GetActor());
			if (targetActor && targetActor->GetTeam() != GetWeaponTeam()) {
				float finalDamage = GetWeaponDamage();
				if (FName("head").IsEqual(OutResult.BoneName)) finalDamage *= (2 + HeadshotDmgModifier);
				targetActor->TakeHitDamage(finalDamage, this);
				if (!targetActor->CheckAlive()) {
					return FireType::VE_Killed;
				}
				if (BloodHitFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodHitFX, OutResult.ImpactPoint);
			}
		}
		return FireType::VE_Fired;
	}
	return FireType::VE_NotFired;
}

void AFirearmWeapon::EquipWeapon(FName SocketName) {
	AttachToComponent(Wielder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

float AFirearmWeapon::GetWeaponDamage() {
	return ((WeaponData) ? WeaponData->BaseDamage : 10.f) * DamageModifier;
}

uint8 AFirearmWeapon::GetWeaponTeam() {
	return Wielder->GetTeam();
}

bool AFirearmWeapon::IsSemiAutomatic() {
	return (WeaponData) ? WeaponData->SemiAutomatic : true;
}

float AFirearmWeapon::GetFireRate() {
	return (WeaponData) ? FMath::Min(WeaponData->FireRate, 0.1f) : 1.f;
}

float AFirearmWeapon::GetReloadSpeedModifier() {
	return WeaponData ? WeaponData->ReloadSpeed : 1.f;
}

bool AFirearmWeapon::ReloadWeapon(float ReloadSpeed) {
	if (ReserveAmmo <= 0) return false;
	if (WeaponData->ReloadSound && WeaponReloaded) UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData->ReloadSound, GetActorLocation());
	WeaponReloaded = false;
	GetWorld()->GetTimerManager().SetTimer(InitiateReloadHandler, this, &AFirearmWeapon::WeaponReloadStop, ReloadSpeed);
	return true;
}

int AFirearmWeapon::MaxAmmoInMagazine() {
	return (WeaponData) ? WeaponData->MagazineSize : 0.f;
}

void AFirearmWeapon::UpgradeDamageModifier(float additionalModifier) {
	DamageModifier += additionalModifier;
}

void AFirearmWeapon::RefillAmmo(int Amount) {
	ReserveAmmo = Amount;
}

FFirearmStats* AFirearmWeapon::GetStats() {
	FFirearmStats* stats = new FFirearmStats();
	if (WeaponData) {
		stats->Name = WeaponData->WeaponName;
		stats->DamageP = WeaponData->BaseDamage / stats->DamageP;
		stats->FireRateP = 1.f - WeaponData->FireRate * stats->FireRateP;
		stats->HandlingP = stats->HandlingP - (WeaponData->RecoilYawVariance + WeaponData->RecoiPitchVariance);
		stats->InfoText = WeaponData->StatInfo;
	}
	return stats;
}

FireType AFirearmWeapon::OnFire(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult &OutResult, float FireRateModifier, float HeadshotDmgModifier) {
	if (!IsFiring) {
		GetWorld()->GetTimerManager().SetTimer(InitiateFireHandler, this, &AFirearmWeapon::WeaponFireStop, WeaponData->FireRate, false);
		return FireWeapon(startLocation, forwardVector, collisionParams, OutResult, FireRateModifier, HeadshotDmgModifier);
	}
	return FireType::VE_NotFired;
}

FVector2D AFirearmWeapon::GenerateRecoil() {
	return FVector2D(
		FMath::RandRange(-WeaponData->RecoilYawVariance, WeaponData->RecoilYawVariance),
		FMath::RandRange(-WeaponData->RecoiPitchVariance, 0.f)
	);
}
