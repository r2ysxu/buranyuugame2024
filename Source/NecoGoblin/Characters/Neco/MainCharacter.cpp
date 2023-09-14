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
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>


//////////////////////////////////////////////////////////////////////////
// AMainCharacter

const float cameraArmLengthOffset = 100.f;
const float FRAMES_PER_MAG = 2.f;
const int POINTS_PER_KILL = 10;
const float RELOAD_SPEED = 1.5f;

AMainCharacter::AMainCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(30.f, 60.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
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
	upgradeComponent = CreateDefaultSubobject<UUpgradeShopComponent>(TEXT("UpgradeShopComponent"));
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

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	if (HudWidget) {
		HudWidget->AddToViewport();
	}
	CrosshairHudWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairHudWidgetClass);
	if (CrosshairHudWidget) {
		CrosshairHudWidget->AddToViewport();
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	ShopHudWidget = CreateWidget<UUserWidget>(GetWorld(), ShopHudWidgetClass);
	if (ShopHudWidget) {
		ShopHudWidget->AddToViewport();
		ShopHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainCharacter::UpgradeWeaponDamage(float additionalDamage) {
	if (Firearm) Firearm->UpgradeDamageModifier(additionalDamage);
}

void AMainCharacter::OnAimModeStart() {
	if (!IsAimMode) {
		IsAimMode = true;
		FVector offset = FVector(0, 50, 50);
		GetCameraBoom()->SetRelativeLocation(offset);
		GetCameraBoom()->TargetArmLength -= cameraArmLengthOffset;
		bUseControllerRotationYaw = IsAimMode;
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainCharacter::OnAimModeStop() {
	if (IsAimMode) {
		IsAimMode = false;
		GetCameraBoom()->SetRelativeLocation(FVector::ZeroVector);
		GetCameraBoom()->TargetArmLength += cameraArmLengthOffset;
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
	const FireType fireResponse = Firearm->OnFire(camStart, FollowCamera->GetForwardVector(), collisionParams, result);
	switch(fireResponse) {
	case FireType::VE_Killed:
		stats->IncrementKillCount();
		upgradeComponent->IncrementPoints(POINTS_PER_KILL);
	case FireType::VE_Fired:
		FVector2D recoil = Firearm->GenerateRecoil();
		Look(FInputActionValue(recoil));
	}
}

void AMainCharacter::OnFireWeapon() {
	if (Firearm && IsAimMode) {
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
	//float animationDelay = PlayAnimMontage(ReloadFirearmMontage, Firearm->GetReloadSpeedModifier());
	float animationDelay = RELOAD_SPEED * Firearm->GetReloadSpeedModifier();
	Firearm->ReloadWeapon(animationDelay);
}

void AMainCharacter::OnInteract() {
	if (upgradeComponent->GetCanShop()) {
		upgradeComponent->EnterShopScreen(ShopHudWidget);
		GetMovementComponent()->StopActiveMovement();
	}
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
	}
}

void AMainCharacter::Move(const FInputActionValue& Value) {
	if (ShopHudWidget->GetVisibility() == ESlateVisibility::Visible) return;
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