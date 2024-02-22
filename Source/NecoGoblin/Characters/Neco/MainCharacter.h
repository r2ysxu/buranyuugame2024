// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NecoSpirit.h"
#include "NecoCharacterStat.h"
#include "UpgradeSkillComponent.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


UCLASS(config=Game)
class AMainCharacter : public ANecoSpirit {
	GENERATED_BODY()

private:

	const float CAMERA_SCROLL_SPEED = 5.f;
	const float CameraArmLengthOffset = 100.f;
	const float FRAMES_PER_MAG = 2.f;
	const float RELOAD_SPEED = 2.f;
	const float WALK_SPEED = 300.f;
	const float SPRINT_SPEED = 500.f;
	const float MAX_STAMINA = 20.f;
	const float WATER_LEVEL = 1200.f;
	const int POINTS_PER_KILL = 1;
	const int RESERVE_AMMO = 30 * 10;

	float GameVolume = 1.f;
	float MusicVolume = 1.f;

	void OnBelowWaterLevel();
	float GetMaxHealth();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> FirearmWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> CrosshairHudWidgetClass;
	UUserWidget* CrosshairHudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> HudWidgetClass;
	UUserWidget* HudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> SkillHudWidgetClass;
	UUserWidget* SkillHudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* RefillSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* EmptyMagSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* BGMSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voices", meta = (AllowPrivateAccess = "true"))
	USoundBase* HitVoice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voices", meta = (AllowPrivateAccess = "true"))
	USoundBase* LowHealthVoice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voices", meta = (AllowPrivateAccess = "true"))
	USoundBase* NoStaminaVoice;
	class UAudioComponent* NoStaminaVoiceComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voices", meta = (AllowPrivateAccess = "true"))
	USoundBase* HealthPickupVoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* FlinchMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* GetupMontage;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UNiagaraSystem* BloodHitFX = nullptr;
	class UNiagaraComponent* BloodSplatter = nullptr;

	UNecoCharacterStat* stats;
	UUpgradeSkillComponent* upgradeComponent;

	bool IsToggleAim = false;
	bool IsAutoReload = false;
	bool IsCharacterStart = false;
	bool IsSkillMenuOpen = false;
	bool CanFillAmmo = false;
	bool IsSprinting = false;
	volatile bool IsAimMode = false;
	FName SelectableWeaponKey = FName();
	float PlayerPitch = 0.f;
	FVector2D Recoil = FVector2D();
	float Stamina = MAX_STAMINA;
	FTimerHandle OnSprintHandler;
	FTimerHandle OnSprintRegenHandler;
	FTimerHandle OnFireWeaponHandler;
	FTimerHandle BloodSplatterHandler;
	FTimerHandle OnWaterLevelCheckHandler;
	FTimerHandle OnHealthRegenHandler;

	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	void OnAimModeStart();
	void OnAimModeStop();
	void OnFireWeapon();
	void OnFireStop();
	FFireResponse FireWeapon(FVector MuzzleLocation, FVector Direction);
	virtual void OnStopAim();
	virtual void OnStartAim();
	virtual void OnFireWeaponOnce();
	virtual void OnHitTarget(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot);

public:
	AMainCharacter();

	UFUNCTION() void OnScrollAxis(const FInputActionValue& Value);
	FORCEINLINE TSubclassOf<class AWeapon> GetFirearmWeaponClass() { return FirearmWeaponClass; }
	UFUNCTION(BlueprintImplementableEvent) void OnCameraDistanceChanged();
	UFUNCTION(BLueprintCallable) float GetCameraDistance();
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsFlinching() { return IsFlinching; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsAimMode() { return IsAimMode; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsToggleAim() { return IsToggleAim; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsToggleAim(bool isToggleAim) { IsToggleAim = isToggleAim; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsAutoReload() { return IsAutoReload; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsAutoReload(bool isAutoReload) { IsAutoReload = isAutoReload; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetPlayerPitch() { return PlayerPitch; }
	void SetPlayerPitch(float Pitch);
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsSkillMenuOpen() { return IsSkillMenuOpen; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UNecoCharacterStat* GetStats() { return stats; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UUpgradeSkillComponent* GetSkillUpgrades() { return upgradeComponent; }
	UFUNCTION(BlueprintCallable) void OnCharacterShow();
	UFUNCTION(BlueprintCallable) void OnCharacterStart();
	UFUNCTION(BlueprintCallable) void PlayBGMusic();
	UFUNCTION(BlueprintCallable) float GetReloadUIFrame();
	UFUNCTION(BlueprintCallable) bool GetIsFiringWeapon();
	UFUNCTION(BlueprintCallable) bool GetIsReloading();
	UFUNCTION(BlueprintCallable) float GetHealthPercentage() { return CurrentHealth / GetMaxHealth(); }
	UFUNCTION(BlueprintCallable) float GetStaminaPercentage() { return Stamina / (MAX_STAMINA * upgradeComponent->GetStaminaModifier()); }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetGameVolume() { return GameVolume; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetGameVolume(float Volume) { GameVolume = Volume; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetMusicVolume() { return MusicVolume; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetMusicVolume(float Volume) { MusicVolume = Volume; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void UpgradeWeaponDamage(float additionalDamage);
	void OnReloadWeapon();
	void OnInteract();
	void OnSprint();
	void OnSprintStop();
	void OnHealthRegen();
	void SetChangableWeapon(FName WeaponKey);
	void StaminaRegen();
	void StaminaDrain();
	void SetRunSpeed(float MovementSpeedModifier);
	void AddMaxHP(float AdditionalHP);
	int RefillAmmo(int AmmoAmount);

	virtual bool CheckAlive() override;
	virtual void TakeHitDamage(float damage, AActor* DamageCauser) override;
	virtual void HealthPot(float HealAmount);
	UFUNCTION(BlueprintCallable) void PlayGetupMontage();
	UFUNCTION(BlueprintCallable) void SetupHuds();
	UFUNCTION(BlueprintCallable) void OnShowSkills();
	UFUNCTION(BlueprintCallable) struct FFirearmStats GetFirearmStats();
	UFUNCTION(BlueprintCallable) FORCEINLINE int GetReserveAmmo() { return Firearm->GetReserveAmmo(); }

	void OnRemoveBloodSplatter();
};
