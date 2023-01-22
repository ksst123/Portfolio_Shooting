// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGameModeBase.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 충돌 영역 생성
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// BoxComponent를 RootComponent 로 설정
	RootComponent = BoxComponent;
	BoxComponent->SetBoxExtent(FVector(50.0f));
	BoxComponent->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));

	// 스태틱 메시 생성
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// 스태틱 메시를 루트 컴포넌트에 할당
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50.0f));
	MeshComponent->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// 충돌(overlap) 발생 시 실행할 함수를 연결
	// BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);

	// 오버랩 이벤트가 발생하도록 설정
	BoxComponent->SetGenerateOverlapEvents(true);
	
	// 생성 후 3초 뒤에 제거
	// SetLifeSpan(3.0f);

	// 생성 후 3초 뒤에 제거, World의 TimerManager 이용
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ABullet::DestroyBulletSelf, 3.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 액터의 정면 방향으로 계속 이동
	// direction = FVector(1, 0, 0);
	direction = GetActorForwardVector();

	FVector dir = GetActorLocation() + direction * MoveSpeed * DeltaTime;
	SetActorLocation(dir);
}



void ABullet::DestroyBulletSelf() {
	Destroy();
}

