// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "NiagaraComponent.h"
#include "Engine/DataTable.h"
#include "FirearmWeapon.generated.h"

USTRUCT(BlueprintType)
struct FFirearmWeaponData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString WeaponName;
	UPROPERTY(EditAnywhere)
		class USkeletalMesh* WeaponMesh;
	UPROPERTY(EditAnywhere)
		class UAnimationAsset* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ShotSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere)
		int MagazineSize;
	UPROPERTY(EditAnywhere)
		int ReloadSpeed;
	UPROPERTY(EditAnywhere)
		float FireRate;
	UPROPERTY(EditAnywhere)
		bool SemiAutomatic;
	UPROPERTY(EditAnywhere)
		float RecoilYawVariance;
	UPROPERTY(EditAnywhere)
		float RecoiPitchVariance;
	UPROPERTY(EditAnywhere)
		float BaseDamage;
};

UENUM(BlueprintType)
enum class FireType : uint8 {
	VE_NotFired UMETA(DisplayName = "NotFired"),
	VE_Fired	UMETA(DisplayName = "Fired"),
	VE_Hit		UMETA(DisplayName = "Hit"),
	VE_Killed   UMETA(Displayname = "Killed"),
};

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AFirearmWeapon : public AWeapon
{
	GENERATED_BODY()

private:
	FireType FireWeapon(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult& OutResult);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponKey = FName("AK-47");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	class UNiagaraSystem* BloodHitFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	class UNiagaraSystem* MuzzleFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UDataTable* weaponDataTable;
	class USkeletalMeshComponent* WeaponMeshComponent;
	FFirearmWeaponData* WeaponData;
	FTimerHandle InitiateFireHandler;
	FTimerHandle InitiateReloadHandler;
	volatile bool IsFiring = false;
	volatile bool WeaponReloaded = true;

	float MaxRange = 5000.f;
	int CurrentAmmoInMagazine = 0;

	float DamageModifier = 1.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void WeaponFireStart();
	void WeaponFireStop();
	void WeaponReloadStop();

public:
	AFirearmWeapon();
	void EquipWeapon(FName SocketName) override;
	float GetWeaponDamage() override;
	uint8 GetWeaponTeam() override;
	bool IsSemiAutomatic();
	float GetFireRate();
	float GetReloadSpeedModifier();
	void ReloadWeapon(float ReloadSpeed);
	int MaxAmmoInMagazine();
	FORCEINLINE int GetAmmoMagazine() { return CurrentAmmoInMagazine; }
	FORCEINLINE bool GetIsFiring() { return IsFiring; }
	FORCEINLINE bool GetIsReloading() { return !WeaponReloaded; }
	void UpgradeDamageModifier(float additionalModifier);
	
	FireType OnFire(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, FHitResult &OutResult);
	FVector2D GenerateRecoil();
};
