// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UStartMenuWidget : public UUserWidget{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartButton;

	class AMainMenuActor* Parent;
	class AActor* MenuCamera;

	virtual void NativeConstruct() override;

	UFUNCTION() void OnStartClicked();

public:
	UFUNCTION(BlueprintCallable) void SetParent(class AMainMenuActor* MenuActor);
};
