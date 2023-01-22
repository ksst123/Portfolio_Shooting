// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyShootingLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMyShootingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void MoveMyActor(class AActor* actor, FVector dir, float speed, float deltaTime);
};
