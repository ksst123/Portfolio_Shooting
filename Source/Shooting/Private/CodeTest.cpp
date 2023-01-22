// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeTest.h"
#include "PointerTest.h"

// Sets default values
ACodeTest::ACodeTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodeTest::BeginPlay()
{
	Super::BeginPlay();
	
	//// number 변수에 3을 대입한다.
	//number = 3;

	//// number2 변수에 number 변수를 대입한다.
	//number2 = number;
	//// number2 변수에 -7를 대입한다.
	//number2 = -7;

	//// number2의 값을 output log 콘솔 창에 출력한다.
	//UE_LOG(LogTemp, Warning, TEXT("number is %d, number2 is %d"), number, number2);
	//UE_LOG(LogTemp, Error, TEXT("Hello, world!"));

	//fnumber = 3.14f;
	//UE_LOG(LogTemp, Warning, TEXT("float number is %.2f, float number2 is %.3f"), fnumber, fnumber2);
	//UE_LOG(LogTemp, Error, TEXT("Hello, world! %s"), *name2);

	//if (isStudent)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("그는 학생이다."));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("그는 선생이다."));
	//}

	//isTeacher = false;
	//if (isTeacher)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("그는 선생이다."));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("그는 학생이다."));
	//}

	//int32 result_add = Add(number, number2);
	//int32 result_sub = Sub(number, number2);
	//int32 result_mul = Mul(number, number2);
	//float result_div = Div(number, number2);
	//UE_LOG(LogTemp, Error, TEXT("number + number2 = %d"), result_add);
	//UE_LOG(LogTemp, Error, TEXT("number - number2 = %d"), result_sub);
	//UE_LOG(LogTemp, Error, TEXT("number * number2 = %d"), result_mul);
	//UE_LOG(LogTemp, Error, TEXT("number / number2 = %.2f"), result_div);

	//// 만일 나이가 20살 이상이면 "꼰대"라고 출력한다.
	//if (age >= 30)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("할배다~"));
	//}
	//else if (age >= 20)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("꼰대시군요~"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("잼민이네~"));
	//}

	// 만일 나이가 20살 이상이면서, 키가 160 이상인 경우에는
	// "환영"이라고 출력하고,
	// 그렇지 않으면,
	// "외면"이라고 출력한다.
	//if (age >= 20 || height >= 160)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("환영합니다~~~!!"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("실례했습니다."));
	//}


	//// 만일 국적이 "한국"이면서, 나이가 20세 이상 40세 미만인 경우 "청년"이라고 출력한다.
	//// 그 밖에는, "청년아님"이라고 출력한다.
	//if (nation == TEXT("한국") && (age >= 20 && age < 40))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("청년"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("청년아님"));
	//}

	//UE_LOG(LogTemp, Error, TEXT("%s"), *GetFullName(string1, string2));

	
	//int32 checker = 30;
	//// checker 변수의 값이 짝수라면 "even" 이라고 출력.
	//// 그렇지 않고 홀수라면 "odd" 라고 출력.
	//if (checker == 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("0"));
	//}
	//else if(checker % 2 == 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("even"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("odd"));
	//}

	/*MultiplicationTable();*/


	/*for (int i = 0; i < ages.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), ages[i]);
	}

	UE_LOG(LogTemp, Warning, TEXT("Array length is : %d"), ages.Num());*/




	/*distances.Add("Seoul", 250.5f);
	distances.Add("Incheon", 325.3f);
	distances.Add("Sungnam", 123.9f);
	FString myKey = "Incheon";*/

	// UE_LOG(LogTemp, Warning, TEXT("%s : %.1f"), *myKey, distances[myKey]);

	/*for (auto dist : distances)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : %f"), *dist.Key, dist.Value);
	}*/


	/*for (auto age : ages)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), age);
	}*/

	/*int32 number1 = 10;
	int32 copyNum = number1;
	UE_LOG(LogTemp, Warning, TEXT("%d, %d"), number1, copyNum);
	number1 = 30;
	UE_LOG(LogTemp, Warning, TEXT("%d, %d"), number1, copyNum);*/

	
	/*int32 number1 = 10;
	int32* copyNum = &number1;
	number1 = 30;
	UE_LOG(LogTemp, Warning, TEXT("%d, %d"), number1, *copyNum);
	UE_LOG(LogTemp, Warning, TEXT("%d, %p"), number1, copyNum);
	UE_LOG(LogTemp, Warning, TEXT("%p, %p"), &number1, copyNum);*/


	/*int32 aa = 10;
	int32 bb = 15;
	int32 result_add;
	result_add = Add(&aa, &bb);
	UE_LOG(LogTemp, Warning, TEXT("aa : %d, bb : %d, result : %d"), aa, bb, result_add);
	int32 result_sub;
	result_sub = Sub(aa, bb);
	UE_LOG(LogTemp, Warning, TEXT("aa : %d, bb : %d, result : %d"), aa, bb, result_sub);*/

	/*for(APointerTest* pt : pointerTest)
	{
		pt->pt_number = 115;
	}*/

	if (pointerTest != nullptr)
	{
		*(pointerTest->myAge) = 15;
		// pointerTest->realAge = 15;
	}
}

void ACodeTest::MultiplicationTable()
{
	for (int i = 2; i <= 9; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("========== %d단 =========="), i);
		for (int j = 1; j <= 9; j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d X %d = %d"), i, j, i * j);
		}
	}
}


// Called every frame
void ACodeTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ACodeTest::Add(int32* num1, int32* num2)
{
	*num1 *= 2;
	*num2 *= 2;

	return *num1 + *num2;
}

int32 ACodeTest::Sub(int32& num1, int32& num2)
{
	return num1 - num2;
}

int32 ACodeTest::Mul(int32 num1, int32 num2)
{
	return num1 * num2;
}

float ACodeTest::Div(int num1, int num2)
{
	return static_cast<float>(num1) / num2;
}

FString ACodeTest::GetFullName(FString FirstName, FString LastName) 
{
	return LastName + FirstName;
}