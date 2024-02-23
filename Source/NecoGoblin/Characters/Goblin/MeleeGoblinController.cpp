// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeGoblinController.h"
#include "Goblin.h"
#include "MeleeGoblinCharacter.h"
#include "../Humanoid.h"
#include "../Neco/NecoSpirit.h"

#include "Components/SplineComponent.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h>
#include <Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	PossessedPawn = Cast<AMeleeGoblinCharacter>(InPawn);
	CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
	Theta = FMath::RandRange(-45.f, 45.f);
	GetWorld()->GetTimerManager().SetTimer(MoveHandler, this, &AMeleeGoblinController::OnMoveToTarget, 0.1f, true);
}

void AMeleeGoblinController::OnMoveToTarget() {
	if (PossessedPawn->GetIsAttacking()) {
	} else if (CurrentTarget && CurrentTarget->GetIsAlive()) {
		PossessedPawn->LookAtTarget(FindTargetHeadRotation());
		float distance = CurrentTarget->GetDistanceTo(PossessedPawn);
		if (distance < TargetHomingRadius * 2 && distance > TargetHomingRadius) {
			FVector actorLocation = CurrentTarget->GetActorLocation();
			float thetaByDistance = Theta * (distance / (TargetHomingRadius * 2));
			FVector offset = FVector(TargetHomingRadius * FMath::Cos(thetaByDistance), TargetHomingRadius * FMath::Sin(thetaByDistance), 0.f);
			MoveToLocation(actorLocation - offset);
		} else MoveToActor(CurrentTarget);
	} else {
		CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
	}
}

FRotator AMeleeGoblinController::FindTargetHeadRotation() {
	if (CurrentTarget == nullptr) return FRotator();
	FVector headLocation = GetCharacter()->GetMesh()->GetSocketLocation(FName("head"));
	return UKismetMathLibrary::FindLookAtRotation(headLocation, CurrentTarget->GetActorLocation());
}