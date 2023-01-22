// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class SHOOTING_API ATrap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// USphereComponent 선언
	UPROPERTY(EditAnywhere, Category = "Trap Settings")
	class USphereComponent* SphereComponent;

	// UStaticMeshComponent 선언
	UPROPERTY(EditAnywhere, Category = "Trap Settings")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Trap Settings")
	float TeleportTime = 5.0f;

	// ComponentBeginOverlap 함수에 연결할 InTrap 함수 선언
	UFUNCTION()
	void InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	float CurrentTime = 0.0f;
};