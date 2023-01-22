// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "kismet/GameplayStatics.h"
#include "ShootingFlight.h"
#include "Components/BoxComponent.h"
#include "ShootingGameModeBase.h"

AEnemyBullet::AEnemyBullet() {
	
	BoxComponent->SetCollisionProfileName(TEXT("EnemyPreset"));
	BoxComponent->SetGenerateOverlapEvents(true);
}

void AEnemyBullet::BeginPlay() {
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	// OtherActor 가 ShootingFlight 인지 확인
	AShootingFlight* PlayerFlight = Cast<AShootingFlight>(OtherActor);

	// 충돌 대상이 플레이어라면..
	if (PlayerFlight != nullptr)
	{
		// 플레이어를 제거
		PlayerFlight->Destroy();

		// MenuWidget을 생성하는 함수를 실행
		AShootingGameModeBase* GameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode != nullptr)
		{
			GameMode->ShowMenu();
		}

		// 스스로도 제거
		Destroy();
	}
}

void AEnemyBullet::SetDirection(FVector NewDirection) {

	// 이동 방향을 NewDirection으로 변경
	direction = NewDirection;
}