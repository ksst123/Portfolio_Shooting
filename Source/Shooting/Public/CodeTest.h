// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// #include "PointerTest.h"

#include "CodeTest.generated.h"

// class APointerTest*;

UCLASS()
class SHOOTING_API ACodeTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACodeTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 문자열 변수 name을 선언한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name; // 초기화 안하면 default 값은 ""?

	// 문자열 변수 name2를 선언한다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString name2 = FString(TEXT("김요한")); // == L"김요한";

private:
	// 정수형 변수 number를 선언한다.
	UPROPERTY(EditDefaultsOnly, Category=CodeVariable)
	int32 number; // 초기화 안하면 default 값은 0?
	// 정수형 변수 number2를 선언한다.
	UPROPERTY(EditInstanceOnly)
	int32 number2;

	
	// 실수형 변수 fnumber를 선언한다.
	UPROPERTY(VisibleAnywhere)
	float fnumber; // 초기화 안하면 default 값은 0.0?
	// 실수형 변수 fnumber2를 선언한다.
	UPROPERTY(VisibleInstanceOnly)
	float fnumber2 = 10.0f;

	// 논리형 변수 isStudent를 선언한다.
	bool isStudent; // default는 false;
	// 논리형 변수 isTeacher를 선언한다.
	bool isTeacher = true;

	// 나이
	/*UPROPERTY(EditAnywhere, Category=CodeVariable)
	int32 age = 20;*/
	// 키
	UPROPERTY(EditAnywhere, Category=CodeVariable)
	int32 height = 170;

	// 국적
	UPROPERTY(EditAnywhere, Category=CodeVariable)
	FString nation = TEXT("한국");

	UPROPERTY(EditAnywhere, Category = CodeVariable)
	TArray<int32> ages;

	UPROPERTY(EditAnywhere, Category = CodeVariable)
	TMap<FString, float> distances;

public:
	int32 Add(int32* num1, int32* num2);
	int32 Sub(int32& num1, int32& num2);
	int32 Mul(int32 num1, int32 num2);
	float Div(int32 num1, int32 num2);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CodeVariable)
	FString string1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CodeVariable)
	FString string2;

	// 문자열 2개를 받아서 문자열 1개로 합쳐서 반환하는 함수.
	// UFUNCTION(BlueprintCallable)
	UFUNCTION(BlueprintPure)
	FString GetFullName(FString FirstName, FString LastName);

	void MultiplicationTable();

	UPROPERTY(EditAnywhere, Category=CodeVariable)
	class APointerTest* pointerTest;
	// TArray<class APointerTest*> pointerTest;
};
