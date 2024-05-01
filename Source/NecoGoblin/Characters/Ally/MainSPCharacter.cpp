// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSPCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMainSPCharacter::AMainSPCharacter() {}

void AMainSPCharacter::BeginPlay() {
	Super::BeginPlay();
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	GetMesh()->SetVisibility(false);
	if (Firearm) Firearm->SetVisible(false);
}

void AMainSPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::OnSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainSPCharacter::OnSprintStop);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::OnAimModeStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMainSPCharacter::OnAimModeStop);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::OnFireWeapon);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMainSPCharacter::OnFireStop);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::OnReloadWeapon);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::OnInteract);

		EnhancedInputComponent->BindAction(InfoAction, ETriggerEvent::Completed, this, &AMainSPCharacter::OnShowSkills);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AMainSPCharacter::OnScrollAxis);
	}
}

void AMainSPCharacter::Move(const FInputActionValue& Value) {
	if (!IsAlive) return;
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

void AMainSPCharacter::Look(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	FRotator playerRotation = (GetControlRotation() - GetActorRotation()).GetNormalized();
	SetPlayerPitch(playerRotation.Pitch);
}

void AMainSPCharacter::OnFireWeaponOnce() {
	if (Firearm->IsWeaponFireable()) {
		FVector camStart = GetCameraBoom()->GetComponentLocation() + GetCameraBoom()->GetForwardVector();
		FFireResponse fireResponse = FireWeapon(camStart, FollowCamera->GetForwardVector());
		switch (fireResponse.Type) {
		case EFireType::VE_NotFired: break;
		case EFireType::VE_Hit: OnHitTarget(fireResponse.Target, fireResponse.ImpactPoint, fireResponse.bHeadshot);
		case EFireType::VE_Fired:
			Firearm->PlayFireEffects();
			Recoil += Firearm->GenerateRecoil();
			break;
		case EFireType::VE_Killed: break;
		}
	}
}
