// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingLibrary.h"

void UMyShootingLibrary::MoveMyActor(AActor* actor, FVector dir, float speed, float deltaTime) {

	actor->SetActorLocation(actor->GetActorLocation() + dir * speed * deltaTime);
}
