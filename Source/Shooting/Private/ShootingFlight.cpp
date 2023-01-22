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
	RootComponent = BoxComponent; // == SetRootComponent(BoxComponent);
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
	// MeshComponent->SetWorldLocation(FVector(0, 0, -50));

}

// Called when the game starts or when spawned
void AShootingFlight::BeginPlay()
{
	Super::BeginPlay();
	OriginMoveSpeed = MoveSpeed;
	
	// 플레이어 컨트롤러를 캐스팅
	// APlayerController* PlayerController = Cast<APlayerController>(Controller);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem != nullptr)
		{
			SubSystem->AddMappingContext(IMC_MyMapping, 0);
		}
	}

	// 현재 색상 값 저장
	/*PlayerMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(0));
	FHashedMaterialParameterInfo Param;

	PlayerMaterial->GetVectorParameterValue(Param, InitColor);*/

	// 현재 색상 값 저장
	UMaterialInterface* PlayerMaterial = MeshComponent->GetMaterial(0);
	FHashedMaterialParameterInfo Param = FHashedMaterialParameterInfo(TEXT("Color"));
	// Material Interface 에서 벡터 파라미터 값을 InitColor 변수에 저장
	PlayerMaterial->GetVectorParameterValue(Param, InitColor);

	UE_LOG(LogTemp, Warning, TEXT("R: %f, G: %f, B: %f"), InitColor.R, InitColor.G, InitColor.B);

	// material interface PlayerMaterial 을 기준으로 Material instance dynamic 개체를 생성
	DynamicMaterial = UMaterialInstanceDynamic::Create(PlayerMaterial, this);

	// 생성한 dynamic material 을 메시 컴포넌트에 설정
	if (DynamicMaterial != nullptr)
	{
		MeshComponent->SetMaterial(0, DynamicMaterial);
	}
	
}

// Called every frame
void AShootingFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// direction 벡터 정규화
	// 사용자가 입력한 방향대로 이동
	// P = P0 + vt == SetActorLocation = GetActorLocation + (direction(방향) * speed(크기)) * DeltaTime
	// 방향*크기 = 속도, Scalar = 속도, Delta는 경과 중인 흐름 의미
	direction.Normalize(); // 길이가 1인 벡터 = 단위 벡터
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

	//// Horizontal Axis 입력에 함수를 연결
	//PlayerInputComponent->BindAxis("MoveHorizontal", this, &AShootingFlight::MoveHorizontal);
	//// Vertical Axis 입력에 함수를 연결
	//PlayerInputComponent->BindAxis("MoveVertical", this, &AShootingFlight::MoveVertical);

	//// Fire Action 입력에 함수를 연결
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShootingFlight::FireBullet);
}

//// 좌우 입력이 있을 때 실행될 함수
//void AShootingFlight::MoveHorizontal(float value) {
//	
//	// 입력 value 출력
//	h = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
//
//	direction.Y = h;
//}
//
//// 상하 입력이 있을 때 실행될 함수
//void AShootingFlight::MoveVertical(float value) {
//	
//	// 입력 value 출력
//	v = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), v);
//
//	direction.Z = v;
//}
//
//// 마우스 왼쪽 버튼을 눌렀을 때 실행될 함수
//void AShootingFlight::FireBullet() {
//	
//	// Bullet 생성 위치를 담을 변수 생성
//	FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
//	// Bullet 생성 시 회전값을 담을 변수 생성
//	// 에디터와는 다르게 코드에서는 FRotator(Pitch, Yaw, Roll) 순서
//	FRotator SpawnRotation = FRotator(90.0f, 0, 0);
//	FActorSpawnParameters SpawnParam;
//	// 오브젝트 생성 시 생성 위치에 충돌 가능한 다른 오브젝트가 있을 경우에 어떻게 할 것인지 설정하는 옵션
//	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	// Bullet을 생성
//	// Bullet Blueprint 변수
//	GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
//}

void AShootingFlight::MoveHorizontal(const FInputActionValue& value) {
	h = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
	direction.Y = h;
}

