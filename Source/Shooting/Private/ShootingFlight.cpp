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

	// �浹 �ڽ� ������Ʈ ����
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// ������ �浹 �ڽ� ������Ʈ�� ��Ʈ ������Ʈ�� ����
	SetRootComponent(BoxComponent);
	// �ڽ� �ݸ��� ũ�⸦ (50, 50, 50) ���� ����
	BoxComponent->SetBoxExtent(FVector(50));
	// �ڽ� �ݸ����� �浹 ó�� �������� PlayerPreset ���� ����
	BoxComponent->SetCollisionProfileName(TEXT("PlayerPreset"));


	// �޽� ������Ʈ ����
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// ������ �޽� ������Ʈ�� ��Ʈ ������Ʈ�� ���� ��ü�� ���
	MeshComponent->SetupAttachment(RootComponent);
	// Shape_Cube ������ ã�Ƽ� ������ ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// ���� ť�� ������ �ε��ϴµ� �����ߴٸ�
	if (CubeStaticMesh.Succeeded())
	{
		// �ε��� ������ �޽� ������Ʈ�� Static Mesh �׸� �Ҵ�
		MeshComponent->SetStaticMesh(CubeStaticMesh.Object);
	}
}

// Called when the game starts or when spawned
void AShootingFlight::BeginPlay()
{
	Super::BeginPlay();
	OriginMoveSpeed = MoveSpeed;
	
	// �÷��̾� ��Ʈ�ѷ��� ĳ����
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

	// direction ���� ����ȭ
	// ����ڰ� �Է��� ������ �̵�
	direction.Normalize();
	FVector dir = GetActorLocation() + (direction * MoveSpeed) * DeltaTime;
	// bSweep = true, �̵� �� �浹 ü�� �ִ��� Sweep(���� ����) ���� true ����
	SetActorLocation(dir, true);

}

// Called to bind functionality to input
void AShootingFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������ UInputComponent* ������ UEnhancedInputComponent �� ��ȯ
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// �׼� �Լ��� ����
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
		// �Ѿ��� ��ü ����
		float TotalSize = (BulletCount - 1) * BulletSpace;

		// ���� ��ġ
		float BaseY = TotalSize * -0.5f;

		// ���� ��ġ�� ������ ���� ����
		FVector Offset = FVector(0.0f, BaseY + BulletSpace * i, 0.0f);


		// Bullet ���� ��ġ�� ���� ���� ����
		FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
		SpawnPosition += Offset;
		// Bullet ���� �� ȸ������ ���� ���� ����
		FRotator SpawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters SpawnParam;
		// ������Ʈ ���� �� ���� ��ġ�� �浹 ������ �ٸ� ������Ʈ�� ���� ��쿡 ��� �� ������ �����ϴ� �ɼ�
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Bullet�� ����
		// Bullet Blueprint ����
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);


		// ������ Bullet �ν��Ͻ��� BulletAngle ��ŭ �����ϰ� ȸ����Ų��.
		float ExtraBulletTotalAngle = -0.5 * BulletAngle * (BulletCount - 1);
		FRotator OffsetAngle = FRotator(0.0f, 0.0f, ExtraBulletTotalAngle + BulletAngle * i);
		// Bullet->SetActorRelativeRotation(GetActorRotation() + OffsetAngle);
		if (Bullet != nullptr)
		{
			Bullet->AddActorWorldRotation(OffsetAngle);
		}
	}
	// Bullet �߻� ȿ���� ����
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
	 // ��� ���ʹ̸� �ı�(��������Ʈ)
	 PlayerBomb.Broadcast();
}

void AShootingFlight::CheckEnemies()
{
	// �ݰ� 5���� �̳��� �ִ� ��� AEnemy ���͵��� ����
	// ������ ���ʹ̵��� ������ ���� ������ �迭
	TArray<FOverlapResult> EnemiesInfo;
	FVector CenterLocation = GetActorLocation() + GetActorUpVector() * 700.f;
	FQuat CenterRotation = GetActorRotation().Quaternion(); // GetActorQuat();
	FCollisionObjectQueryParams params = ECC_GameTraceChannel2;
	FCollisionShape CheckShape = FCollisionShape::MakeSphere(500.f);

	GetWorld()->OverlapMultiByObjectType(EnemiesInfo, CenterLocation, CenterRotation, params, CheckShape);

	// üũ�� ��� ���ʹ��� �̸��� ���
	for (FOverlapResult EnemyInfo : EnemiesInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitted: %s"), *EnemyInfo.GetActor()->GetName());

		EnemyInfo.GetActor()->Destroy();
	}
}