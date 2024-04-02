// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "../Goblin/Goblin.h"
#include "../../NecoGoblinGameMode.h"
#include "../../Widgets/HUDs/CharacterHUD.h"
#include "../../Widgets/Actors/MagazineActor.h"
#include "../../Widgets/Menus/SkillsMenuWidget.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"


//////////////////////////////////////////////////////////////////////////
// AMainCharacter

AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(15.f, 30.f);
	GetCapsuleComponent()->bHiddenInGame = false;
		
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

	HeadBox->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnHeadHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnBodyHit);
	//GetMesh()->SetVisibility(false);
	//if (Firearm) Firearm->SetVisible(false);

	GetWorld()->GetTimerManager().SetTimer(OnSprintRegenHandler, this, &AMainCharacter::StaminaRegen, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(OnWaterLevelCheckHandler, this, &AMainCharacter::OnBelowWaterLevel, 1.f, true);
	GetWorld()->GetTimerManager().SetTimer(OnHealthRegenHandler, this, &AMainCharacter::OnHealthRegen, 10.f, true);
}

void AMainCharacter::Tick(float DeltaSeconds) {
	FVector2D recoilRate =  FMath::Vector2DInterpTo(FVector2D(), Recoil, DeltaSeconds, 5.f);
	Recoil -= recoilRate;
	Look(FInputActionValue(recoilRate));
}

void AMainCharacter::SetupHuds() {
	HudWidget = CreateWidget<UCharacterHUD>(GetWorld(), HudWidgetClass);
	if (HudWidget) {
		HudWidget->AddToViewport();
	}
	CrosshairHudWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairHudWidgetClass);
	if (CrosshairHudWidget) {
		CrosshairHudWidget->AddToViewport();
		CrosshairHudWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	SkillHudWidget = CreateWidget<USkillsMenuWidget>(GetWorld(), SkillHudWidgetClass);
	upgradeComponent->SetupWidget(SkillHudWidget);
}

bool AMainCharacter::CheckAlive() {
	if (!IsAlive) return false;
	if (CurrentHealth <= 0) {
		IsAlive = false;
		Tags.Remove(FName("MainPlayer"));
		if (HudWidget)	HudWidget->RemoveFromParent();
		if (SkillHudWidget) SkillHudWidget->RemoveFromParent();
		if (CrosshairHudWidget) CrosshairHudWidget->RemoveFromParent();
		GetCharacterMovement()->StopMovementImmediately();
		GetMovementComponent()->Deactivate();
		DisableInput(Cast<APlayerController>(GetController()));
		ANecoGoblinGameMode* gameMode = Cast<ANecoGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(gameMode)) {
			gameMode->DelegateGameOver.Broadcast(DECOMPOSE_DELAY);
		}
		GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
	}
	return IsAlive;
}

void AMainCharacter::OnRevivePlayer() {
	CurrentHealth = 10.f;
	IsAlive = true;
	Tags.Add(FName("MainPlayer"));
	GetMesh()->SetCollisionProfileName(FName("Custom"));
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -30.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMovementComponent()->Activate();
	EnableInput(Cast<APlayerController>(GetController()));
}

void AMainCharacter::SetPlayerPitch(float Pitch) {
	PlayerPitch = Pitch;
}

void AMainCharacter::OnCharacterShow() {
	GetMesh()->SetVisibility(true);
	if (Firearm) Firearm->SetVisible(true);
}