void AShootingFlight::MoveVertical(const FInputActionValue& value) {
	v = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
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
		// 에디터와는 다르게 코드에서는 FRotator(Pitch, Yaw, Roll) 순서
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

	

	/*float BulletHalfSpace = BulletSpace / 2;
	int32 BulletHalfCount = BulletCount / 2;

		if (BulletCount == 1)
		{
			FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
			FRotator SpawnRotation = FRotator(90.0f, 0, 0);
			FActorSpawnParameters SpawnParam;
			GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
			UGameplayStatics::PlaySound2D(this, BulletFireSound);
		}
		else if (BulletCount % 2 == 0)
		{
			FVector ExtraSpawnPostion;
			float ExtraSpawnStartPosition = BulletHalfSpace + (BulletSpace * (BulletHalfCount / 2));
			ExtraSpawnPostion = FVector(0.0f, -1 * ExtraSpawnStartPosition, 0.0f);
			for (int i = 1; i <= BulletCount; i++)
			{
				FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
				FRotator SpawnRotation = FRotator(90.0f, 0, 0);
				FActorSpawnParameters SpawnParam;
				GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition + ExtraSpawnPostion, SpawnRotation, SpawnParam);
				ExtraSpawnPostion += FVector(0.0f, BulletSpace, 0.0f);
			}
			UGameplayStatics::PlaySound2D(this, BulletFireSound);
		}
		else if (BulletCount % 2 != 0)
		{
			FVector ExtraSpawnPostion;
			float ExtraSpawnStartPosition = (BulletSpace * (BulletHalfCount));
			ExtraSpawnPostion = FVector(0.0f, -1 * ExtraSpawnStartPosition, 0.0f);
			for (int i = 1; i <= BulletCount; i++)
			{
				FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
				FRotator SpawnRotation = FRotator(90.0f, 0, 0);
				FActorSpawnParameters SpawnParam;
				GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition + ExtraSpawnPostion, SpawnRotation, SpawnParam);
				ExtraSpawnPostion += FVector(0.0f, BulletSpace, 0.0f);
			}
			UGameplayStatics::PlaySound2D(this, BulletFireSound);
		}*/
}

void AShootingFlight::BoostStarted() {
	
	MoveSpeed *= 2;
}

void AShootingFlight::BoostFinished() {
	
	MoveSpeed = OriginMoveSpeed;
}

void AShootingFlight::ExplosionAll() {

	// 모든 에너미를 파괴(TActorIterator)
	/*for (TActorIterator<AEnemy> itr(GetWorld()); itr; ++itr)
	{
		itr->DestroyMyself();
	}*/


	// 모든 에너미를 파괴(배열, 문제있음)
	//AShootingGameModeBase* MyGM = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	//if (MyGM != nullptr)
	//{
	//	/*for (AEnemy* enemy : MyGM->EnemyArray)
	//	{
	//		enemy->DestroyMyself();
	//	}*/

	//	for (int32 i = 0; i < MyGM->EnemyArray.Num(); i++)
	//	{
	//		// IsValid를 통해 상속여부 및 Pendingkill 상태 확인
	//		if (IsValid(MyGM->EnemyArray[i]))
	//		{
	//			MyGM->EnemyArray[i]->DestroyMyself();
	//		}
	//	}
	//	// 배열을 한 번 초기화
	//	MyGM->EnemyArray.Empty();
	//}


	// 모든 에너미를 파괴(델리게이트)
	// PlayerBomb.Broadcast();

	// 모든 에너미 이동 방향 변경(델리게이트)
	FVector NewDir = (-1 * GetActorUpVector()) + GetActorRightVector();
	OnEnemyDirModify.Broadcast(NewDir);
}

void AShootingFlight::CheckEnemies() {

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

	DrawDebugSphere(GetWorld(), CenterLocation, 700.f, 20, FColor::Yellow, false, 2.f);
	
}

// 충돌 시 색을 0.2초 동안 변경
//void AShootingFlight::ChangeColor() {
//	PlayerMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(0));
//	
//	PlayerMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
//}

void AShootingFlight::ChangeOriginColor() {
	//DynamicMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(0));

	//DynamicMaterial->SetVectorParameterValue(TEXT("Color"), InitColor);
	//// PlayerMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(255, 0, 0, 255);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), InitColor);
}

void AShootingFlight::ReserveChangeColor(float time) {
	// ChangeColor();
	// 색상을 Red 로 변경
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), (FVector4)FLinearColor::Red);

	GetWorld()->GetTimerManager().SetTimer(ColorTimer, this, &AShootingFlight::ChangeOriginColor, time, false);
}


// Boost Toggle
/*
*	void APlyerFlight::Boost()
*	{
*		IsBoosting = !IsBoosting;
* 
*		if (IsBoosting)
*		{
*			MoveSpeed = MoveSpeedOrigin * 2;
*		}
*		else
		{
			MoveSpeed = MoveSpeedOrigin;
		}

		// 삼항연산
		// MoveSpeed = IsBoosting ? MoveSpeedOrigin*2 : MoveSpeedOrigin;
*
* 
*/