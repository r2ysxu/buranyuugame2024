// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/Goblin/Goblin.h"
#include "GoblinSpawner.generated.h"

UCLASS()
class NECOGOBLIN_API AGoblinSpawner : public AActor
{
	GENERATED_BODY()

private:
	class ANecoGoblinGameMode* GameMode;

	bool SpawnEnemyType(uint8 Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle OnSpawnHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AMeleeGoblinCharacter> MeleeGoblinClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ARangeGoblinCharacter> RangeGoblinClass;
	
public:	
	// Sets default values for this actor's properties
	AGoblinSpawner();

	void SpawnEnemy();
};
