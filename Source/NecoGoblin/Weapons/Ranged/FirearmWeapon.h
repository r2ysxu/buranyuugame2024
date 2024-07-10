// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "NiagaraComponent.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "FirearmWeapon.generated.h"

UENUM(BlueprintType)
enum class EFireType : uint8 {
	VE_NotFired UMETA(DisplayName = "NotFired"),
	VE_Fired	UMETA(DisplayName = "Fired"),
	VE_Hit		UMETA(DisplayName = "Hit"),
	VE_Killed   UMETA(Displayname = "Killed"),
};

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
	USoundCue* ShotSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundCue* ReloadSound;
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
	UPROPERTY(EditAnywhere)
	FString StatInfo;
};

USTRUCT(BlueprintType)
struct FFirearmStats {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageP = 40.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FireRateP = 4.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HandlingP = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString InfoText;
};

USTRUCT(BlueprintType)
struct FFireResponse {
	GENERATED_BODY()

	EFireType Type;
	bool bHeadshot;
	FVector ImpactPoint;
	class AHumanoid* Target;

	FFireResponse() { Type = EFireType::VE_NotFired; }
	FFireResponse(EFireType ResponseType) {
		Type = ResponseType;
	}
	FFireResponse(EFireType ResponseType, bool IsHeadshot, class AHumanoid* HitTarget, FVector HitImpactPoint) {
		Type = ResponseType;
		bHeadshot = IsHeadshot;
		Target = HitTarget;
		ImpactPoint = HitImpactPoint;
	}
};

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AFirearmWeapon : public AWeapon {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_ReserveAmmoChange, int, ReserveAmmo);

private:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	class UNiagaraSystem* BloodHitFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	class UNiagaraSystem* MuzzleFX = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UDataTable* WeaponDataTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ABulletCasing> BulletCasingClass;

	class USkeletalMeshComponent* WeaponMeshComponent;
	class UNiagaraComponent* MuzzleFXComponent;
	FFirearmWeaponData* WeaponData;
	FTimerHandle InitiateFireHandler;
	FTimerHandle InitiateReloadHandler;
	volatile bool IsFiring = false;
	volatile bool WeaponReloaded = true;
	int ReserveAmmo = 30;
	
	float MaxRange = 5000.f;
	int CurrentAmmoInMagazine = 0;

	float DamageModifier = 1.f;
	float GunVolume = 1.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void WeaponFireStart();
	void WeaponFireStop();
	void WeaponReloadStop();

public:
	AFirearmWeapon();

	UPROPERTY(BlueprintAssignable)
	FDelegate_ReserveAmmoChange Delegate_ReserveAmmoChange;

	void ChangeWeapon(FName WeaponKey);
	void EquipWeapon(FName SocketName) override;
	float GetWeaponDamage() override;
	uint8 GetWeaponTeam() override;
	bool IsSemiAutomatic();
	float GetFireRate();
	float GetReloadSpeedModifier();
	bool ReloadWeapon(float ReloadSpeed);
	int MaxAmmoInMagazine();
	void UpgradeDamageModifier(float additionalModifier);
	void RefillAmmo(int Amount);
	void SetVisible(bool Visible);
	void SetGunVolume(const float VolumeMultiplier);
	FFirearmStats* GetStats();
	FORCEINLINE int GetReserveAmmo() { return ReserveAmmo; }
	FORCEINLINE int GetAmmoMagazine() { return CurrentAmmoInMagazine; }
	FORCEINLINE bool GetIsFiring() { return IsFiring; }
	FORCEINLINE bool GetIsReloading() { return !WeaponReloaded; }
	FORCEINLINE float GetGunVolume() { return GunVolume; }
	
	bool IsWeaponFireable();
	void PlayFireEffects();
	FFireResponse OnFire(FVector startLocation, FVector forwardVector, FCollisionQueryParams collisionParams, float FireRateModifier = 1.f, float WeaponDamageModifier = 1.f, float HeadshotDmgModifier = 1.f);
	FVector2D GenerateRecoil();
};
