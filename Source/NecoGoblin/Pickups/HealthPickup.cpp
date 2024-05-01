// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Components/SphereComponent.h"

void AHealthPickup::BeginPlay() {
	Super::BeginPlay();

	PickupRangeBox->OnComponentBeginOverlap.AddDynamic(this, &APickupable::OnWithinPickupRange);
	GetWorld()->GetTimerManager().SetTimer(OnDecomposeHandler, this, &AHealthPickup::OnDecompose, DECOMPOSE_DELAY, false);
}

void AHealthPickup::OnPickup(AAllyBase* actor) {
	if (IsValid(actor)) {
		actor->HealthPot(60);
		Destroy();
	}
}

void AHealthPickup::OnDecompose() {
	Destroy();
}
