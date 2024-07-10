// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMPCharacter.h"
#include "../../Gamemodes/MultiplayerGameMode.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"


AMainMPCharacter::AMainMPCharacter() {
	ReviveBox = CreateDefaultSubobject<USphereComponent>(TEXT("ReviveBox"));
	ReviveBox->SetupAttachment(GetMesh());
	ReviveBox->SetSphereRadius(25.f);
	ReviveBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//ReviveBox->bHiddenInGame = false;
}

void AMainMPCharacter::BeginPlay() {
	Super::BeginPlay();
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMainMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::OnSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainMPCharacter::OnSprintStop);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::OnAimModeStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMainMPCharacter::OnAimModeStop);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::OnFireWeapon);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMainMPCharacter::OnFireStop);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::OnReloadWeapon);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::OnInteract);

		EnhancedInputComponent->BindAction(InfoAction, ETriggerEvent::Completed, this, &AMainMPCharacter::OnShowSkills);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AMainMPCharacter::OnScrollAxis);
	}
}

void AMainMPCharacter::Move(const FInputActionValue& Value) {
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

void AMainMPCharacter::Look(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	FRotator playerRotation = (GetControlRotation() - GetActorRotation()).GetNormalized();
	SetPlayerPitch(playerRotation.Pitch);
	//if (!HasAuthority()) Server_SetRotation(GetActorRotation(), playerRotation.Pitch);
}

void AMainMPCharacter::OnDeadBodyTouched(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (AAllyBase* teammate = Cast<AAllyBase>(actor)) {
		OnRevivePlayer();
	}
}

void AMainMPCharacter::OnInteract() {
	if (!HasAuthority()) {
		Server_OnInteract();
	} else {
		Server_OnInteract_Implementation();
	}
}

void AMainMPCharacter::Server_OnInteract_Implementation() {
	Super::OnInteract();
	Multicast_OnInteract();
}

void AMainMPCharacter::Multicast_OnInteract_Implementation() {
	Super::OnInteract();
}

void AMainMPCharacter::OnRevivePlayer() {
	if (IsAlive) return;
	ReviveBox->OnComponentBeginOverlap.RemoveDynamic(this, &AMainMPCharacter::OnDeadBodyTouched);
	if (!HasAuthority()) {
		Server_OnRevivePlayer();
	} else {
		Multicast_OnRevivePlayer();
	}
}

void AMainMPCharacter::Server_OnRevivePlayer_Implementation() {
	Super::OnRevivePlayer();
	Multicast_OnRevivePlayer();
}

void AMainMPCharacter::Multicast_OnRevivePlayer_Implementation() {
	Super::OnRevivePlayer();
}

void AMainMPCharacter::Server_SetupCharacters_Implementation() {
	OnCharacterStart();
	OnCharacterShow();
}

void AMainMPCharacter::Server_SetRotation_Implementation(FRotator Rotation, float Pitch) {
	SetActorRotation(Rotation);
	SetPlayerPitch(Pitch);
}

void AMainMPCharacter::OnStartAim() {
	if (IsAimMode) return;
	Super::OnStartAim();
	if (!HasAuthority()) {
		Server_OnStartAim();
	} else {
		Multicast_OnStartAim();
	}
}

void AMainMPCharacter::Server_OnStartAim_Implementation() {
	if (!IsAimMode && !IsSkillMenuOpen) {
		IsAimMode = true;
		Multicast_OnStartAim();
	}
}

void AMainMPCharacter::Multicast_OnStartAim_Implementation() {
	Server_OnStartAim_Implementation();
}

void AMainMPCharacter::OnStopAim() {
	if (!IsAimMode) return;
	Super::OnStopAim();
	if (!HasAuthority()) {
		Server_OnStopAim();
	} else {
		Multicast_OnStopAim();
	}
}

void AMainMPCharacter::Server_OnStopAim_Implementation() {
	if (IsAimMode) {
		IsAimMode = false;
		Multicast_OnStopAim();
	}
}

void AMainMPCharacter::Multicast_OnStopAim_Implementation() {
	Server_OnStopAim_Implementation();
}

void AMainMPCharacter::OnFireWeaponOnce() {
	if (Firearm->IsWeaponFireable()) {
		FVector camStart = GetCameraBoom()->GetComponentLocation() + GetCameraBoom()->GetForwardVector();
		if (!HasAuthority()) {
			Server_OnFireWeaponOnce(camStart, FollowCamera->GetForwardVector());
		} else {
			Server_OnFireWeaponOnce_Implementation(camStart, FollowCamera->GetForwardVector());
		}
	}
}

void AMainMPCharacter::Server_OnFireWeaponOnce_Implementation(FVector MuzzleLocation, FVector Direction) {
	FFireResponse fireResponse = FireWeapon(MuzzleLocation, Direction);
	switch (fireResponse.Type) {
		case EFireType::VE_NotFired: break;
		case EFireType::VE_Hit: OnHitTarget(fireResponse.Target, fireResponse.ImpactPoint, fireResponse.bHeadshot);
		case EFireType::VE_Fired:
			Multicast_OnFireWeaponOnceFired();
			Recoil += Firearm->GenerateRecoil();
			break;
		case EFireType::VE_Killed: break;
	}
}

void AMainMPCharacter::Multicast_OnFireWeaponOnceFired_Implementation() {
	Firearm->PlayFireEffects();
}

void AMainMPCharacter::OnHitTarget(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot) {
	Multicast_OnHitTarget(Target, ImpactPoint, IsHeadshot);
}

void AMainMPCharacter::Multicast_OnHitTarget_Implementation(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot) {
	Super::OnHitTarget(Target, ImpactPoint, IsHeadshot);
}

void AMainMPCharacter::OnReloadWeapon() {
	if (!HasAuthority()) {
		Server_OnReloadWeapon();
	} else {
		Server_OnReloadWeapon_Implementation();
	}
}

void AMainMPCharacter::Server_OnReloadWeapon_Implementation() {
	Super::OnReloadWeapon();
	Multicast_OnReloadWeapon();
}

void AMainMPCharacter::Multicast_OnReloadWeapon_Implementation() {
	Super::OnReloadWeapon();
}

void AMainMPCharacter::OnSprint() {
	if (!HasAuthority()) {
		Server_OnSprint();
	} else {
		Server_OnSprint_Implementation();
	}
}

void AMainMPCharacter::Server_OnSprint_Implementation() {
	Super::OnSprint();
	Multicast_OnSprint();
}

void AMainMPCharacter::Multicast_OnSprint_Implementation() {
	Super::OnSprint();
}

void AMainMPCharacter::OnSprintStop() {
	if (!HasAuthority()) {
		Server_OnSprintStop();
	} else {
		Server_OnSprintStop_Implementation();
	}
}

void AMainMPCharacter::Server_OnSprintStop_Implementation() {
	Super::OnSprintStop();
	Multicast_OnSprintStop();
}

void AMainMPCharacter::Multicast_OnSprintStop_Implementation() {
	Super::OnSprintStop();
}

int AMainMPCharacter::RefillAmmo(int AmmoAmount) {
	if (!HasAuthority()) {
		Server_OnRefillAmmo(AmmoAmount);
	}
	return Super::RefillAmmo(AmmoAmount);
}

void AMainMPCharacter::ChangeCharacterSkin(int IndexOffset) {
	if (!HasAuthority()) {
		Server_ChangeCharacterSkin(IndexOffset);
	} else {
		Super::ChangeCharacterSkin(IndexOffset);
		Multicast_ChangeCharacterSkin(IndexOffset);
	}
}

void AMainMPCharacter::Server_ChangeCharacterSkin_Implementation(int IndexOffset) {
	Super::ChangeCharacterSkin(IndexOffset);
	Multicast_ChangeCharacterSkin(IndexOffset);
}

void AMainMPCharacter::Multicast_ChangeCharacterSkin_Implementation(int IndexOffset) {
	Super::OnChangeCharacterSkin();
}

void AMainMPCharacter::Server_OnRefillAmmo_Implementation(int AmmoAmount) {
	Super::RefillAmmo(AmmoAmount);
	Multicast_OnRefillAmmo(AmmoAmount);
}

void AMainMPCharacter::Multicast_OnRefillAmmo_Implementation(int AmmoAmount) {
	Super::RefillAmmo(AmmoAmount);
}

bool AMainMPCharacter::CheckAlive() {
	if (!IsAlive) return false;
	if (CurrentHealth <= 0) {
		IsAlive = false;
		Tags.Remove(FName("MainPlayer"));
		GetCharacterMovement()->StopMovementImmediately();
		GetMovementComponent()->Deactivate();
		//DisableInput(Cast<APlayerController>(GetController()));
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		Server_NotifyDead();
		ReviveBox->OnComponentBeginOverlap.AddDynamic(this, &AMainMPCharacter::OnDeadBodyTouched);
	}
	return IsAlive;
}

void AMainMPCharacter::UpgradeSkill(FNecoSkills Skill) {
	if (!HasAuthority()) {
		Server_UpgradeSkill(Skill);
	} else {
		Server_UpgradeSkill_Implementation(Skill);
	}
}

void AMainMPCharacter::Server_UpgradeSkill_Implementation(FNecoSkills Skill) {
	Multicast_UpgradeSkill(Skill);
}

void AMainMPCharacter::Multicast_UpgradeSkill_Implementation(FNecoSkills Skill) {
	Super::UpgradeSkill(Skill);
}


void AMainMPCharacter::Server_NotifyDead_Implementation() {
	AMultiplayerGameMode* gameMode = Cast<AMultiplayerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->OnPlayerDead(0.f);
	//if (IsValid(gameMode)) {
	//	gameMode->DelegateGameOver.Broadcast(20.f);
	//}
}