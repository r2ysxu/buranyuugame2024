// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "../../Characters/Humanoid.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"

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
	box->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void AFirearmWeapon::BeginPlay() {
	Super::BeginPlay();
	WeaponData = weaponDataTable->FindRow<FFirearmWeaponData>(WeaponKey, FString("MainFirearm"), true);
	if (WeaponData) {
		CurrentAmmoInMagazine = WeaponData->MagazineSize;
		if (WeaponData->WeaponMesh) {
			WeaponMeshComponent->SetRelativeScale3D(FVector(0.7, 0.7, 0.7));
			WeaponMeshComponent->SetSkeletalMesh(WeaponData->WeaponMesh);
			WeaponMeshComponent->AddLocalRotation(FRotator(0.f, 180, 20.f));
		}
	}
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
	CurrentAmmoInMagazine = (WeaponData) ? WeaponData->MagazineSize : 0.f;
}

bool AFirearmWeapon::FireWeapon(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult& OutResult) {
	if (!WeaponData) return false;
	if (CurrentAmmoInMagazine > 0 && WeaponReloaded) {
		CurrentAmmoInMagazine--;
		if (WeaponData->FireAnimation) WeaponMeshComponent->PlayAnimation(WeaponData->FireAnimation, false);
		FVector endLocation = startLocation + (forwardVector * MaxRange);
		collisionParams.AddIgnoredActor(this);
		if (GetWorld()->LineTraceSingleByChannel(OUT OutResult, startLocation, endLocation, ECollisionChannel::ECC_Pawn, collisionParams)) {
			WeaponFireStart();
			DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Emerald, false, 3.0f);
			AHumanoid* targetActor = Cast<AHumanoid>(OutResult.GetActor());
			if (targetActor && targetActor->GetTeam() != GetWeaponTeam()) {
				targetActor->TakeHitDamage(GetWeaponDamage(), this);
				if (BloodHitFX) {
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodHitFX, OutResult.ImpactPoint);
				}
			}
		}
		return true;
	}
	return false;
}

void AFirearmWeapon::EquipWeapon(FName SocketName) {
	AttachToComponent(Wielder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

float AFirearmWeapon::GetWeaponDamage() {
	return (WeaponData) ? WeaponData->BaseDamage : 10.f;
}

uint8 AFirearmWeapon::GetWeaponTeam() {
	return Wielder->GetTeam();
}

bool AFirearmWeapon::IsSemiAutomatic() {
	return (WeaponData) ? WeaponData->SemiAutomatic : true;
}

float AFirearmWeapon::GetFireRate() {
	return (WeaponData) ? std::min(WeaponData->FireRate, 0.1f) : 1.f;
}

float AFirearmWeapon::GetReloadSpeedModifier() {
	return WeaponData ? WeaponData->ReloadSpeed : 1.f;
}

void AFirearmWeapon::ReloadWeapon(float ReloadSpeed) {
	WeaponReloaded = false;
	GetWorld()->GetTimerManager().SetTimer(InitiateReloadHandler, this, &AFirearmWeapon::WeaponReloadStop, ReloadSpeed);
}

int AFirearmWeapon::MaxAmmoInMagazine() {
	return (WeaponData) ? WeaponData->MagazineSize : 0.f;
}

bool AFirearmWeapon::OnFire(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult &OutResult) {
	if (!IsFiring) {
		GetWorld()->GetTimerManager().SetTimer(InitiateFireHandler, this, &AFirearmWeapon::WeaponFireStop, WeaponData->FireRate, false);
		return FireWeapon(startLocation, forwardVector, collisionParams, OutResult);
	}
	return false;
}

FVector2D AFirearmWeapon::GenerateRecoil() {
	return FVector2D(
		FMath::RandRange(-WeaponData->RecoilYawVariance, WeaponData->RecoilYawVariance),
		FMath::RandRange(-WeaponData->RecoiPitchVariance, 0.f)
	);
}
