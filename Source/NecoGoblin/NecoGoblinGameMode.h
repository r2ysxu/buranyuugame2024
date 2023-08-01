// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NecoGoblinGameMode.generated.h"

UCLASS(minimalapi)
class ANecoGoblinGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FTimerHandle NextRoundHandler;

	int CurrentRound = 0;
	int GoblinPerRound;
	int GoblinCount;
	int GoblinKilled = 0;
	float GoblinSpawnRate = 5.f;

	void NextRound();

public:
	ANecoGoblinGameMode();

	void StartPlay() override;
	
	bool IncrementGoblin();
	bool DecrementGoblin();
	FORCEINLINE int GetGoblinCount() { return GoblinCount; }
	FORCEINLINE int GetGoblinPerRound() { return GoblinPerRound; }
	FORCEINLINE float GetGoblinSpawnRate() { return GoblinSpawnRate; }
};



