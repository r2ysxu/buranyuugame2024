// Fill out your copyright notice in the Description page of Project Settings.


#include "../Neco/NecoSpirit.h"
#include "RangeGoblinController.h"
#include "RangeGoblinCharacter.h"

#include "Kismet/GameplayStatics.h"

ANecoSpirit* ARangeGoblinController::GetNecoSpiritByTag(const UObject* WorldContextObject, FName tagName) {
	TArray<AActor*> mainPlayers;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, tagName, mainPlayers);
	if (mainPlayers.Num() > 0) {
		return Cast<ANecoSpirit>(mainPlayers[0]);
	}
	return nullptr;
}

ARangeGoblinController::ARangeGoblinController() {
	CurrentTarget = nullptr;
}

void ARangeGoblinController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
	PossessedPawn = Cast<ARangeGoblinCharacter>(InPawn);
	Cast<ARangeGoblinCharacter>(InPawn)->SetAIController(this);
	GetWorld()->GetTimerManager().SetTimer(MoveHandler, this, &ARangeGoblinController::OnMoveToTarget, 0.1f, true);
}

void ARangeGoblinController::OnMoveToTarget() {
	if (GetIsAttacking()) {
	} else if (CurrentTarget && CurrentTarget->GetIsAlive()) {
		FVector tossVelocity;
		if (PossessedPawn->CheckRangeAttack(CurrentTarget, tossVelocity)) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Checked Range Passed"));
			PossessedPawn->InitiateRangeAttack(tossVelocity);
		} else if (GetPawn()->GetDistanceTo(CurrentTarget) > MinimumTargetDistance) {
			MoveToActor(CurrentTarget);
		} else {
			StopMovement();
		}
	} else {
		CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
	}
}
