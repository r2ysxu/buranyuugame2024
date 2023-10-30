// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "../Goblin/Goblin.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>


//////////////////////////////////////////////////////////////////////////
// AMainCharacter

AMainCharacter::AMainCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(15.f, 30.f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	stats = CreateDefaultSubobject<UNecoCharacterStat>(TEXT("CharacterStats"));
	upgradeComponent = CreateDefaultSubobject<UUpgradeSkillComponent>(TEXT("UpgradeSkillComponent"));
	upgradeComponent->SetParentCharacter(this);
	
	Tags.Add(FName("MainPlayer"));
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	HeadBox->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnHeadHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnBodyHit);
	GetMesh()->SetVisibility(false);

	GetWorld()->GetTimerManager().SetTimer(OnSprintRegenHandler, this, &AMainCharacter::StaminaRegen, 0.5f, true);
}

void AMainCharacter::SetupHuds() {
	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	if (HudWidget) {
		HudWidget->AddToViewport();
	}
	CrosshairHudWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairHudWidgetClass);
	if (CrosshairHudWidget) {
		CrosshairHudWidget->AddToViewport();
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	SkillHudWidget = CreateWidget<UUserWidget>(GetWorld(), SkillHudWidgetClass);
	upgradeComponent->SetupWidget(SkillHudWidget);
}

bool AMainCharacter::CheckAlive() {
	if (!Super::CheckAlive()) {
		GetCharacterMovement()->StopMovementImmediately();
		GetWorld()->GetTimerManager().SetTimer(GameOverHandler, this, &AMainCharacter::GameRestart, 2.f, false);
		return false;
	}
	return true;
}

void AMainCharacter::OnCharacterStart() {
	GetMesh()->SetVisibility(true);
}

void AMainCharacter::UpgradeWeaponDamage(float additionalDamage) {
	if (Firearm) Firearm->UpgradeDamageModifier(additionalDamage);
}

