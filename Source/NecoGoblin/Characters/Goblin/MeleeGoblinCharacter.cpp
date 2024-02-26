// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeGoblinCharacter.h"
#include "Components/BoxComponent.h"
#include "../../Weapons/Melee/MeleeWeapon.h"
#include "../Humanoid.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMeleeGoblinCharacter::AMeleeGoblinCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	// GetCharacterMovement()->UseAccelerationForPathFollowing() => true

	MeleeDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeDetectionSphere"));
	MeleeDetectionBox->SetBoxExtent(FVector(30.f, 30.f, 30.f));
	MeleeDetectionBox->AddLocalOffset(FVector(50.f, 0.f, 0.f));
	MeleeDetectionBox->SetupAttachment(GetRootComponent());
	MeleeDetectionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//MeleeDetectionBox->bHiddenInGame = false;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeGoblinCharacter::SetupWeapon() {
	FTransform weaponTransform;
	weaponTransform.SetLocation(FVector::ZeroVector);
	weaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(MeleeWeaponClass, weaponTransform);
	if (MeleeWeapon) {
		MeleeWeapon->AttachToWielder(this, WeaponSocketName);
	}
}

// Called when the game starts or when spawned
void AMeleeGoblinCharacter::BeginPlay() {
	Super::BeginPlay();
	SpawnDefaultController();
	SetupWeapon();
	MeleeDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeGoblinCharacter::OnWithinMeleeAttackRange);
	MeleeDetectionBox->OnComponentEndOverlap.AddDynamic(this, &AMeleeGoblinCharacter::OnOutsideMeleeAttackRange);
}

void AMeleeGoblinCharacter::LookAtTarget(FRotator Rotation) {
	if (HasAuthority()) {
		Multicast_LookAtTarget(Rotation);
	}
	LookAtTarget_Implementation(Rotation);
}

void AMeleeGoblinCharacter::LookAtTarget_Implementation(FRotator Rotation) {
	HeadRotation = FRotator(
		0.f, // Roll
		Rotation.Yaw - GetActorRotation().Yaw,
		GetActorRotation().Pitch - Rotation.Pitch + 90.f // Pitch
	);
}

void AMeleeGoblinCharacter::Multicast_LookAtTarget_Implementation(FRotator Rotation) {
	LookAtTarget_Implementation(Rotation);
}

void AMeleeGoblinCharacter::OnWithinMeleeAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (actor == this || !GetIsAlive()) return;
	AHumanoid* target = Cast<AHumanoid>(actor);
	if (target && target->GetIsAlive() && target->GetTeam() != GetTeam()) {
		GetWorld()->GetTimerManager().SetTimer(InitateAttackHandler, this, &AMeleeGoblinCharacter::InitiateMeleeAttack, AttackDelay, true, 0.f);
	}
}

void AMeleeGoblinCharacter::OnOutsideMeleeAttackRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	if (actor == this || !GetIsAlive()) return;
	AHumanoid* target = Cast<AHumanoid>(actor);
	if (target && target->GetTeam() != GetTeam()) {
		GetWorld()->GetTimerManager().ClearTimer(InitateAttackHandler);
	}
}

void AMeleeGoblinCharacter::SetRunSpeed(float MovementSpeedModifier) {
	GetCharacterMovement()->MaxWalkSpeed = FMath::Min(350.f + MovementSpeedModifier, 600.f);
}

void AMeleeGoblinCharacter::InitiateMeleeAttack() {
	if (!GetIsAlive()) {
		SetIsAttacking(false);
		GetWorld()->GetTimerManager().ClearTimer(InitateAttackHandler);
	} else if (MeleeAttackMontage && !GetIsAttacking()) {
		SetIsAttacking(true);
		if (MeleeWeapon) MeleeWeapon->SetIsMeleeAttacking(true);
		float animationDelay = PlayAnimMontage(MeleeAttackMontage);
		GetWorld()->GetTimerManager().SetTimer(OnMeleeAttackHandler, this, &AMeleeGoblinCharacter::OnAttackStop, animationDelay, false);
	}
}

void AMeleeGoblinCharacter::OnAttackStop() {
	SetIsAttacking(false);
	if (MeleeWeapon) MeleeWeapon->SetIsMeleeAttacking(false);
}

void AMeleeGoblinCharacter::TakeHitDamage(float damage, AActor* DamageCauser) {
	Super::TakeHitDamage(damage, DamageCauser);
}

bool AMeleeGoblinCharacter::CheckAlive() {
	if (Super::CheckAlive()) {
		if (MeleeAttackMontage) StopAnimMontage(MeleeAttackMontage);
		if (MeleeWeapon) MeleeWeapon->SetIsMeleeAttacking(false);
		SetIsAttacking(false);
		return true;
	}
	return false;
}

void AMeleeGoblinCharacter::OnDecompose() {
	Super::OnDecompose();
	MeleeWeapon->Destroy();
}
