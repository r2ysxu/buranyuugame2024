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
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AActor* StartMenuCamera;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AActor* MainMenuCamera;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AActor* MPMenuCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDs")
	class UWidgetComponent* LogoMenuWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDs")
	class UWidgetComponent* GameMenuWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDs")
	class UWidgetComponent* StartMenuWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDs")
	class UWidgetComponent* MPMenuWidget;
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
