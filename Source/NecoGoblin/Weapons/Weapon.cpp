// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../Characters/Humanoid.h"

// Sets default values
AWeapon::AWeapon() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay() {
	Super::BeginPlay();
	
}

void AWeapon::AttachToWielder(AHumanoid* WieldCharacter, FName SocketName) {
	Wielder = WieldCharacter;
	EquipWeapon(SocketName);
}