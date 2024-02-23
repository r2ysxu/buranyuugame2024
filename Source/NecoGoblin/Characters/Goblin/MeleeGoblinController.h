// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Neco/NecoSpirit.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h>
#include "MeleeGoblinController.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API AMeleeGoblinController : public AAIController {
	GENERATED_BODY()

private:
	const FName MainPlayer = FName("MainPlayer");
	ANecoSpirit* GetNecoSpiritByTag(const UObject* WorldContextObject, FName tagName);
	float TargetHomingRadius = 500.f;
	float Theta = 0.f;

protected:
	FTimerHandle MoveHandler;
	ANecoSpirit* CurrentTarget;
	class AMeleeGoblinCharacter* PossessedPawn;

public:
	AMeleeGoblinController();
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable) FORCEINLINE FRotator FindTargetHeadRotation();

	void OnMoveToTarget();
};
