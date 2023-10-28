// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeGoblinController.h"
#include "MeleeGoblinCharacter.h"
#include "../Humanoid.h"
#include "../Neco/NecoSpirit.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h>
#include <Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h>
#include "Kismet/GameplayStatics.h"

ANecoSpirit* AMeleeGoblinController::GetNecoSpiritByTag(const UObject* WorldContextObject, FName tagName) {
	TArray<AActor*> mainPlayers;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, tagName, mainPlayers);
	if (mainPlayers.Num() > 0) {
		return Cast<ANecoSpirit>(mainPlayers[0]);
	}
	return nullptr;
}

AMeleeGoblinController::AMeleeGoblinController() {
	CurrentTarget = nullptr;
}

void AMeleeGoblinController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	Cast<AMeleeGoblinCharacter>(InPawn)->SetAIController(this);
	CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
	GetWorld()->GetTimerManager().SetTimer(MoveHandler, this, &AMeleeGoblinController::OnMoveToTarget, 0.1f, true);
}

void AMeleeGoblinController::OnMoveToTarget() {
	if (GetIsAttacking()) {
	} else if (CurrentTarget && CurrentTarget->GetIsAlive()) {
		MoveToActor(CurrentTarget);
	} else {
		CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
	}
}
