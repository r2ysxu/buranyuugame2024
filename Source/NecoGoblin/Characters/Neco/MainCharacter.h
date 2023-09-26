// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NecoSpirit.h"
#include "NecoCharacterStat.h"
#include "UpgradeSkillComponent.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h>
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

private:

	const float CameraArmLengthOffset = 100.f;
	const float FRAMES_PER_MAG = 2.f;
	const float RELOAD_SPEED = 1.5f;
	const float WALK_SPEED = 300.f;
	const float SPRINT_SPEED = 500.f;
	const float MAX_STAMINA = 20.f;
	const float WATER_LEVEL = 1200.f;
	const int POINTS_PER_KILL = 10;
	const int RESERVE_AMMO = 30 * 10;

	void SetupHuds();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	bool CanFillAmmo = false;
	bool IsSprinting = false;
	bool IsAimMode = false;
	float PlayerPitch = 0.f;
	float Stamina = MAX_STAMINA;
	FTimerHandle OnSprintHandler;
	FTimerHandle OnFireWeaponHandler;
	FTimerHandle GameOverHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class UAnimMontage* ReloadFirearmMontage;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	
	bool CheckAlive() override;
	UFUNCTION(BlueprintCallable) void OnCharacterStart();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* RefillSound;

	UNecoCharacterStat* stats;
	UUpgradeSkillComponent* upgradeComponent;

	AMainCharacter();

	FORCEINLINE TSubclassOf<class AWeapon> GetFirearmWeaponClass() { return FirearmWeaponClass; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsAimMode() { return IsAimMode; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetPlayerPitch() { return PlayerPitch; }
	UFUNCTION(BlueprintCallable) float GetReloadUIFrame();
	UFUNCTION(BlueprintCallable) bool GetIsFiringWeapon();
	UFUNCTION(BlueprintCallable) bool GetIsReloading();
	UFUNCTION(BlueprintCallable) float GetHealthPercentage() { return CurrentHealth / MaxHealth; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UNecoCharacterStat* GetStats() { return stats; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UUpgradeSkillComponent* GetSkillUpgrades() { return upgradeComponent; }

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
	void CanRefillAmmo(bool Fillable);
	void RefillAmmo();
	void StaminaGen();

	void GameRestart();

	UFUNCTION(BlueprintCallable) FORCEINLINE int GetReserveAmmo() { return Firearm->GetReserveAmmo(); }

};

