// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AmmoCrate.generated.h"

UCLASS()
class NECOGOBLIN_API AAmmoCrate : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAmmoCrate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class USphereComponent* InteractRangeBox;

	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMesh* Mesh;
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> InteractHudWidgetClass;
	UUserWidget* InteractHudWidget;

public:	

	UFUNCTION()	void OnWithinRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnOutsideRange(UPrimitiveComponent* OverlappedComponent, AActor* actor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
