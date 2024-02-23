// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/Goblin/Goblin.h"
#include "GoblinSpawner.generated.h"

UENUM(BlueprintType)
enum class ESpawnEnemyType : uint8 {
	VE_Melee,
	VE_Range,
};

UCLASS()
class NECOGOBLIN_API AGoblinSpawner : public AActor {
	GENERATED_BODY()

private:
	class ANecoGoblinGameMode* GameMode;

protected:
	float MovementSpeedModifier = 1.f;
	float SpawnRate = 5.f;
	FTimerHandle OnSpawnHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AMeleeGoblinCharacter> MeleeGoblinClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ARangeGoblinCharacter> RangeGoblinClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	AGoblinSpawner();

	void SpawnEnemy();
	bool SpawnEnemyType(uint8 Type);
	void ChangeSpawnInfo(float EnemySpawnRate, float EnemyMovementSpeedMod);
};
