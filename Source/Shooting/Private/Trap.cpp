// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShootingFlight.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// USphereComponent로 루트 콜리전 생성
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(200);

	// UstaticMesh를 루트의 자식 컴포넌트로 생성
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(4.0f));
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 충돌 프리셋을 "TrapPreset" 으로 설정
	SphereComponent->SetCollisionProfileName(TEXT("TrapPreset"));
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel6);

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
	// overlap 델리게이트에 InTrap 함수를 연결
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrap::InTrap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATrap::OutTrap);


}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 현재 누적 시간(CurrentTime)이 지정된 시간(TeleportTime)에 도달하면
	if (CurrentTime >= TeleportTime)
	{
		// 랜덤한 위치(벡터)를 설정
		float RandomY = FMath::RandRange(-200.0f, 200.0f);
		float RandomZ = FMath::RandRange(-600.0f, 600.0f);

		/*RandomLocation.Y = RandomY;
		RandomLocation.Z = RandomZ;*/
		FVector RandomLocation = FVector(0.0f, RandomY, RandomZ);
		
		/*FVector RandomLocation = FMath::VRand() * 500.0f;
		RandomLocation.X = 0.0f;*/

		// 해당 위치로 이동
		// SetActorLocation(RandomLocation);
		TeleportTo(RandomLocation, GetActorRotation());

		// 누적 시간(CurrentTime) 초기화
		CurrentTime = 0.0f;
	}
	// 그렇지 않다면
	else
	{
		// 시간을 누적
		CurrentTime += DeltaTime;
	}

}

void ATrap::InTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	AShootingFlight* player = Cast<AShootingFlight>(OtherActor);

	if (player != nullptr)
	{
		player->IsTrapped = true;
	}
}

void ATrap::OutTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	AShootingFlight* player = Cast<AShootingFlight>(OtherActor);
	if (player != nullptr)
	{
		player->IsTrapped = false;
	}
}