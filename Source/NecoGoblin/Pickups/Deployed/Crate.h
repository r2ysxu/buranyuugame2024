// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crate.generated.h"

UCLASS()
class NECOGOBLIN_API ACrate : public AActor {
	GENERATED_BODY()
	
public:	

protected:
	virtual void BeginPlay() override;

	class USphereComponent* InteractRangeBox;

public:	
	ACrate();

	UFUNCTION()	virtual void OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
