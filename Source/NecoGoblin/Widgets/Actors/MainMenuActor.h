// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuActor.generated.h"

UCLASS()
class NECOGOBLIN_API AMainMenuActor : public AActor {
	GENERATED_BODY()
	
public:	
	AMainMenuActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* StartMenuCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* MainMenuCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDs")
	TSubclassOf<class UStartMainMenuWidget> StartMainMenuWidgetClass;

	class UStartMainMenuWidget* StartMainMenuWidget;

	virtual void BeginPlay() override;

public:	

	void ChangeToMenuCamera();
};
