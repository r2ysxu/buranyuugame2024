// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon() {
	PrimaryActorTick.bCanEverTick = false;

	class USceneComponent* sceneRoot = CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot");
	SetRootComponent(sceneRoot);

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("OneHandWeaponMesh");
	WeaponMeshComponent->SetupAttachment(GetRootComponent());
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeleeAttackWeaponBox = CreateDefaultSubobject<UBoxComponent>("OneHandWeaponAttackBox");
	MeleeAttackWeaponBox->SetBoxExtent(FVector(10.f, 50.f, 0));
	MeleeAttackWeaponBox->AddLocalOffset(FVector(10.f, 0, 0));
	MeleeAttackWeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeleeAttackWeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	MeleeAttackWeaponBox->SetupAttachment(GetRootComponent());
	//MeleeAttackWeaponBox->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) WeaponMeshComponent->SetStaticMesh(Mesh);
	WeaponMeshComponent->AddLocalOffset(FVector(0.f, 50.f, 0));
	WeaponMeshComponent->AddLocalRotation(FRotator(180.f, 90.f, 90.f));
}

void AMeleeWeapon::EquipWeapon(FName SocketName) {
	AttachToComponent(Wielder->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

float AMeleeWeapon::GetWeaponDamage() {
	return 10.f;
}

uint8 AMeleeWeapon::GetWeaponTeam() {
	return Wielder->GetTeam();
}

void AMeleeWeapon::SetIsMeleeAttacking(bool IsMeleeAttacking) {
	MeleeAttacking = IsMeleeAttacking;
	if (!IsMeleeAttacking) {
		LastHitCharacter = nullptr;
	}
}