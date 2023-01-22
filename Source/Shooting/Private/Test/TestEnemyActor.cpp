// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestEnemyActor.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ATestEnemyActor::ATestEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	MoveDirection = -1 * GetActorUpVector();
}

// Called when the game starts or when spawned
void ATestEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEnemyActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	FVector EnemyMove = GetActorLocation() + MoveDirection * MoveSpeed * DeltaTime;
	SetActorLocation(EnemyMove);
}

