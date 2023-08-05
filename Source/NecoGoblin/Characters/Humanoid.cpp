// Fill out your copyright notice in the Description page of Project Settings.


#include "Humanoid.h"
#include "../Weapons/Melee/MeleeWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AHumanoid::AHumanoid() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HeadBox = CreateDefaultSubobject<USphereComponent>(TEXT("HeadBox"));
	HeadBox->SetSphereRadius(20.f);
	HeadBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HeadSocketName);
}

// Called when the game starts or when spawned
void AHumanoid::BeginPlay() {
	Super::BeginPlay();	
}

void AHumanoid::OnMeleeHit(AActor* actor, float modifier) {
	AMeleeWeapon* weaponActor = Cast<AMeleeWeapon>(actor);
	if (weaponActor && weaponActor->GetWeaponTeam() != GetTeam() && weaponActor->GetIsMeleeAttacking()) {
		if (weaponActor->GetLastHitCharacter() != this) {
			weaponActor->SetLastHitCharacter(this);
			TakeHitDamage(weaponActor->GetWeaponDamage() * modifier, actor);
		}
	}
}

void AHumanoid::HealToFull() {
	CurrentHealth = MaxHealth;
}

void AHumanoid::IncreaseMaxHealth(float additionalHealth) {
	MaxHealth += additionalHealth;
	CurrentHealth += additionalHealth;
}

void AHumanoid::OnDecompose() {
	GetWorld()->GetTimerManager().ClearTimer(OnDeadHandler);
	Destroy();
}

bool AHumanoid::TakeHitDamage(float damage, AActor* DamageCauser) {
	CurrentHealth -= damage;
	if (CurrentHealth <= 0) {
		IsAlive = false;
		if (!UseDeathAnimation) {
			GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
			GetMesh()->SetSimulatePhysics(true);
		}
		GetCapsuleComponent()->DestroyComponent();
		GetWorld()->GetTimerManager().SetTimer(OnDeadHandler, this, &AHumanoid::OnDecompose, DECOMPOSE_DELAY, false);
	}
	return IsAlive;
}