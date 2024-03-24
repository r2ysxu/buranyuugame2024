// Fill out your copyright notice in the Description page of Project Settings.


#include "NGGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

UNGGameInstance::UNGGameInstance() {}

void UNGGameInstance::Init() {
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNGGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNGGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNGGameInstance::OnJoinSessionComplete);
		}
	}
}

void UNGGameInstance::OnCreateSessionComplete(FName SessionName, bool IsSuccessful) {
	if (IsSuccessful) {
		GetWorld()->ServerTravel("/Game/NecoGoblin/Maps/MenuMap/MPLobbyWorld?listen");
	}
}

void UNGGameInstance::OnFindSessionComplete(bool IsSuccessful) {
	if (IsSuccessful) {
		SessionSearchResults = FoundSessions->SearchResults;
		DelegateSessionFound.Broadcast();
	}
}

void UNGGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (Result == EOnJoinSessionCompleteResult::Type::Success) {
		if (APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
			FString sessionAddress;
			SessionInterface->GetResolvedConnectString(SessionName, sessionAddress);
			controller->ClientTravel(sessionAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UNGGameInstance::HostSession() {
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = true;
	//sessionSettings.bIsLANMatch = false;
	sessionSettings.bUsesPresence = true;
	sessionSettings.NumPublicConnections = 4;
	sessionSettings.bShouldAdvertise = true;

	SessionInterface->CreateSession(0, DefaultSessionName, sessionSettings);
}

void UNGGameInstance::SearchSessions() {
	FoundSessions = MakeShareable(new FOnlineSessionSearch());
	FoundSessions->bIsLanQuery = true;
	FoundSessions->MaxSearchResults = 10000;
	SessionInterface->FindSessions(0, FoundSessions.ToSharedRef());
}

void UNGGameInstance::JoinSearchedSession(FOnlineSessionSearchResult SessionResult) {
	SessionInterface->JoinSession(0, DefaultSessionName, SessionResult);
}

void UNGGameInstance::JumpToLevel(FString LevelName) {
	GetWorld()->ServerTravel(LevelName);
}

TArray<FOnlineSessionSearchResult> UNGGameInstance::GetSearchResults() {
	return SessionSearchResults;
}
