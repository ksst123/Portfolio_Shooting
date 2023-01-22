// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBullet.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(50.0f, 300.0f, 150.0f));
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetCollisionProfileName(TEXT("BossPreset"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1.0f, 6.0f, 3.0f));
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -150.0f));
	MeshComponent->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	TargetLocation = FVector(0.0f, 0.0f, 200.0f);

	/*FTimerHandle Pattern1Timer;
	GetWorld()->GetTimerManager().SetTimer(Pattern1Timer, this, &ABossEnemy::BossAttack1, 5, true, 3);*/
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GetActorLocation().Z >= 200.0f)
	//{
	//	// 현재 위치에서 (0.0f, 0.0f, 200.0f) 위치까지 이동
	//	SetActorLocation(GetActorLocation() - FVector(0.0f, 0.0f, DeltaTime * 100));


	//}

	// 현재 위치에서 (0.0f, 0.0f, 200.0f) 위치까지 이동
	//float distance = (StartLocation - GetActorLocation()).Length();
	//float shaking = FMath::RandRange(-1.0f, 1.0f);
	//if (distance <= 500.0f)
	//{
	//	FVector direction = GetActorUpVector() * -1;
	//	direction.Y = shaking;
	//	SetActorLocation(GetActorLocation() + direction * 200.0f * DeltaTime);
	//}
	//else
	//{
	//	// 원하는 위치에서 오차가 생길 경우 수정
	//	SetActorLocation(FVector(0.0f, 0.0f, 200.0f));
	//}


	// 현재 위치에서 (0.0f, 0.0f, 200.0f) 위치까지 이동(선형 보간)
	TestValue += DeltaTime * 0.5;
	
	// TestValue가 어떤 범위 안에서 벗어나지 못하도록(Clamp) 한다
	TestValue = FMath::Clamp(TestValue, 0.0f, 1.0f);

	FVector Result = FMath::Lerp(StartLocation, TargetLocation, TestValue);
	SetActorLocation(Result);

	if (CurrentTime >= PatternDelay)
	{
		BossAttack1(15.0f, 10);
		CurrentTime = 0;
	}
	else
	{
		CurrentTime += DeltaTime;
	}
}
	
void ABossEnemy::BossAttack1(float angle, int32 count) {

	// 벡터 이용
	// angle 간격으로 count 수 만큼 총알을 생성
	/*BulletSpawnPostion = GetActorLocation() + GetActorUpVector() * -100.0f;
	float ExtraBossBulletAngle = (count - 1) * angle * -0.5f;
	FRotator BulletSpawnRotation = FRotator(-90.0f, 0.0f, 0.0f);
	for (int32 i = 0; i < count; i++)
	{
		AEnemyBullet* EnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(BossBullet, BulletSpawnPostion, BulletSpawnRotation);
		FRotator AngleOffset = FRotator(0.0f, 0.0f, ExtraBossBulletAngle + angle * i);

		if (EnemyBullet != nullptr)
		{
			EnemyBullet->AddActorWorldRotation(AngleOffset);
		}
	}*/



	// 수학 이용
	// angle 간격으로 count 수 만큼 총알을 생성
	// 시작 각도 = 270 - (angle * (count - 1) * 0.5
	float StartAngle = 270 - (angle * (count - 1) * 0.5f);

	for (int32 i = 0; i < count; i++)
	{
		// 기본 좌표 (0, rcos@, rsin@)
		FVector BulletBase = FVector(0.0f, 
			200 * FMath::Cos(FMath::DegreesToRadians(StartAngle + angle * i)), 
			200 * FMath::Sin(FMath::DegreesToRadians(StartAngle + angle * i)));

		AEnemyBullet* EnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(BossBullet, GetActorLocation() + BulletBase, FRotator(-90.0f, 0.0f, 0.0f));
		EnemyBullet->SetDirection(BulletBase.GetSafeNormal());
		// EnemyBullet->AddActorLocalRotation(FRotator(0.0f, StartAngle + angle * i, 0.0f));


		//AEnemyBullet* EnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(BossBullet, GetActorLocation() + BulletBase, FRotator(-90.0f, 0.0f, 0.0f));
		//// UpVector 축을 BulletBase 방향으로 회전시켰을 때 Rotator 값을 계산
		FRotator rot = UKismetMathLibrary::MakeRotFromZX(EnemyBullet->GetActorUpVector(), BulletBase.GetSafeNormal());
		EnemyBullet->SetActorRotation(rot);
		//// EnemyBullet->SetDirection(EnemyBullet->GetActorForwardVector());
	}
}

