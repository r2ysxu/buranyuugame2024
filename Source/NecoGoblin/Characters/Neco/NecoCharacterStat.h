// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NecoCharacterStat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECOGOBLIN_API UNecoCharacterStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNecoCharacterStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int KillCount = 0;

public:	

	int IncrementKillCount();
	UFUNCTION(BlueprintCallable) FORCEINLINE int GetKillCount() { return KillCount; }
};
