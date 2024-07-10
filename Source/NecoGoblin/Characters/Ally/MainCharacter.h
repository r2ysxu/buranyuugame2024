// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AllyBase.h"
#include "NecoCharacterStat.h"
#include "UpgradeSkillComponent.h"
#include "../../Weapons/Ranged/FirearmWeapon.h"

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


UCLASS(config=Game)
class AMainCharacter : public AAllyBase {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_HealthChange, float, Health);

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

	void OnBelowWaterLevel();
	float GetMaxHealth();
	void SpawnMagazineActor();

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
	TSubclassOf<class UUserWidget> LoadingScreenWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> CrosshairHudWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UCharacterHUD> HudWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class USkillsMenuWidget> SkillHudWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class AMagazineActor> MagazineActorBPClass;

	UUserWidget* LoadingScreenWidget;
	UUserWidget* CrosshairHudWidget;
	class UCharacterHUD* HudWidget;
	class USkillsMenuWidget* SkillHudWidget;
	class AMagazineActor* MagazineActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<USkeletalMesh*> CharacterMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<UClass*> CharacterAnimClasses;

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
	TArray<class UAnimMontage*> FlinchMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> GetupMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UNiagaraSystem* BloodHitFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UNiagaraSystem* BloodShotFX = nullptr;
	class UNiagaraComponent* BloodSplatter = nullptr;

	UPROPERTY(ReplicatedUsing = OnChangeCharacterSkin)
	int CharacterSkinIndex = 0;

	UNecoCharacterStat* stats;
	UUpgradeSkillComponent* upgradeComponent;

	int CharacterIndex = 0;


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
	virtual void SetupHuds();
	virtual void Tick(float DeltaSeconds) override;
	virtual void Look(const FInputActionValue& Value) {}
	virtual void OnStopAim();
	virtual void OnStartAim();
	virtual void OnFireWeaponOnce();
	virtual void OnHitTarget(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot);
	virtual void OnReloadWeapon();
	virtual void OnSprint();
	virtual void OnSprintStop();
	virtual void OnRemoveBloodSplatter();
	virtual void OnRevivePlayer();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void OnAimModeStart();
	void OnAimModeStop();
	void OnFireWeapon();
	void OnFireStop();
	FFireResponse FireWeapon(FVector MuzzleLocation, FVector Direction);

public:
	AMainCharacter();
	
	UPROPERTY(BlueprintAssignable)
	FDelegate_HealthChange Delegate_HealthChange;

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
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsCharacterStart() { return IsCharacterStart; }
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
	UFUNCTION(BlueprintCallable) int GetCharacterSkinSize();
	UFUNCTION(BlueprintCallable) float GetHealthPercentage() { return CurrentHealth / GetMaxHealth(); }
	UFUNCTION(BlueprintCallable) float GetStaminaPercentage() { return Stamina / (MAX_STAMINA * upgradeComponent->GetStaminaModifier()); }
	UFUNCTION() void OnChangeCharacterSkin();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetCharacterIndex(int Index);
	int GetCharacterIndex();
	int GetCharacterSkinIndex();
	void UpgradeWeaponDamage(float additionalDamage);
	void OnHealthRegen();
	void SetChangableWeapon(FName WeaponKey);
	void StaminaRegen();
	void StaminaDrain();
	void SetRunSpeed(float MovementSpeedModifier);
	void AddMaxHP(float AdditionalHP);

	virtual	void OnInteract();
	virtual int RefillAmmo(int AmmoAmount);
	virtual bool CheckAlive() override;
	virtual void TakeHitDamage(float damage, AActor* DamageCauser) override;
	virtual void HealthPot(float HealAmount);
	virtual void UpgradeSkill(FNecoSkills Skill);
	virtual void ChangeCharacterSkin(int IndexOffset);
	UFUNCTION(BlueprintCallable) void PlayGetupMontage();
	UFUNCTION(BlueprintCallable) void OnShowSkills();
	UFUNCTION(BlueprintCallable) struct FFirearmStats GetFirearmStats();
	UFUNCTION() class AFirearmWeapon* GetWeapon();
};
