// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "ShootingGameModeBase.h"

APlayerBullet::APlayerBullet() {
	
	BoxComponent->SetCollisionProfileName(TEXT("BulletPreset"));
}

void APlayerBullet::BeginPlay() {

	// 부모의 BeginPlay()를 그대로 실행
	Super::BeginPlay();

	SetDirection(GetActorForwardVector());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnOverlap);
}

// OtherActor 로 대상 액터 정보, HitComponent 로 충돌 대상 컴포넌트 정보?
void APlayerBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// 만일 충돌 대상이 Enemy 라면
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy != nullptr)
	{
		// 적이 있던 위치에 폭발 이펙트 생성
		FVector EnemyLocation = Enemy->GetActorLocation();
		FRotator EnemyRotation = Enemy->GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, EnemyLocation, EnemyRotation, true);

		// Enemy 제거
		Enemy->Destroy();

		// 게임 모드 안에 점수 1점 추가
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
		// AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		AShootingGameModeBase* ShootingGameMode = Cast<AShootingGameModeBase>(GameMode);
		ShootingGameMode->AddScore(10);
		// UE_LOG(LogTemp, Warning, TEXT("Score: %d"), ShootingGameMode->GetCurrentScore());

		// Bullet 스스로도 제거
		Destroy();
	}
}

void APlayerBullet::SetDirection(FVector NewDirection) {

	// 이동 방향을 NewDirection으로 변경
	direction = NewDirection;
}