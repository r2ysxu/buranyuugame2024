// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Components/SphereComponent.h"

void AHealthPickup::BeginPlay() {
	Super::BeginPlay();

	PickupRangeBox->OnComponentBeginOverlap.AddDynamic(this, &APickupable::OnWithinPickupRange);
}

void AHealthPickup::OnPickup(ANecoSpirit* actor) {
	if (IsValid(actor)) {
		actor->HealToFull();
		Destroy();
	}
}
