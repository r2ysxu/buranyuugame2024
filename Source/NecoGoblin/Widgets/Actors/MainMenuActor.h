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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* MPMenuCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDs")
	TSubclassOf<class UStartMainMenuWidget> StartMainMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDs")
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	class UStartMainMenuWidget* StartMainMenuWidget;
	class UUserWidget* LoadingWidget;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	void ChangeToMenuCamera();
	void ChangeToMultiplayer();
	void ChangeToSingleplayer();
};
