// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShopKeeper.generated.h"

UCLASS()
class NECOGOBLIN_API AShopKeeper : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShopKeeper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class USphereComponent* ShopRangeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> ShopOpenHudWidgetClass;
	UUserWidget* ShopOpenHudWidget;

public:	

	UFUNCTION()	void OnWithinShopRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnOutsideShopRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
