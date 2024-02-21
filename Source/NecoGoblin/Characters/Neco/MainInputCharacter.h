// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "MainInputCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMainInputCharacter : public AMainCharacter {
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

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable) void Server_SetupCharacters();
	UFUNCTION(Server, Unreliable) void Server_SetRotation(FRotator Rotation, float Pitch);

	virtual void OnStartAim() override;
	UFUNCTION(Server, Reliable) void Server_OnStartAim();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnStartAim();

	virtual void OnStopAim() override;
	UFUNCTION(Server, Reliable) void Server_OnStopAim();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnStopAim();


	virtual void OnFireWeaponOnce() override;
	UFUNCTION(Server, Reliable) void Server_OnFireWeaponOnce();
	UFUNCTION(NetMulticast, Reliable) void Multicast_OnFireWeaponOnce();

public:
	
};
