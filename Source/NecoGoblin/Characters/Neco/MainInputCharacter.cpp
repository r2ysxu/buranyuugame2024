// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInputCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

void AMainInputCharacter::BeginPlay() {
	Super::BeginPlay();
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (!HasAuthority()) {
		OnCharacterStart();
		OnCharacterShow();
	} else {
		Server_SetupCharacters();
	}
}

void AMainInputCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::OnSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainInputCharacter::OnSprintStop);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::OnAimModeStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMainInputCharacter::OnAimModeStop);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::OnFireWeapon);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMainInputCharacter::OnFireStop);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::OnReloadWeapon);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::OnInteract);

		EnhancedInputComponent->BindAction(InfoAction, ETriggerEvent::Completed, this, &AMainInputCharacter::OnShowSkills);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AMainInputCharacter::OnScrollAxis);
	}
}

void AMainInputCharacter::Move(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMainInputCharacter::Look(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	FRotator playerRotation = (GetControlRotation() - GetActorRotation()).GetNormalized();
	SetPlayerPitch(playerRotation.Pitch);
	if (!HasAuthority()) Server_SetRotation(GetActorRotation(), playerRotation.Pitch);
}

void AMainInputCharacter::Server_SetupCharacters_Implementation() {
	OnCharacterStart();
	OnCharacterShow();
}

void AMainInputCharacter::Server_SetRotation_Implementation(FRotator Rotation, float Pitch) {
	SetActorRotation(Rotation);
	SetPlayerPitch(Pitch);
}

void AMainInputCharacter::OnStartAim() {
	Super::OnStartAim();
	if (!HasAuthority()) {
		Server_OnStartAim();
	} else {
		Multicast_OnStartAim();
	}
}

void AMainInputCharacter::Server_OnStartAim_Implementation() {
	if (!IsAimMode && !IsSkillMenuOpen) {
		IsAimMode = true;
		Multicast_OnStartAim();
	}
}

void AMainInputCharacter::Multicast_OnStartAim_Implementation() {
	Server_OnStartAim_Implementation();
}

void AMainInputCharacter::OnStopAim() {
	Super::OnStopAim();
	if (!HasAuthority()) {
		Server_OnStopAim();
	} else {
		Multicast_OnStopAim();
	}
}

void AMainInputCharacter::Server_OnStopAim_Implementation() {
	if (IsAimMode) {
		IsAimMode = false;
		Multicast_OnStopAim();
	}
}

void AMainInputCharacter::Multicast_OnStopAim_Implementation() {
	Server_OnStopAim_Implementation();
}

void AMainInputCharacter::OnFireWeaponOnce() {
	if (Firearm->IsWeaponFireable()) {
		FVector camStart = GetCameraBoom()->GetComponentLocation() + GetCameraBoom()->GetForwardVector();
		if (!HasAuthority()) {
			Server_OnFireWeaponOnce(camStart, FollowCamera->GetForwardVector());
		} else {
			Server_OnFireWeaponOnce_Implementation(camStart, FollowCamera->GetForwardVector());
		}
	}
}

void AMainInputCharacter::Server_OnFireWeaponOnce_Implementation(FVector MuzzleLocation, FVector Direction) {
	FHitResult result;
	FFireResponse fireResponse = FireWeapon(MuzzleLocation, Direction, OUT result);
	switch (fireResponse.Type) {
		case EFireType::VE_NotFired: break;
		case EFireType::VE_Hit: OnHitTarget(fireResponse.Target, fireResponse.ImpactPoint, fireResponse.bHeadshot);
		case EFireType::VE_Fired:
			Firearm->PlayFireEffects();
			Multicast_OnFireWeaponOnceFired();
			Recoil += Firearm->GenerateRecoil();
			break;
		case EFireType::VE_Killed: break;
	}
}

void AMainInputCharacter::Multicast_OnFireWeaponOnceFired_Implementation() {
	Firearm->PlayFireEffects();
}

void AMainInputCharacter::OnHitTarget(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot) {
	Multicast_OnHitTarget(Target, ImpactPoint, IsHeadshot);
}

void AMainInputCharacter::Multicast_OnHitTarget_Implementation(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot) {
	Super::OnHitTarget(Target, ImpactPoint, IsHeadshot);
}