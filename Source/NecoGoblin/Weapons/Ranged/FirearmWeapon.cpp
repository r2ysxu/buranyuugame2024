// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "../../Characters/Humanoid.h"
#include "./BulletCasing.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"

const FName MUZZLE = FName("Muzzle");

// Sets default values
AFirearmWeapon::AFirearmWeapon() {
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot"));

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FirearmWeaponMesh");
	WeaponMeshComponent->SetupAttachment(GetRootComponent());
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
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
	//WeaponMeshComponent->AddLocalRotation(FRotator(0.f, 180, 20.f));
	WeaponMeshComponent->AddLocalOffset(FVector(0.f, -5.f, 0.f));
	WeaponMeshComponent->AddLocalRotation(FRotator(90.f, 180.f, 0.f));
	RefillAmmo(30 * 3);
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
	const int reloadedAmmo = FMath::Min(ReserveAmmo, MaxAmmoInMagazine() - CurrentAmmoInMagazine);
	CurrentAmmoInMagazine += reloadedAmmo;
	ReserveAmmo -= reloadedAmmo;
	Delegate_ReserveAmmoChange.Broadcast(ReserveAmmo);
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
	ReserveAmmo += Amount;
	Delegate_ReserveAmmoChange.Broadcast(ReserveAmmo);
}

void AFirearmWeapon::SetVisible(bool Visible) {
	if (WeaponMeshComponent) WeaponMeshComponent->SetVisibility(Visible);
}

void AFirearmWeapon::SetGunVolume(const float VolumeMultiplier) {
	GunVolume = VolumeMultiplier;
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

bool AFirearmWeapon::IsWeaponFireable() {
	return WeaponData && CurrentAmmoInMagazine > 0 && WeaponReloaded && !IsFiring;
}

void AFirearmWeapon::PlayFireEffects() {
	if (MuzzleFX) UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMeshComponent, MUZZLE, FVector(), FRotator(), EAttachLocation::SnapToTargetIncludingScale, true);
	if (WeaponData->ShotSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponData->ShotSound, GetActorLocation(), GunVolume);
	CurrentAmmoInMagazine--;
	//if (WeaponData->FireAnimation) WeaponMeshComponent->PlayAnimation(WeaponData->FireAnimation, false);

	FTransform casingTransform;
	casingTransform.SetLocation(WeaponMeshComponent->GetSocketLocation(FName("b_gun_shelleject")));
	GetWorld()->SpawnActor<ABulletCasing>(BulletCasingClass, casingTransform);
}

FFireResponse AFirearmWeapon::OnFire(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, float FireRateModifier, float WeaponDamageModifier, float HeadshotDmgModifier) {
	if (!IsWeaponFireable()) return FFireResponse(EFireType::VE_NotFired);
	FHitResult outResult;
	WeaponFireStart();
	GetWorld()->GetTimerManager().SetTimer(InitiateFireHandler, this, &AFirearmWeapon::WeaponFireStop, WeaponData->FireRate * FireRateModifier, false);
	FVector endLocation = startLocation + (forwardVector * MaxRange);
	collisionParams.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Green, false, 3.f, 0U, 1.f);
	if (GetWorld()->LineTraceSingleByChannel(OUT outResult, startLocation, endLocation, ECollisionChannel::ECC_Pawn, collisionParams)) {
		if (AHumanoid* target = Cast<AHumanoid>(outResult.GetActor())) {
			return FFireResponse(EFireType::VE_Hit, FString(TEXT("HeadBox")) == outResult.GetComponent()->GetName(), target, outResult.ImpactPoint);
		}
	}
	return FFireResponse(EFireType::VE_Fired);
}

FVector2D AFirearmWeapon::GenerateRecoil() {
	return FVector2D(
		FMath::RandRange(-WeaponData->RecoilYawVariance, WeaponData->RecoilYawVariance),
		FMath::RandRange(-WeaponData->RecoiPitchVariance, 0.f)
	);
}
