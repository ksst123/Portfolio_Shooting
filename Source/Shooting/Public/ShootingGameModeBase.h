// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="UI Settings")
	TSubclassOf<class UMainWidget> MainWidget;

	UPROPERTY(EditAnywhere, Category="UI Settings")
	TSubclassOf<class UMenuWidget> MenuWidget;

	TArray<class AEnemy*> EnemyArray;

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentScore() { return CurrentScore; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetHighScore() { return HighScore; }

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABossEnemy> BossSpawner;

	bool bIsAppearBoss = false;

	virtual void BeginPlay() override;

	void AddScore(int32 point);

	void ShowMenu();

	UFUNCTION()
	void SpawnBoss();

	void StopAllEnemySpawn();

	int32 CurrentScore = 0;
private:

	int32 HighScore = 0;

	// HighScore 파일을 저장할 절대 경로
	FString SavePath = FString("D:/Unreal Projects/Shooting/SaveScore/HighScore.txt");

	// 저장 상대 경로
	// FString SavePathRelative = FString("../../../Content/SaveScore/HighScore.txt");

	class UMainWidget* MainUI;
	class UMenuWidget* MenuUI;
};
