// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingFlight.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EngineUtils.h"
#include "ShootingGameModeBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
AShootingFlight::AShootingFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 박스 컴포넌트 생성
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// 생성된 충돌 박스 컴포넌트를 루트 컴포넌트로 설정
	SetRootComponent(BoxComponent);
	// 박스 콜리전 크기를 (50, 50, 50) 으로 설정
	BoxComponent->SetBoxExtent(FVector(50));
	// 박스 콜리전의 충돌 처리 프리셋을 PlayerPreset 으로 설정
	BoxComponent->SetCollisionProfileName(TEXT("PlayerPreset"));


	// 메시 컴포넌트 생성
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// 생성된 메시 컴포넌트를 루트 컴포넌트의 하위 개체로 등록
	MeshComponent->SetupAttachment(RootComponent);
	// Shape_Cube 파일을 찾아서 변수로 생성
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// 만일 큐브 파일을 로드하는데 성공했다면
	if (CubeStaticMesh.Succeeded())
	{
		// 로드한 파일을 메시 컴포넌트의 Static Mesh 항목 할당
		MeshComponent->SetStaticMesh(CubeStaticMesh.Object);
	}
}

// Called when the game starts or when spawned
void AShootingFlight::BeginPlay()
{
	Super::BeginPlay();
	OriginMoveSpeed = MoveSpeed;
	
	// 플레이어 컨트롤러를 캐스팅
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem != nullptr)
		{
			SubSystem->AddMappingContext(IMC_MyMapping, 0);
		}
	}
}

// Called every frame
void AShootingFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// direction 벡터 정규화
	// 사용자가 입력한 방향대로 이동
	direction.Normalize();
	FVector dir = GetActorLocation() + (direction * MoveSpeed) * DeltaTime;
	// bSweep = true, 이동 시 충돌 체가 있는지 Sweep(쓸고 간다) 여부 true 설정
	SetActorLocation(dir, true);

}

// Called to bind functionality to input
void AShootingFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 기존의 UInputComponent* 변수를 UEnhancedInputComponent 로 변환
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// 액션 함수에 연결
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Triggered, this, &AShootingFlight::MoveHorizontal);
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Completed, this, &AShootingFlight::MoveHorizontal);

	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Triggered, this, &AShootingFlight::MoveVertical);
	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Completed, this, &AShootingFlight::MoveVertical);

	EnhancedInputComponent->BindAction(IA_FireBullet, ETriggerEvent::Triggered, this, &AShootingFlight::FireBullet);

	EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AShootingFlight::BoostStarted);
	EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AShootingFlight::BoostFinished);

	EnhancedInputComponent->BindAction(IA_ULT, ETriggerEvent::Triggered, this, &AShootingFlight::CheckEnemies);
}


void AShootingFlight::MoveHorizontal(const FInputActionValue& value) {
	h = value.Get<float>();
	direction.Y = h;
}

void AShootingFlight::MoveVertical(const FInputActionValue& value) {
	v = value.Get<float>();
	direction.Z = v;
}

void AShootingFlight::FireBullet() {
		
	if (IsTrapped)
	{
		return;
	}

	for (int32 i = 0; i < BulletCount; i++)
	{
		// 총알의 전체 간격
		float TotalSize = (BulletCount - 1) * BulletSpace;

		// 기준 위치
		float BaseY = TotalSize * -0.5f;

		// 기준 위치로 오프셋 벡터 생성
		FVector Offset = FVector(0.0f, BaseY + BulletSpace * i, 0.0f);


		// Bullet 생성 위치를 담을 변수 생성
		FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
		SpawnPosition += Offset;
		// Bullet 생성 시 회전값을 담을 변수 생성
		FRotator SpawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters SpawnParam;
		// 오브젝트 생성 시 생성 위치에 충돌 가능한 다른 오브젝트가 있을 경우에 어떻게 할 것인지 설정하는 옵션
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Bullet을 생성
		// Bullet Blueprint 변수
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);


		// 생성된 Bullet 인스턴스를 BulletAngle 만큼 일정하게 회전시킨다.
		float ExtraBulletTotalAngle = -0.5 * BulletAngle * (BulletCount - 1);
		FRotator OffsetAngle = FRotator(0.0f, 0.0f, ExtraBulletTotalAngle + BulletAngle * i);
		// Bullet->SetActorRelativeRotation(GetActorRotation() + OffsetAngle);
		if (Bullet != nullptr)
		{
			Bullet->AddActorWorldRotation(OffsetAngle);
		}
	}
	// Bullet 발사 효과음 실행
	UGameplayStatics::PlaySound2D(this, BulletFireSound);
}

void AShootingFlight::BoostStarted()
{	
	MoveSpeed *= 2;
}

void AShootingFlight::BoostFinished()
{	
	MoveSpeed = OriginMoveSpeed;
}

void AShootingFlight::ExplosionAll()
{
	 // 모든 에너미를 파괴(델리게이트)
	 PlayerBomb.Broadcast();
}

void AShootingFlight::CheckEnemies()
{
	// 반경 5미터 이내에 있는 모든 AEnemy 액터들을 감지
	// 감지된 에너미들의 정보를 담을 변수의 배열
	TArray<FOverlapResult> EnemiesInfo;
	FVector CenterLocation = GetActorLocation() + GetActorUpVector() * 700.f;
	FQuat CenterRotation = GetActorRotation().Quaternion(); // GetActorQuat();
	FCollisionObjectQueryParams params = ECC_GameTraceChannel2;
	FCollisionShape CheckShape = FCollisionShape::MakeSphere(500.f);

	GetWorld()->OverlapMultiByObjectType(EnemiesInfo, CenterLocation, CenterRotation, params, CheckShape);

	// 체크된 모든 에너미의 이름을 출력
	for (FOverlapResult EnemyInfo : EnemiesInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitted: %s"), *EnemyInfo.GetActor()->GetName());

		EnemyInfo.GetActor()->Destroy();
	}
}