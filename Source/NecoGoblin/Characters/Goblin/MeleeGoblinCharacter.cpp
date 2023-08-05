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
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	// GetCharacterMovement()->UseAccelerationForPathFollowing() => true

	MeleeDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeDetectionSphere"));
	MeleeDetectionBox->SetBoxExtent(FVector(30.f, 30.f, 30.f));
	MeleeDetectionBox->AddLocalOffset(FVector(50.f, 0.f, 0.f));
	MeleeDetectionBox->SetupAttachment(GetRootComponent());
	//MeleeDetectionBox->bHiddenInGame = false;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeGoblinCharacter::SetupMeleeWeapon() {
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
	SetupMeleeWeapon();
	MeleeDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeGoblinCharacter::OnWithinMeleeAttackRange);
	MeleeDetectionBox->OnComponentEndOverlap.AddDynamic(this, &AMeleeGoblinCharacter::OnOutsideMeleeAttackRange);
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

void AMeleeGoblinCharacter::InitiateMeleeAttack() {
	if (!GetIsAlive()) {
		GetAIController()->SetIsAttacking(false);
		GetWorld()->GetTimerManager().ClearTimer(InitateAttackHandler);
	} else if (MeleeAttackMontage && !GetAIController()->GetIsAttacking()) {
		GetAIController()->SetIsAttacking(true);
		if (MeleeWeapon) MeleeWeapon->SetIsMeleeAttacking(true);
		float animationDelay = PlayAnimMontage(MeleeAttackMontage);
		GetWorld()->GetTimerManager().SetTimer(OnMeleeAttackHandler, this, &AMeleeGoblinCharacter::OnAttackStop, animationDelay, false);
	}
}

void AMeleeGoblinCharacter::OnAttackStop() {
	GetAIController()->SetIsAttacking(false);
	if (MeleeWeapon) MeleeWeapon->SetIsMeleeAttacking(false);
}

void AMeleeGoblinCharacter::TakeHitDamage(float damage, AActor* DamageCauser) {
	Super::TakeHitDamage(damage, DamageCauser);
}

bool AMeleeGoblinCharacter::CheckAlive() {
	if (Super::CheckAlive()) {
		if (MeleeAttackMontage) StopAnimMontage(MeleeAttackMontage);
		if (MeleeWeapon) MeleeWeapon->SetIsMeleeAttacking(false);
		GetAIController()->SetIsAttacking(false);
		return true;
	}
	return false;
}

void AMeleeGoblinCharacter::OnDecompose() {
	Super::OnDecompose();
	MeleeWeapon->Destroy();
}
