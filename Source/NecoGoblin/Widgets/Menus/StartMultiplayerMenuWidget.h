// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMultiplayerMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NECOGOBLIN_API UStartMultiplayerMenuWidget : public UUserWidget {
	GENERATED_BODY()

private:
	void PopulateSessionList(TArray<FOnlineSessionSearchResult*> Results);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* MPHostButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* MPSearchButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta =(BindWidget))
	class UVerticalBox* SessionListPanel;

	class AMainMenuActor* Parent;

	virtual void NativeConstruct() override;

	UFUNCTION() void OnBackClicked();
	UFUNCTION() void OnMPHostClicked();
	UFUNCTION() void OnMPSearchClicked();

	UFUNCTION() void OnUpdateSearchedSession();
	
public:
	UFUNCTION(BlueprintCallable) void SetParent(class AMainMenuActor* MenuActor);
};
