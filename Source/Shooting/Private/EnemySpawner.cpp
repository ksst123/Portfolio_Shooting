// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "Components/ArrowComponent.h"
#include "ShootingGameModeBase.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;

	SpawnDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Direction"));
	SpawnDirection->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	myGM = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 만일 IsSpawn의 값이 False 라면 Tick 함수를 종료
	if (!IsSpawn)
	{
		return;
	}

	// 만일 게임모드의 CurrentScore가 30 이상이면 함수 종료
	/*MyScore = myGM->CurrentScore;
	if (MyScore >= 30)
	{
		return;
	}*/


	// 경과된 시간을 누적
	CurrentTime += DeltaTime;
	// 만일 누적 시간이 SpawnTime 이상일 경우

	if (CurrentTime >= SpawnTime)
	{
		// Enemy 블루프린트를 생성
		GetWorld()->SpawnActor<AEnemy>(EnemyFactory, SpawnDirection->GetComponentLocation(), SpawnDirection->GetComponentRotation());
		//누적된 시간을 다시 0으로 초기화
		CurrentTime = 0;
	}
}

