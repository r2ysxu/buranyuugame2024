// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "Kismet/GameplayStatics.h"
#include "NecoGoblin/NecoGoblinGameMode.h"


void AGoblin::BeginPlay() {
	Super::BeginPlay();
	GameMode = Cast<ANecoGoblinGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool AGoblin::TakeHitDamage(float damage, AActor* DamageCauser) {
	const bool isAlive = Super::TakeHitDamage(damage, DamageCauser);
	if (!isAlive) {
		GameMode->DecrementGoblin();
	}
	return isAlive;
}

void AGoblin::OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}

void AGoblin::OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}