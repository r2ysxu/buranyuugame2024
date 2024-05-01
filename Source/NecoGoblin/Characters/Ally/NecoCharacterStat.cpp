// Fill out your copyright notice in the Description page of Project Settings.


#include "NecoCharacterStat.h"

// Sets default values for this component's properties
UNecoCharacterStat::UNecoCharacterStat() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UNecoCharacterStat::BeginPlay() {
	Super::BeginPlay();
}

int UNecoCharacterStat::IncrementKillCount() {
	return ++KillCount;
}