// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class SHOOTING_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category="Spawner Settings")
	float SpawnTime = 2.0f;

	UPROPERTY(EditInstanceOnly, Category = "Spawner Settings")
	bool IsSpawn = true;

	UPROPERTY(EditInstanceOnly, Category="Spawner Settings")
	TSubclassOf<class AEnemy> EnemyFactory;

	UPROPERTY(EditInstanceOnly, Category="Spawner Settings")
	USceneComponent* RootScene;

	UPROPERTY(EditInstanceOnly, Category="Spawner Settings")
	class UArrowComponent* SpawnDirection;

	class AShootingGameModeBase* myGM;
	int32 MyScore;

private:
	float CurrentTime = 0.0f;

};
