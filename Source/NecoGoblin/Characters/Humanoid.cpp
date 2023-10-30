// Fill out your copyright notice in the Description page of Project Settings.


#include "Humanoid.h"
#include "../Weapons/Melee/MeleeWeapon.h"

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHumanoid::AHumanoid() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HeadBox = CreateDefaultSubobject<USphereComponent>(TEXT("HeadBox"));
	HeadBox->SetupAttachment(GetMesh(), HeadSocketName);
	HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
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
			CheckAlive();
		}
	}
}

void AHumanoid::IncreaseMaxHealth(float additionalHealth) {
	MaxHealth += additionalHealth;
	CurrentHealth += additionalHealth;
}

void AHumanoid::OnDecompose() {
	GetWorld()->GetTimerManager().ClearTimer(OnDeadHandler);
	Destroy();
}

void AHumanoid::TakeHitDamage(float damage, AActor* DamageCauser) {
	CurrentHealth -= damage;
}

bool AHumanoid::CheckAlive() {
	if (CurrentHealth <= 0) {
		IsAlive = false;
		if (!UseDeathAnimation) {
			HeadBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
			GetMesh()->SetSimulatePhysics(true);
		}
		GetMovementComponent()->Deactivate();
		GetWorld()->GetTimerManager().SetTimer(OnDeadHandler, this, &AHumanoid::OnDecompose, DECOMPOSE_DELAY, false);
	}
	return IsAlive;
}
