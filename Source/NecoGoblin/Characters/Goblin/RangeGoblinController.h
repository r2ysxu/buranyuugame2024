// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RangeGoblinController.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API ARangeGoblinController : public AAIController {
	GENERATED_BODY()

private:
	class ARangeGoblinCharacter* PossessedPawn;
	const FName MainPlayer = FName("MainPlayer");
	class ANecoSpirit* GetClosestNecoSpiritByTag(const UObject* WorldContextObject, FName tagName);

protected:
	FTimerHandle MoveHandler;
	class ANecoSpirit* CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float MinimumTargetDistance = 2000.f;
	
public:
	ARangeGoblinController();
	virtual void OnPossess(APawn* InPawn) override;

	FORCEINLINE class ANecoSpirit* GetCurrentTarget() { return CurrentTarget; }

	void OnMoveToTarget();
};
