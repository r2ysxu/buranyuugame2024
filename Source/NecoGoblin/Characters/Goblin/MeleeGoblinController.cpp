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

const FName MainPlayer = FName("MainPlayer");

ANecoSpirit* GetNecoSpiritByTag(const UObject* WorldContextObject, FName tagName) {
	TArray<AActor*> mainPlayers;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, tagName, mainPlayers);
	if (mainPlayers.Num() > 0) {
		return Cast<ANecoSpirit>(mainPlayers[0]);
	}
	return nullptr;
}

AMeleeGoblinController::AMeleeGoblinController() {
	InitializeAIPerception();
	CurrentTarget = nullptr;
}

void AMeleeGoblinController::InitializeAIPerception() {
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("MeleeEnemySightConfig"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.f;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("MeleeEnemyPerceptionComponent")));
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMeleeGoblinController::PawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	if (GetPerceptionComponent() == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Perception Component Null"));
	}
}

void AMeleeGoblinController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(MoveHandler, this, &AMeleeGoblinController::OnMoveToTarget, 0.1f, true);
}


void AMeleeGoblinController::PawnDetected(const TArray<AActor*>& DetectedPawn) {
	for (int i = 0; i < DetectedPawn.Num(); i++) {
		if (DetectedPawn[i] != nullptr) {
			auto const detectedCharacter = Cast<ANecoSpirit>(DetectedPawn[i]);
			if (detectedCharacter == CurrentTarget) return;
			else if (detectedCharacter && detectedCharacter->GetIsAlive()) {
				float distanceToCurrent = GetPawn()->GetDistanceTo(Cast<AActor>(CurrentTarget));
				float distanceToDetected = GetPawn()->GetDistanceTo(DetectedPawn[i]);
				if (distanceToDetected < distanceToCurrent) {
					CurrentTarget = detectedCharacter;
				}
			} else {
				CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
			}
		}
	}
}

void AMeleeGoblinController::OnMoveToTarget() {
	if (GetIsAttacking()) {
	} else if (CurrentTarget && CurrentTarget->GetIsAlive()) {
		MoveToActor(Cast<ACharacter>(CurrentTarget));
	} else {
		CurrentTarget = GetNecoSpiritByTag(GetWorld(), MainPlayer);
	}
}
