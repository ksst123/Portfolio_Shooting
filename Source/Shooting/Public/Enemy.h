// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTING_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Settings")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Settings")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Settings")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category="Enemy Settings")
	int32 TraceRate = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Settings")
	class UParticleSystem* ExplosionEffect;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION() // 델리게이트에 연결하기 위해서
	void DestroyMyself();

	UFUNCTION()
	void SetNewDirection(FVector NewDirection);


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



private:
	FVector MoveDirection;
	class AShootingFlight* TraceTarget;
};
