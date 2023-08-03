// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Humanoid.generated.h"

UCLASS(Abstract)
class NECOGOBLIN_API AHumanoid : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanoid();

protected:
	FName HeadSocketName = FName("head");
	class USphereComponent* HeadBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool UseDeathAnimation;

	FTimerHandle OnDeadHandler;
	const float MAX_HEALTH = 100.f;
	const float DECOMPOSE_DELAY = 20.f;
	float CurrentHealth = 100.f;
	bool IsAlive = true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnMeleeHit(AActor* actor, float modifier);

public:
	
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsAlive() { return IsAlive; }
	virtual void OnDecompose();
	virtual void TakeHitDamage(float damage, AActor* DamageCauser);
	virtual uint8 GetTeam() { return 0; }
	virtual void OnHeadHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
	virtual void OnBodyHit(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
};
