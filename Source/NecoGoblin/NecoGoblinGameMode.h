// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/HUDs/RoundHUD.h"
#include "NecoGoblinGameMode.generated.h"


UCLASS(minimalapi)
class ANecoGoblinGameMode : public AGameModeBase {
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_Gameover, float, TimeDelay);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_GameRoundChange, float, EnemySpawnRate, float, EnemyMovementSpeedMod);

private:
	const int MELEE_ENEMY_PER_ROUND = 10;
	const int RANGE_ENEMY_PER_ROUND = 3;
	const int MAX_ENEMY = 100;
	const int FINAL_ROUND = 10;

	bool bEndlessMode = false;

	void GameRestart();
	void SetupEndlessMode();

protected:
	FTimerHandle NextRoundHandler;
	FTimerHandle RestartHandler;

	int CurrentRound = 0;
	int RangeEnemySpawned = 0;
	int MeleeEnemySpawned = 0;
	int MeleeEnemyPerRound;
	int RangeEnemyPerRound;
	int EnemyCount;
	float EnemySpawnRate = 5.f;

	float EndlessNextRoundTime = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class URoundHUD> RoundHudWidgetClass;
	URoundHUD* RoundHudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UUserWidget* GameOverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voices", meta = (AllowPrivateAccess = "true"))
	USoundBase* NextRoundVoice;

	void NextRound();
	void NextEndlessRound();

public:
	ANecoGoblinGameMode();

	FDelegate_Gameover DelegateGameOver;
	FDelegate_GameRoundChange DelegateRoundChange;

	void StartPlay() override;
	
	bool IncrementMeleeEnemy();
	bool IncrementRangeEnemy();
	bool DecrementEnemy();
	void SetEndlessMode(bool IsEndlessMode);

	UFUNCTION(BlueprintCallable) void ShowHuds();
	UFUNCTION(BlueprintCallable) void HideHuds();
	UFUNCTION(BlueprintCallable) void RestartPlay(float TimeDelay);
	UFUNCTION(BlueprintImplementableEvent) void OnVictoryAchieved();
	FORCEINLINE int GetCurrentRound() { return CurrentRound; }
	FORCEINLINE int GetGoblinCount() { return EnemyCount; }
	FORCEINLINE int GetMeleeGoblinPerRound() { return MeleeEnemyPerRound; }
	FORCEINLINE int GetRangeEnemyPerRound() { return RangeEnemyPerRound; }
	FORCEINLINE float GetGoblinSpawnRate() { return EnemySpawnRate; }
};
