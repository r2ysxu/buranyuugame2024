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

void ARangeGoblinCharacter::SetRunSpeed(float MovementSpeedModifier) {
	GetCharacterMovement()->MaxWalkSpeed = FMath::Min(350.f + MovementSpeedModifier, 600.f);
}

bool ARangeGoblinCharacter::CheckRangeAttack(ANecoSpirit* TargetCharacter, FVector& OutTossVelocity) {
	if (!IsValid(Weapon) || IsAttackCooldown) return false;
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetTargetLocation(), TargetCharacter->GetActorLocation());
	SetActorRotation(FRotator(GetActorRotation().Pitch, rotation.Yaw, GetActorRotation().Roll));

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

void ARangeGoblinCharacter::InitiateRangeAttack(FVector& OutTossVelocity) {
	if (RangeAttackMontage && IsAlive && IsValid(Weapon) && !IsAttackCooldown) {
		float animationDelay = PlayAnimMontage(RangeAttackMontage, 1.f) + 0.2f;
		GetWorld()->GetTimerManager().SetTimer(AttackResetHandler, this, &ARangeGoblinCharacter::OnAttackReset, animationDelay, false);
		GetAIController()->SetIsAttacking(true);
		Weapon->OnFire(OutTossVelocity, animationDelay - 0.5f);
		Weapon = nullptr;
		IsAttackCooldown = true;
	}
}

void ARangeGoblinCharacter::TrackTargetStopMovement(ANecoSpirit* TargetCharacter) {
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetTargetLocation(), TargetCharacter->GetActorLocation());
	SetActorRotation(FRotator(GetActorRotation().Pitch, rotation.Yaw, GetActorRotation().Roll));
}

void ARangeGoblinCharacter::OnAttackReset() {
	GetAIController()->SetIsAttacking(false);
	SetupWeapon();
	GetWorld()->GetTimerManager().SetTimer(AttackResetHandler, this, &ARangeGoblinCharacter::OnAttackCooldownReset, AttackCooldownRate, false);
}

void ARangeGoblinCharacter::OnAttackCooldownReset() {
	IsAttackCooldown = false;
}

bool ARangeGoblinCharacter::CheckAlive() {
	if (Super::CheckAlive()) {
		if (RangeAttackMontage) StopAnimMontage(RangeAttackMontage);
		GetAIController()->SetIsAttacking(false);
		return true;
	}
	return false;
}

void ARangeGoblinCharacter::OnDecompose() {
	Super::OnDecompose();
	Weapon->Destroy();
}
