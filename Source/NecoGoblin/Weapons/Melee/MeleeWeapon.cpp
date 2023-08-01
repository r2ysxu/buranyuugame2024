// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	class USceneComponent* sceneRoot = CreateDefaultSubobject<USceneComponent>("OneHandWeaponRoot");
	SetRootComponent(sceneRoot);

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("OneHandWeaponMesh");
	WeaponMeshComponent->SetupAttachment(GetRootComponent());
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeleeAttackWeaponBox = CreateDefaultSubobject<UBoxComponent>("OneHandWeaponAttackBox");
	MeleeAttackWeaponBox->SetBoxExtent(FVector(50.f, 50.f, 0));
	MeleeAttackWeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeleeAttackWeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	MeleeAttackWeaponBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	//MeleeAttackWeaponBox->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay() {
	Super::BeginPlay();
	if (Mesh) WeaponMeshComponent->SetStaticMesh(Mesh);
	WeaponMeshComponent->AddLocalOffset(FVector(7.f, 2.f, 0));
	WeaponMeshComponent->AddLocalRotation(FRotator(0.f, 180.f, 180.f));
	MeleeAttackWeaponBox->AddLocalRotation(FRotator(0.f, 90.f, 90.f));
	//MeleeAttackWeaponBox->AddLocalOffset(FVector(0, 50.f, 0));
}

void AMeleeWeapon::EquipWeapon(FName SocketName) {
	AttachToComponent(Wielder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
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