void AMainCharacter::OnCharacterStart() {
	if (!IsCharacterStart) {
		SetupHuds();
		PlayBGMusic();
		SpawnMagazineActor();
		APlayerController* controller = Cast<APlayerController>(GetController());
		if (IsValid(controller)) {
			controller->SetViewTargetWithBlend(this, 0.5f);
			controller->SetInputMode(FInputModeGameOnly());
			controller->bShowMouseCursor = false;
		}
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
		if (CrosshairHudWidget) CrosshairHudWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainCharacter::OnBelowWaterLevel() {
	if (GetActorLocation().Z < WATER_LEVEL && IsAlive) {
		TakeHitDamage(CurrentHealth, nullptr);
		CheckAlive();
	}
}

float AMainCharacter::GetMaxHealth() {
	return (MaxHealth + upgradeComponent->GetAdditionalHP());
}

void AMainCharacter::SpawnMagazineActor() {
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform transform;
	transform.SetLocation(FVector(-6680.f, 5190.f, -22070.f));

	MagazineActor = GetWorld()->SpawnActor<AMagazineActor>(MagazineActorBPClass, transform, spawnParams);
	MagazineActor->SetParent(this);
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

FFireResponse AMainCharacter::FireWeapon(FVector MuzzleLocation, FVector Direction) {
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	return Firearm->OnFire(
		MuzzleLocation,
		Direction,
		collisionParams,
		upgradeComponent->GetFireRateModifier(),
		upgradeComponent->GetWeaponDamageModifier(),
		upgradeComponent->GetHeadshotModifier()
	);
}

void AMainCharacter::OnFireWeaponOnce() {}

void AMainCharacter::OnHitTarget(AHumanoid* Target, FVector ImpactPoint, bool IsHeadshot) {
	if (AHumanoid* targetActor = Cast<AHumanoid>(Target)) {
		float finalDamage = Firearm->GetWeaponDamage() * upgradeComponent->GetWeaponDamageModifier();
		if (IsHeadshot) finalDamage *= (2 + upgradeComponent->GetHeadshotModifier());
		targetActor->TakeHitDamage(finalDamage, this);
		if (BloodShotFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodShotFX, ImpactPoint);
		if (!targetActor->CheckAlive()) {
			stats->IncrementKillCount();
			upgradeComponent->AddExpPoints(POINTS_PER_KILL);
		}
	}
}

void AMainCharacter::OnFireWeapon() {
	if (Firearm && IsAimMode && !IsSkillMenuOpen) {
		OnFireWeaponOnce();
		if (!Firearm->IsSemiAutomatic()) {
			GetWorld()->GetTimerManager().SetTimer(OnFireWeaponHandler, this, &AMainCharacter::OnFireWeaponOnce, Firearm->GetFireRate() * upgradeComponent->GetFireRateModifier(), true);
		}
	}
}

void AMainCharacter::OnFireStop() {
	if (Firearm) {
		GetWorld()->GetTimerManager().ClearTimer(OnFireWeaponHandler);
		if (IsAutoReload && Firearm->GetAmmoMagazine() <= 0) {
			OnReloadWeapon();
		} else if (Firearm->GetAmmoMagazine() <= 0 && EmptyMagSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptyMagSound, GetActorLocation(), Firearm->GetGunVolume());
		}
	}
}

void AMainCharacter::OnReloadWeapon() {
	float animationDelay = RELOAD_SPEED * Firearm->GetReloadSpeedModifier() * upgradeComponent->GetReloadSpeedModifier();
	Firearm->ReloadWeapon(animationDelay);
}

void AMainCharacter::SetCharacterIndex(int Index) {
	CharacterIndex = Index;
	HudWidget->SwitchToCharacterIndex(Index);
}

int AMainCharacter::GetCharacterIndex() {
	return CharacterIndex;
}

void AMainCharacter::OnInteract() {
	if (CanFillAmmo) {
		Firearm->RefillAmmo(RESERVE_AMMO * upgradeComponent->GetAdditionalReserveAmmoModifier());
	} else if (SelectableWeaponKey != FName()) {
		Firearm->ChangeWeapon(SelectableWeaponKey);
	}
}

void AMainCharacter::OnSprint() {
	if (Stamina <= (MAX_STAMINA * 0.4f) || IsSprinting) return;
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
		
		if (Stamina <= 0) {
			if (IsValid(NoStaminaVoiceComponent) && !NoStaminaVoiceComponent->IsPlaying()) {
				NoStaminaVoiceComponent->Play();
			} else {
				NoStaminaVoiceComponent = UGameplayStatics::SpawnSound2D(GetWorld(), NoStaminaVoice, 1.f, 1.f, 0.f, nullptr, false, false);
			}
		}
	}
}

void AMainCharacter::OnHealthRegen() {
	CurrentHealth = FMath::Min(CurrentHealth + upgradeComponent->GetRegenHP(), GetMaxHealth());
	Delegate_HealthChange.Broadcast(GetHealthPercentage());
}

void AMainCharacter::SetChangableWeapon(FName WeaponKey) {
	SelectableWeaponKey = WeaponKey;
}

int AMainCharacter::RefillAmmo(int AmmoAmount) {
	int refillAmount = FMath::Min(AmmoAmount, RESERVE_AMMO * upgradeComponent->GetAdditionalReserveAmmoModifier() - Firearm->GetReserveAmmo());
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

AFirearmWeapon* AMainCharacter::GetWeapon() {
	return Firearm;
}

void AMainCharacter::OnStartAim() {
	if (!IsAimMode && !IsSkillMenuOpen) {
		IsAimMode = true;
		if (IsSprinting) OnSprintStop();
		GetCameraBoom()->SetRelativeLocation(FVector(0, 50, 50));
		GetCameraBoom()->TargetArmLength -= CameraArmLengthOffset;
		bUseControllerRotationYaw = IsAimMode;
		if (CrosshairHudWidget) CrosshairHudWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainCharacter::TakeHitDamage(float damage, AActor* DamageCauser) {
	if (!IsAlive) return;
	Super::TakeHitDamage(damage, DamageCauser);
	if (FlinchMontage) PlayAnimMontage(FlinchMontage, 1.f);
	if (BloodHitFX && BloodSplatter == nullptr) {
		BloodSplatter = UNiagaraFunctionLibrary::SpawnSystemAttached(BloodHitFX, GetCapsuleComponent(), NAME_None, FVector(0.f, 0.f, 10.f), GetActorRotation(), EAttachLocation::Type::SnapToTarget, true);
		GetWorld()->GetTimerManager().SetTimer(BloodSplatterHandler, this, &AMainCharacter::OnRemoveBloodSplatter, 1.f, false);
	}
	if (CurrentHealth == 20.f && LowHealthVoice) UGameplayStatics::PlaySound2D(GetWorld(), LowHealthVoice);
	else if (HitVoice) UGameplayStatics::PlaySound2D(GetWorld(), HitVoice);
	Delegate_HealthChange.Broadcast(GetHealthPercentage());
}

void AMainCharacter::HealthPot(float HealAmount) {
	const float totalHealAmount = HealAmount + upgradeComponent->GetAdditionalHeal();
	CurrentHealth = FMath::Min(CurrentHealth + totalHealAmount, GetMaxHealth());
	if (HealthPickupVoice) UGameplayStatics::PlaySound2D(GetWorld(), HealthPickupVoice);
	Delegate_HealthChange.Broadcast(GetHealthPercentage());
}

void AMainCharacter::UpgradeSkill(FNecoSkills Skill) {
	GetSkillUpgrades()->AddSkillPoint(Skill);
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

void AMainCharacter::OnRemoveBloodSplatter() {
	if (BloodSplatter) {
		BloodSplatter->DestroyComponent();
		BloodSplatter = nullptr;
	}
}

void AMainCharacter::OnScrollAxis(const FInputActionValue& Value) {
	CameraBoom->TargetArmLength = FMath::Min(400, FMath::Max(50, CameraBoom->TargetArmLength - (Value.Get<float>() * CAMERA_SCROLL_SPEED)));
	OnCameraDistanceChanged();
}

float AMainCharacter::GetCameraDistance() {
	return CameraBoom->TargetArmLength;
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