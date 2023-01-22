// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "PlayerBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API APlayerBullet : public ABullet
{
	GENERATED_BODY()
	
public:
	APlayerBullet();

	virtual void BeginPlay() override;

	// Delegate로 연결할 함수는 반드시 UFCUNTION 으로 선언해야 한다
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetDirection(FVector dir);
};
