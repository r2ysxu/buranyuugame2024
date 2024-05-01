// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "MainMPCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMainMPCharacter : public AMainCharacter {
	GENERATED_BODY()

protected:

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UCharacterSwitcherMenuWidget> CharacterSwitcherMenuClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	class USphereComponent* ReviveBox;

	class UCharacterSwitcherMenuWidget* CharacterSwitcherMenu;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetupHud();
	virtual void Look(const FInputActionValue& Value) override;
	void Move(const FInputActionValue& Value);

	UFUNCTION() void OnDeadBodyTouched(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable) void Server_SetupCharacters();
	UFUNCTION(Server, Unreliable) void Server_SetRotation(FRotator Rotation, float Pitch);

	virtual void OnStartAim() override;
	UFUNCTION(Server, Reliable) void Server_OnStartAim();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnStartAim();

	virtual void OnStopAim() override;
	UFUNCTION(Server, Reliable) void Server_OnStopAim();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnStopAim();


	virtual void OnFireWeaponOnce() override;
	UFUNCTION(Server, Reliable) void Server_OnFireWeaponOnce(FVector StartLocation, FVector Direction);
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnFireWeaponOnceFired();

	virtual void OnHitTarget(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot) override;
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnHitTarget(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot);

	virtual void OnReloadWeapon() override;
	UFUNCTION(Server, Reliable) void Server_OnReloadWeapon();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnReloadWeapon();

	virtual void OnSprint() override;
	UFUNCTION(Server, Reliable) void Server_OnSprint();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnSprint();

	virtual void OnSprintStop() override;
	UFUNCTION(Server, Reliable) void Server_OnSprintStop();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnSprintStop();

	virtual int RefillAmmo(int AmmoAmount) override;
	UFUNCTION(Server, Reliable) void Server_OnRefillAmmo(int AmmoAmount);
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnRefillAmmo(int AmmoAmount);

	virtual void ChangeCharacterSkin(int SkinIndex) override;
	UFUNCTION(Server, Reliable) void Server_ChangeCharacterSkin(int SkinIndex);
	UFUNCTION(NetMultiCast, Reliable) void Multicast_ChangeCharacterSkin(int SkinIndex);

	virtual void OnInteract() override;
	UFUNCTION(Server, Reliable) void Server_OnInteract();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnInteract();

	virtual void OnRevivePlayer();
	UFUNCTION(Server, Reliable) void Server_OnRevivePlayer();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnRevivePlayer();

	UFUNCTION(Server, Reliable) void Server_UpgradeSkill(enum FNecoSkills Skill);
	UFUNCTION(NetMulticast, Reliable) void Multicast_UpgradeSkill(enum FNecoSkills Skill);

	UFUNCTION(Server, Reliable) void Server_NotifyDead();

public:
	AMainMPCharacter();

	virtual bool CheckAlive() override;
	virtual void UpgradeSkill(enum FNecoSkills Skill) override;
};
