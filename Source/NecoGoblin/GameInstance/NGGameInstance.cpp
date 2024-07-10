// Fill out your copyright notice in the Description page of Project Settings.


#include "NGGameInstance.h"

#include "Containers/Array.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/GameplayStatics.h"

UNGGameInstance::UNGGameInstance() {}

void UNGGameInstance::Init() {
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNGGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNGGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNGGameInstance::OnJoinSessionComplete);
			SessionInterface->OnSessionFailureDelegates.AddUObject(this, &UNGGameInstance::OnSessionFailed);
			SessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UNGGameInstance::OnSessionEnded);
		}
	}
}

void UNGGameInstance::OnCreateSessionComplete(FName SessionName, bool IsSuccessful) {
	if (IsSuccessful) {
		GetWorld()->ServerTravel("/Game/NecoGoblin/Maps/MultiplayerMaps/PersistentLevel_MP?listen");
	}
}

void UNGGameInstance::OnFindSessionComplete(bool IsSuccessful) {
	if (IsSuccessful) {
		for (FOnlineSessionSearchResult result : FoundSessions->SearchResults) {
			SessionSearchResults.Add(&result);
		}
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

void UNGGameInstance::OnSessionFailed(const FUniqueNetId& Id, ESessionFailure::Type ErrorType) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("OnSessionFailed"));
}

void UNGGameInstance::OnSessionEnded(FName SessionName, bool IsSuccessful) {
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("OnSessionEnded"));
}

void UNGGameInstance::HostSession() {
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = false;
	sessionSettings.bUsesPresence = true;
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.NumPublicConnections = 4;
	sessionSettings.bShouldAdvertise = true;

	SessionInterface->CreateSession(0, DefaultSessionName, sessionSettings);
}

void UNGGameInstance::SearchSessions() {
	FoundSessions = MakeShareable(new FOnlineSessionSearch());
	FoundSessions->bIsLanQuery = false;
	FoundSessions->MaxSearchResults = 100;
	FoundSessions->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, FoundSessions.ToSharedRef());
}

void UNGGameInstance::QuitSession() {
	SessionInterface->EndSession(DefaultSessionName);
	SessionInterface->DestroySession(DefaultSessionName);
}

void UNGGameInstance::JoinSearchedSession(FOnlineSessionSearchResult SessionResult) {
	SessionInterface->JoinSession(0, DefaultSessionName, SessionResult);
}

void UNGGameInstance::JumpToLevel(FString LevelName) {
	GetWorld()->ServerTravel(LevelName);
}

TArray<FOnlineSessionSearchResult*> UNGGameInstance::GetSearchResults() {
	return SessionSearchResults;
}
