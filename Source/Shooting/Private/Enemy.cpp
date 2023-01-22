// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingFlight.h"
#include "EngineUtils.h"
#include "ShootingGameModeBase.h"
#include "MyShootingLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = BoxComponent;
	BoxComponent->SetBoxExtent(FVector(50.0f));
	// Collision Enabled 값을 Query and Physics 로 설정
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// Object Type(Collision Response Channel?) 을 "Enemy"로 설정
	BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
	// response channel 을 일괄적으로 ignore 상태로 처리
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	// response channel 을 Player 와 Bullet 채널에 대해서만 Overlap 으로 처리
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50.0f));


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 게임 모드의 EnemyArray 배열에 자기 자신을 넣는다(가리키게 한다).
	/*AShootingGameModeBase* MyGM = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	if (MyGM != nullptr)
	{
		MyGM->EnemyArray.Add(this);
	}*/

	// 델리게이트에 연결


	// 플레이어 제거 함수 연결
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	// 오버랩 이벤트 활성화
	BoxComponent->SetGenerateOverlapEvents(true);



	// 랜덤으로 정면 방향 또는 플레이어 방향으로 이동 결정
	// 랜덤 확률은 변수를 이용해서 7:3(정면:플레이어) 비율
	int32 RandomNum = FMath::RandRange(1, 100);

	for (TActorIterator<AShootingFlight> itr(GetWorld()); itr; ++itr)
	{
		TraceTarget = *itr;
	}

	// 만일 값이 TraceRate 이하라면 플레이어를 향해서 (플레이어 위치 - 나의 위치 = 갈 방향)
	if (RandomNum <= TraceRate)		
	{
		//// 월드에서 특정한 객체를 찾는 1번째 방법.
		//AActor* TraceTarget = UGameplayStatics::GetActorOfClass(GetWorld(), AShootingFlight::StaticClass());
		//AShootingFlight* RealTraceTarget = Cast<AShootingFlight>(TraceTarget);
		/*if (RealTraceTarget != nullptr)
		{
			float temp = RealTraceTarget.MoveSpeed;
		}*/

		// 월드에서 특정한 객체를 찾는 2번째 방법
		// iterator 에서는 itr; 는 itr != nullptr;  조건식과 같다
		// 1번째 방식은 어떤 형태의 클래스든 액터 형식으로 반환하기 때문에 캐스팅을 해줘야 하는데, 2번째는 캐스팅이 필요없다.

		/*for (TActorIterator<AShootingFlight> itr(GetWorld()); itr; ++itr)
		{
			TraceTarget = *itr;
		}*/
		// float temp = TraceTarget->MoveSpeed;

		if (TraceTarget != nullptr)
		{
			// 플레이어 위치 - 나의 위치 = 갈 방향
			FVector TargetDirection = TraceTarget->GetActorLocation() - GetActorLocation();
			// TargetDirection 의 길이가 길어서 이 상태에서 MoveSpeed와 곱하면 너무 커지므로 정규화해서 1짜리 단위벡터로 만든다.
			TargetDirection.Normalize();
			MoveDirection = TargetDirection;

			// 델리게이트에 에너미의 DestroyMeself 함수 연결
			// 에너미 일부만 터지는 이유는, 플레이어를 쫓아오는 에너미만 대상으로 하기 때문에
			// TraceTarget->PlayerBomb.AddDynamic(this, &AEnemy::DestroyMyself);
		}
	}
	else // 그렇지 않으면 정면 방향
	{
		MoveDirection = GetActorForwardVector();
	}

	if (TraceTarget != nullptr)
	{
		TraceTarget->OnEnemyDirModify.AddDynamic(this, &AEnemy::SetNewDirection);
	}
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// MoveDirection = GetActorForwardVector();
	/*FVector EnemyMove = GetActorLocation() + MoveSpeed * DeltaTime * MoveDirection;
	SetActorLocation(EnemyMove);*/

	// 우리가 만든 라이브러리 함수를 이용해서 이동시킨다
	UMyShootingLibrary::MoveMyActor(this, MoveDirection, MoveSpeed, DeltaTime);
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// OtherActor 가 ShootingFlight 인지 확인
	AShootingFlight* PlayerFlight = Cast<AShootingFlight>(OtherActor);
	// 만약 OtherActor 가 ShootingFlight 라면
	if (PlayerFlight != nullptr)
	{
		// 플레이어 제거
		PlayerFlight->Destroy();

		// MenuWidget을 생성하는 함수를 실행
		AShootingGameModeBase* GameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode != nullptr)
		{
			GameMode->ShowMenu();
		}

		// 플레이어와 충돌 시 플레이어 색 변경
		// PlayerFlight->ReserveChangeColor(0.1f);

		// 스스로도 제거
		Destroy();
	}
}

void AEnemy::DestroyMyself() {
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), true);
	Destroy();
}



void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason) {

	// 자기 자신을 배열에서 제거
	/*AShootingGameModeBase* MyGM = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	if (MyGM != nullptr)
	{
		MyGM->EnemyArray.Remove(this);
	}*/

	// 델리게이트에 걸어놓은 자기 함수를 제거
	// TraceTarget->PlayerBomb.RemoveDynamic(this, &AEnemy::DestroyMyself);

	if (TraceTarget != nullptr)
	{
		TraceTarget->OnEnemyDirModify.RemoveDynamic(this, &AEnemy::SetNewDirection);
	}
	// TraceTarget->EnemyDirModifier.RemoveDynamic(this, &AEnemy::SetNewDirection);
}

void AEnemy::SetNewDirection(FVector NewDirection) 	{

	MoveDirection = NewDirection;
}