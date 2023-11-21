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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InfoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScrollAction;

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

	const float GameVolume = 1.f;
	const float MusicVolume = 1.f;

	void OnStopAim();
	void OnStartAim();
	void OnBelowWaterLevel();
	float GetMaxHealth();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	bool IsToggleAim = false;
	bool IsAutoReload = false;
	bool IsCharacterStart = false;
	bool IsSkillMenuOpen = false;
	bool CanFillAmmo = false;
	bool IsSprinting = false;
	volatile bool IsAimMode = false;
	FName SelectableWeaponKey = FName();
	float PlayerPitch = 0.f;
	float Stamina = MAX_STAMINA;
	FTimerHandle OnSprintHandler;
	FTimerHandle OnSprintRegenHandler;
	FTimerHandle OnFireWeaponHandler;
	FTimerHandle BloodSplatterHandler;
	FTimerHandle OnWaterLevelCheckHandler;
	FTimerHandle OnHealthRegenHandler;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* FlinchMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* GetupMontage;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	
	bool CheckAlive() override;

public:

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UUserWidget* GameOverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* RefillSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* BGMSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UNiagaraSystem* BloodHitFX = nullptr;
	class UNiagaraComponent* BloodSplatter = nullptr;

	UNecoCharacterStat* stats;
	UUpgradeSkillComponent* upgradeComponent;

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

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void UpgradeWeaponDamage(float additionalDamage);
	void OnAimModeStart();
	void OnAimModeStop();
	void OnFireWeapon();
	void OnFireWeaponOnce();
	void OnFireStop();
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
	
	virtual void TakeHitDamage(float damage, AActor* DamageCauser) override;
	virtual void HealthPot(float HealAmount);
	UFUNCTION(BlueprintCallable) void PlayGetupMontage();
	UFUNCTION(BlueprintCallable) void SetupHuds();
	UFUNCTION(BlueprintCallable) void OnShowSkills();
	UFUNCTION(BlueprintCallable) struct FFirearmStats GetFirearmStats();
	UFUNCTION(BlueprintCallable) FORCEINLINE int GetReserveAmmo() { return Firearm->GetReserveAmmo(); }

	void OnRemoveBloodSplatter();
};
