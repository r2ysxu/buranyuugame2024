// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "../Goblin/Goblin.h"
#include "../../NecoGoblinGameMode.h"

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
#include "Niagara/Public/NiagaraFunctionLibrary.h"


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
	if (Firearm) Firearm->SetVisible(false);

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
	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
}

bool AMainCharacter::CheckAlive() {
	if (!Super::CheckAlive()) {
		GetCharacterMovement()->StopMovementImmediately();
		if (GameOverWidget) GameOverWidget->AddToViewport();
		GetWorld()->GetTimerManager().SetTimer(GameOverHandler, this, &AMainCharacter::GameRestart, 2.f, false);
		return false;
	}
	return true;
}

void AMainCharacter::OnCharacterShow() {
	GetMesh()->SetVisibility(true);
	if (Firearm) Firearm->SetVisible(true);
}

void AMainCharacter::OnCharacterStart() {
	if (!IsCharacterStart) {
		SetupHuds();
		PlayBGMusic();
		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameOnly());
		IsCharacterStart = true;
	}
}

void AMainCharacter::PlayBGMusic() {
	if (BGMSound) UGameplayStatics::PlaySound2D(GetWorld(), BGMSound, MusicVolume);
}

void AMainCharacter::UpgradeWeaponDamage(float additionalDamage) {
	if (Firearm) Firearm->UpgradeDamageModifier(additionalDamage);
}

void AMainCharacter::OnStopAim() {
	if (IsAimMode) {
		IsAimMode = false;
		GetCameraBoom()->SetRelativeLocation(FVector::ZeroVector);
		GetCameraBoom()->TargetArmLength += CameraArmLengthOffset;
		bUseControllerRotationYaw = IsAimMode;
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainCharacter::OnStartAim() {
	if (!IsAimMode && !IsSkillMenuOpen) {
		IsAimMode = true;
		if (IsSprinting) OnSprintStop();
		GetCameraBoom()->SetRelativeLocation(FVector(0, 50, 50));
		GetCameraBoom()->TargetArmLength -= CameraArmLengthOffset;
		bUseControllerRotationYaw = IsAimMode;
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainCharacter::OnAimModeStart() {
	if (!IsToggleAim) {
		OnStartAim();
	}
}

void AMainCharacter::OnAimModeStop() {
	if (!IsToggleAim || IsAimMode) {
		OnStopAim();
	} else if (!IsAimMode) {
		OnStartAim();
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
		upgradeComponent->GetWeaponDamageModifier(),
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
		if (IsAutoReload && Firearm->GetAmmoMagazine() == 0) OnReloadWeapon();
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
	OnStopAim();
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
	int refillAmount = FMath::Min(AmmoAmount, RESERVE_AMMO * upgradeComponent->GetAdditionalReserveAmmoModifier() - GetReserveAmmo());
	if (RefillSound && refillAmount > 0) UGameplayStatics::PlaySound2D(GetWorld(), RefillSound, 5.f);
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
	if (BloodHitFX && BloodSplatter == nullptr) {
		BloodSplatter = UNiagaraFunctionLibrary::SpawnSystemAttached(BloodHitFX, GetCapsuleComponent(), NAME_None, FVector(0.f, 0.f, 10.f), GetActorRotation(), EAttachLocation::Type::SnapToTarget, true);
		GetWorld()->GetTimerManager().SetTimer(BloodSplatterHandler, this, &AMainCharacter::OnRemoveBloodSplatter, 1.f, false);
	}
}

void AMainCharacter::HealthPot(float HealAmount) {
	const float totalHealAmount = HealAmount + upgradeComponent->GetAdditionalHeal();
		CurrentHealth = FMath::Min(CurrentHealth + totalHealAmount, MaxHealth);
}

void AMainCharacter::OnShowSkills() {
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

void AMainCharacter::SetGameVolume(float VolumeMultiplier) {
	GameVolume = VolumeMultiplier;
	Firearm->SetGunVolume(GameVolume);
}

void AMainCharacter::SetMusicVolume(float VolumeMultiplier) {
	MusicVolume = VolumeMultiplier;
}

void AMainCharacter::OnRemoveBloodSplatter() {
	if (BloodSplatter) {
		BloodSplatter->DestroyComponent();
		BloodSplatter = nullptr;
	}
}

void AMainCharacter::OnScrollAxis(const FInputActionValue& Value) {
	CameraBoom->TargetArmLength = FMath::Min(400, FMath::Max(50, CameraBoom->TargetArmLength - (Value.Get<float>() * CAMERA_SCROLL_SPEED)));
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

		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AMainCharacter::OnScrollAxis);
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