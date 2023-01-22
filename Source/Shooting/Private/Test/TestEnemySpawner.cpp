// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestEnemySpawner.h"
#include "Components/ArrowComponent.h"
#include "Test/TestEnemyActor.h"


// Sets default values
ATestEnemySpawner::ATestEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	SpawnDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnDirection"));
	SpawnDirection->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATestEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;
	if (CurrentTime >= SpawnTime)
	{
		GetWorld()->SpawnActor<ATestEnemyActor>(EnemyFactory, SpawnDirection->GetComponentLocation(), SpawnDirection->GetComponentRotation());
		CurrentTime = 0;
	}
}

