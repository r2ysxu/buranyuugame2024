// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeGoblinCharacter.h"
#include "../../Weapons/Ranged/ThrownWeapon.h"
#include "RangeGoblinController.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void ARangeGoblinCharacter::BeginPlay() {
	Super::BeginPlay();
	SpawnDefaultController();
	SetupWeapon();
}

void ARangeGoblinCharacter::SetupWeapon() {
	FTransform weaponTransform;
	weaponTransform.SetLocation(FVector::ZeroVector);
	weaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	Weapon = GetWorld()->SpawnActor<AThrownWeapon>(RangeWeaponClass, weaponTransform);
	if (Weapon) {
		Weapon->AttachToWielder(this, WeaponSocketName);
	}
}

ARangeGoblinCharacter::ARangeGoblinCharacter() {
	PrimaryActorTick.bCanEverTick = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	// GetCharacterMovement()->UseAccelerationForPathFollowing() => true

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARangeGoblinCharacter::LookAtTarget(FVector TargetLocation) {
	if (HasAuthority()) {
		Multicast_LookAtTarget(TargetLocation);
	}
	LookAtTarget_Implementation(TargetLocation);
}

void ARangeGoblinCharacter::LookAtTarget_Implementation(FVector TargetLocation) {
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetTargetLocation(), TargetLocation);
	SetActorRotation(FRotator(GetActorRotation().Pitch, rotation.Yaw, GetActorRotation().Roll));
}

void ARangeGoblinCharacter::Multicast_LookAtTarget_Implementation(FVector TargetLocation) {
	LookAtTarget_Implementation(TargetLocation);
}

void ARangeGoblinCharacter::SetRunSpeed(float MovementSpeedModifier) {
	GetCharacterMovement()->MaxWalkSpeed = FMath::Min(350.f + MovementSpeedModifier, 600.f);
}

bool ARangeGoblinCharacter::CheckRangeAttack(ANecoSpirit* TargetCharacter, FVector& OutTossVelocity) {
	if (!IsValid(Weapon) || IsAttackCooldown) return false;

	TArray<AActor*> ignores = { this, TargetCharacter };
	return UGameplayStatics::SuggestProjectileVelocity(
		GetWorld(),
		OutTossVelocity,
		Weapon->GetActorLocation(),
		// Ignore projectile speed in calculation to make it less accurate
		TargetCharacter->GetActorLocation() + TargetCharacter->GetVelocity(),
		TossVelocity,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::TraceFullPath,
		FCollisionResponseParams::DefaultResponseParam,
		ignores
	);
}

void ARangeGoblinCharacter::InitiateRangeAttack_Implementation(FVector OutTossVelocity) {
	if (RangeAttackMontage && IsAlive && IsValid(Weapon) && !IsAttackCooldown) {
		float animationDelay = PlayAnimMontage(RangeAttackMontage, 1.f) + 0.2f;
		GetWorld()->GetTimerManager().SetTimer(AttackResetHandler, this, &ARangeGoblinCharacter::OnAttackReset, animationDelay, false);
		SetIsAttacking(true);
		Weapon->OnFire(OutTossVelocity, animationDelay - 0.5f);
		Weapon = nullptr;
		IsAttackCooldown = true;
	}
}

void ARangeGoblinCharacter::InitiateRangeAttack(FVector OutTossVelocity) {
	if (HasAuthority()) {
		Multicast_InitiateRangeAttack(OutTossVelocity);
	}
	InitiateRangeAttack_Implementation(OutTossVelocity);
}

void ARangeGoblinCharacter::Multicast_InitiateRangeAttack_Implementation(FVector OutTossVelocity) {
	InitiateRangeAttack_Implementation(OutTossVelocity);
}

void ARangeGoblinCharacter::TrackTargetStopMovement(ANecoSpirit* TargetCharacter) {
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetTargetLocation(), TargetCharacter->GetActorLocation());
	SetActorRotation(FRotator(GetActorRotation().Pitch, rotation.Yaw, GetActorRotation().Roll));
}

void ARangeGoblinCharacter::OnAttackReset() {
	SetIsAttacking(false);
	SetupWeapon();
	GetWorld()->GetTimerManager().SetTimer(AttackResetHandler, this, &ARangeGoblinCharacter::OnAttackCooldownReset, AttackCooldownRate, false);
}

void ARangeGoblinCharacter::OnAttackCooldownReset() {
	IsAttackCooldown = false;
}

bool ARangeGoblinCharacter::CheckAlive() {
	if (Super::CheckAlive()) {
		if (RangeAttackMontage) StopAnimMontage(RangeAttackMontage);
		SetIsAttacking(false);
		return true;
	}
	return false;
}

void ARangeGoblinCharacter::OnDecompose() {
	Super::OnDecompose();
	Weapon->Destroy();
}
