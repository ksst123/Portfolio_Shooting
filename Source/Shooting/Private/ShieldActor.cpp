// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShootingFlight.h"
#include "Enemy.h"

// Sets default values
AShieldActor::AShieldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = BoxComponent; // SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(80.0f, 80.0f, 80.0f));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));


	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AShieldActor::BeginPlay()
{
	Super::BeginPlay();

	// 충돌시 실행할 함수 바인딩
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AShieldActor::ShieldOverlapped);
}

// Called every frame
void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// SetActorRotation(GetActorRotation() + FRotator(0.0f, 180.0f, 180.0f) * DeltaTime);
	// SetActorRotation(GetActorRotation() + FRotator(0.0f, 0.0f, 180.0f) * DeltaTime);
	AddActorWorldRotation(FRotator(0.0f, 0.0f, 180.0f) * DeltaTime);
	// AddActorWorldRotation(FRotator(60.0f, 120.0f, 180.0f) * DeltaTime);
}

void AShieldActor::ShieldOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	AShootingFlight* player = Cast<AShootingFlight>(OtherActor);
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	// 충돌 대상이 플레이어라면
	if (player != nullptr)
	{
		// 실드를 플레이어에게 부착
		// AttachToActor(player, FAttachmentTransformRules::KeepWorldTransform);
		// AttachToActor(player, FAttachmentTransformRules::KeepRelativeTransform);
		// AttachToActor(player, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		AttachToActor(player, FAttachmentTransformRules::SnapToTargetIncludingScale);
		// AttachToActor(player, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		// player->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		FTimerHandle SnapTimer;
		GetWorld()->GetTimerManager().SetTimer(SnapTimer, this, &AShieldActor::ResetAttach, 3, false);
	}
	// 충돌 대상이 에너미라면
	else if (enemy != nullptr)
	{
		// 충돌한 에너미를 파괴
		enemy->Destroy();
	}
}

void AShieldActor::ResetAttach() {

	// 부착했던 부모를 해제
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

