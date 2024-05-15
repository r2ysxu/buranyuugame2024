// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMultiplayerMenuWidget.h"
#include "./Components/JoinSessionButton.h"
#include "../Actors/MainMenuActor.h"
#include "../../GameInstance/NGGameInstance.h"

#include "Blueprint/WidgetTree.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UStartMultiplayerMenuWidget::NativeConstruct() {
	BackButton->OnClicked.AddDynamic(this, &UStartMultiplayerMenuWidget::OnBackClicked);
	MPHostButton->OnClicked.AddDynamic(this, &UStartMultiplayerMenuWidget::OnMPHostClicked);
	MPSearchButton->OnClicked.AddDynamic(this, &UStartMultiplayerMenuWidget::OnMPSearchClicked);

	Cast<UNGGameInstance>(GetGameInstance())->DelegateSessionFound.AddDynamic(this, &UStartMultiplayerMenuWidget::OnUpdateSearchedSession);
}

void UStartMultiplayerMenuWidget::PopulateSessionList(TArray<FOnlineSessionSearchResult*> Results) {
	SessionListPanel->ClearChildren();
	for (FOnlineSessionSearchResult* result : Results) {
		UHorizontalBox* frame = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("SessionResult_Frame"));
		UTextBlock* sessionNameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SessionResult_Name"));
		sessionNameText->SetText(FText::FromString(result->GetSessionIdStr()));
		UJoinSessionButton* joinSessionbutton = WidgetTree->ConstructWidget<UJoinSessionButton>(UJoinSessionButton::StaticClass(), TEXT("JoinSesion_Button"));
		joinSessionbutton->SetupJoinTarget(*result);
		UTextBlock* joinSessionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SessionResult_JoinButtonPrompt"));
		joinSessionText->SetText(FText::FromString("Join"));
		joinSessionbutton->AddChild(joinSessionText);

		frame->AddChildToHorizontalBox(sessionNameText);
		frame->AddChildToHorizontalBox(joinSessionbutton);
		SessionListPanel->AddChildToVerticalBox(frame);
	}
}

void UStartMultiplayerMenuWidget::OnBackClicked() {
	Parent->ChangeToMenuCamera();
	//SetVisibility(ESlateVisibility::Hidden);
	//RemoveFromParent();
}

void UStartMultiplayerMenuWidget::OnMPHostClicked() {
	MPHostButton->SetIsEnabled(false);
	Cast<UNGGameInstance>(GetGameInstance())->HostSession();
}

void UStartMultiplayerMenuWidget::OnMPSearchClicked() {
	Cast<UNGGameInstance>(GetGameInstance())->SearchSessions();
	MPSearchButton->SetIsEnabled(false);
}

void UStartMultiplayerMenuWidget::OnUpdateSearchedSession() {
	MPSearchButton->SetIsEnabled(true);
	TArray<FOnlineSessionSearchResult*> results = Cast<UNGGameInstance>(GetGameInstance())->GetSearchResults();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::FromInt(results.Num()));
	PopulateSessionList(results);
}

void UStartMultiplayerMenuWidget::SetParent(AMainMenuActor* MenuActor) {
	Parent = MenuActor;
}