void AMainCharacter::OnAimModeStart() {
	if (!IsAimMode && !IsSkillMenuOpen) {
		if (IsSprinting) OnSprintStop();
		IsAimMode = true;
		GetCameraBoom()->SetRelativeLocation(FVector(0, 50, 50));
		GetCameraBoom()->TargetArmLength -= CameraArmLengthOffset;
		bUseControllerRotationYaw = IsAimMode;
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainCharacter::OnAimModeStop() {
	if (IsAimMode) {
		IsAimMode = false;
		GetCameraBoom()->SetRelativeLocation(FVector::ZeroVector);
		GetCameraBoom()->TargetArmLength += CameraArmLengthOffset;
		bUseControllerRotationYaw = IsAimMode;
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainCharacter::OnFireWeaponOnce() {
	const float targetArmLength = GetCameraBoom()->TargetArmLength;
	FVector camStart = GetCameraBoom()->GetComponentLocation() + GetCameraBoom()->GetForwardVector();
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(GetOwner());

	FHitResult result;
	const FireType fireResponse = Firearm->OnFire(
		camStart,
		FollowCamera->GetForwardVector(),
		collisionParams,
		result,
		upgradeComponent->GetFireRateModifier(),
		upgradeComponent->GetHeadshotModifier()
	);
	switch(fireResponse) {
	case FireType::VE_Killed:
		stats->IncrementKillCount();
		upgradeComponent->AddExpPoints(POINTS_PER_KILL);
	case FireType::VE_Fired:
		FVector2D recoil = Firearm->GenerateRecoil();
		Look(FInputActionValue(recoil));
	}
}

void AMainCharacter::OnFireWeapon() {
	if (Firearm && IsAimMode && !IsSkillMenuOpen) {
		OnFireWeaponOnce();
		if (!Firearm->IsSemiAutomatic()) {
			GetWorld()->GetTimerManager().SetTimer(OnFireWeaponHandler, this, &AMainCharacter::OnFireWeaponOnce, Firearm->GetFireRate(), true);
		}
	}
}

void AMainCharacter::OnFireStop() {
	if (Firearm) {
		GetWorld()->GetTimerManager().ClearTimer(OnFireWeaponHandler);
	}
}

void AMainCharacter::OnReloadWeapon() {
	float animationDelay = RELOAD_SPEED * Firearm->GetReloadSpeedModifier() * upgradeComponent->GetReloadSpeedModifier();
	Firearm->ReloadWeapon(animationDelay);
}

void AMainCharacter::OnInteract() {
	if (CanFillAmmo) {
		Firearm->RefillAmmo(RESERVE_AMMO * upgradeComponent->GetAdditionalReserveAmmoModifier());
	} else if (SelectableWeaponKey != FName()) {
		Firearm->ChangeWeapon(SelectableWeaponKey);
	}
}

void AMainCharacter::OnSprint() {
	if (Stamina <= 0 || IsSprinting) return;
	OnAimModeStop();
	IsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SPRINT_SPEED * upgradeComponent->GetMovementSpeedModifer();
	GetWorld()->GetTimerManager().SetTimer(OnSprintHandler, this, &AMainCharacter::StaminaDrain, 0.1f, true);
}

void AMainCharacter::OnSprintStop() {
	if (IsSprinting) {
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
		GetWorld()->GetTimerManager().UnPauseTimer(OnSprintRegenHandler);
		GetWorld()->GetTimerManager().ClearTimer(OnSprintHandler);
	}
}

void AMainCharacter::SetChangableWeapon(FName WeaponKey) {
	SelectableWeaponKey = WeaponKey;
}

int AMainCharacter::RefillAmmo(int AmmoAmount) {
	if (RefillSound) UGameplayStatics::PlaySound2D(GetWorld(), RefillSound, 5.f);
	int refillAmount = FMath::Min(AmmoAmount, RESERVE_AMMO * upgradeComponent->GetAdditionalReserveAmmoModifier() - GetReserveAmmo());
	Firearm->RefillAmmo(refillAmount);
	return refillAmount;
}

void AMainCharacter::StaminaRegen() {
	if (IsSprinting || Stamina >= MAX_STAMINA * upgradeComponent->GetStaminaModifier()) {
		GetWorld()->GetTimerManager().PauseTimer(OnSprintRegenHandler);
	} else {
		Stamina = FMath::Min(MAX_STAMINA * upgradeComponent->GetStaminaModifier(), Stamina + 1.f);
	}
}

void AMainCharacter::StaminaDrain() {
	if (Stamina <= 0) {
		OnSprintStop();
	} else {
		IsSprinting = true;
		Stamina = FMath::Max(0.f, Stamina - 1.f);
	}
}

void AMainCharacter::SetRunSpeed(float MovementSpeedModifier) {
	GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED * MovementSpeedModifier;
}

void AMainCharacter::AddMaxHP(float AdditionalHP) {
	MaxHealth += AdditionalHP;
	CurrentHealth += AdditionalHP;
}

void AMainCharacter::PlayGetupMontage() {
	if (GetupMontage) PlayAnimMontage(GetupMontage, 1.f);
}

FFirearmStats AMainCharacter::GetFirearmStats() {
	return (IsValid(Firearm)) ? *Firearm->GetStats() : FFirearmStats();
}

void AMainCharacter::TakeHitDamage(float damage, AActor* DamageCauser) {
	Super::TakeHitDamage(damage, DamageCauser);
	if (FlinchMontage) PlayAnimMontage(FlinchMontage, 1.f);
}

void AMainCharacter::HealthPot(float HealAmount) {
	const float totalHealAmount = HealAmount + upgradeComponent->GetAdditionalHeal();
		CurrentHealth = FMath::Min(CurrentHealth + totalHealAmount, MaxHealth);
}

void AMainCharacter::OnShowSkills() {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("OnShowSkills"));
	if (!IsSkillMenuOpen) {
		upgradeComponent->EnterScreen();
		IsSkillMenuOpen = true;
	} else {
		upgradeComponent->ExitScreen();
		IsSkillMenuOpen = false;
	}
}

void AMainCharacter::GameRestart() {
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

float AMainCharacter::GetReloadUIFrame() {
	if (!Firearm) return 0.f;
	return float(Firearm->MaxAmmoInMagazine() - Firearm->GetAmmoMagazine()) * FRAMES_PER_MAG;
}

bool AMainCharacter::GetIsReloading() {
	return Firearm && Firearm->GetIsReloading();
}

bool AMainCharacter::GetIsFiringWeapon() {
	return Firearm && Firearm->GetIsFiring();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
	
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainCharacter::OnSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainCharacter::OnSprintStop);

		//Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMainCharacter::OnAimModeStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMainCharacter::OnAimModeStop);

		//Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainCharacter::OnFireWeapon);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMainCharacter::OnFireStop);

		//Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainCharacter::OnReloadWeapon);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainCharacter::OnInteract);

		//Info
		EnhancedInputComponent->BindAction(InfoAction, ETriggerEvent::Completed, this, &AMainCharacter::OnShowSkills);
	}
}

void AMainCharacter::Move(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (GetActorLocation().Z < WATER_LEVEL) {
			TakeHitDamage(CurrentHealth, nullptr);
			CheckAlive();
		}
	}
}

void AMainCharacter::Look(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		FRotator playerRotation = (GetControlRotation() - GetActorRotation()).GetNormalized();
		PlayerPitch = playerRotation.Pitch;
	}
}