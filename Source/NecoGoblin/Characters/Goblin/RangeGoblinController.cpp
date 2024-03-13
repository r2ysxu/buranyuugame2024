// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeGoblinController.h"
#include "../Neco/NecoSpirit.h"
#include "RangeGoblinCharacter.h"

#include "Kismet/GameplayStatics.h"

ANecoSpirit* ARangeGoblinController::GetClosestNecoSpiritByTag(const UObject* WorldContextObject, FName tagName) {
	TArray<AActor*> mainPlayers;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, tagName, mainPlayers);
	if (mainPlayers.IsEmpty()) return nullptr;

	int minIndex = 0;
	float minDistance = FLT_MAX;
	for (int i = 0; i < mainPlayers.Num(); i++) {
		float distance = PossessedPawn->GetDistanceTo(mainPlayers[i]);
		if (minDistance > distance) {
			minIndex = i;
			minDistance = distance;
		}
	}
	return Cast<ANecoSpirit>(mainPlayers[minIndex]);
}

ARangeGoblinController::ARangeGoblinController() {
	CurrentTarget = nullptr;
}

void ARangeGoblinController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	PossessedPawn = Cast<ARangeGoblinCharacter>(InPawn);
	GetWorld()->GetTimerManager().SetTimer(MoveHandler, this, &ARangeGoblinController::OnMoveToTarget, 0.1f, true);
}

void ARangeGoblinController::OnMoveToTarget() {
	CurrentTarget = GetClosestNecoSpiritByTag(GetWorld(), MainPlayer);
	if (PossessedPawn->GetIsAttacking()) {
	} else if (CurrentTarget && CurrentTarget->GetIsAlive()) {
		FVector tossVelocity;
		PossessedPawn->LookAtTarget(CurrentTarget->GetActorLocation());
		if (PossessedPawn->CheckRangeAttack(CurrentTarget, tossVelocity)) {
			PossessedPawn->InitiateRangeAttack(tossVelocity);
		} else if (GetPawn()->GetDistanceTo(CurrentTarget) > MinimumTargetDistance) {
			MoveToActor(CurrentTarget);
		} else {
			PossessedPawn->TrackTargetStopMovement(CurrentTarget);
			StopMovement();
		}
	}
}
