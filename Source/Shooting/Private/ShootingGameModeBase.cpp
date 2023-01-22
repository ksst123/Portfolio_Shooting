// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShootingGameModeBase.h"
#include "MainWidget.h"
#include "MenuWidget.h"
#include "kismet/GameplayStatics.h"
#include "BossEnemy.h"
#include "EngineUtils.h"
#include "EnemySpawner.h"

void AShootingGameModeBase::BeginPlay() {
	
	// 위젯 블루프린트를 생성
	MainUI = CreateWidget<UMainWidget>(GetWorld(), MainWidget);

	if (MainUI != nullptr)
	{
		// 생성된 위젯 블루프린트를 뷰포트에 그린다.
		MainUI->AddToViewport();
		// 현재 점수를 위젯의 ScoreText 텍스트 블록에 출력
		MainUI->PrintCurrentScore();
		// 최고 점수를 위젯의 HighScoreText 텍스트 블록에 출력
		MainUI->PrintHighScore();

		// 최고 점수를 파일에서 읽어온다.
		FString LoadScoreText;
		bool isLoadSuccess = FFileHelper::LoadFileToString(LoadScoreText, *SavePath);
		if (isLoadSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Success"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed"));
		}
		//읽어온 값을 HighScore 변수에 대입
		HighScore = FCString::Atoi(*LoadScoreText);
		MainUI->PrintHighScore();
	}

	// FString TestPath = FPaths::GetProjectFilePath();
	// FString TestPath = FPaths::EngineContentDir();
	// FString TestPath = FPaths::GetGameLocalizationPaths();
	// FString TextPath = FPaths::Combine("http://www.naver.com", "akmfldv/asdklmv");
	FString TestPath = FPaths::ProjectContentDir() + TEXT("SaveScore/HighScore.txt");
	UE_LOG(LogTemp, Warning, TEXT("Test Path : %s"), *TestPath);

	// 스크린에 Log 출력
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, TEXT("Log Test"), true);
}

void AShootingGameModeBase::AddScore(int32 point) {
	
	CurrentScore += point;

	// 만일 누적된 CurrentScore가 HighScore 보다 크다면
	if (MainUI != nullptr && CurrentScore > HighScore)
	{
		// HighScore에 CurrentScore를 대입
		HighScore = CurrentScore;
		MainUI->PrintHighScore();
	
		// HighScore를 파일로 저장
		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(HighScore), *SavePath);
		UE_LOG(LogTemp, Warning, TEXT("%s"), isSaved ? TEXT("True") : TEXT("False"));
	}
	
	// 현재 점수가 30점 이상이고 보스가 아직 스폰되지 않았으면
	if (CurrentScore >= 30 && !bIsAppearBoss)
	{
		bIsAppearBoss = true;

		// 4초 뒤에 보스 생성
		FTimerHandle BossSpawnTimer;
		GetWorld()->GetTimerManager().SetTimer(BossSpawnTimer, this, &AShootingGameModeBase::SpawnBoss, 4, false);

		// 모든 에너미 스폰을 중단
		StopAllEnemySpawn();
	}

	// 현재 점수를 위젯의 ScoreText 텍스트 블록에 반영
	if (MainUI != nullptr)
	{
		MainUI->PrintCurrentScore();
	}
}

void AShootingGameModeBase::ShowMenu() {

	// 메뉴 위젯을 화면에 출력
	MenuUI = CreateWidget<UMenuWidget>(GetWorld(), MenuWidget);

	if (MenuUI != nullptr)
	{
		MenuUI->AddToViewport();
	}

	// 게임 일시정지
	// 1. 직접 게임의 시간 흐름 비율을 0으로 만든다.
	// UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
	// 2. 일시정지 함수 호출
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// 마우스 커서 화면에 출력
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void AShootingGameModeBase::SpawnBoss() {

	GetWorld()->SpawnActor<ABossEnemy>(BossSpawner, FVector(0.0f, 0.0f, 700.0f), FRotator::ZeroRotator);
}

void AShootingGameModeBase::StopAllEnemySpawn() {

	for (TActorIterator<AEnemySpawner> spawner(GetWorld()); spawner; ++spawner)
	{
		spawner->IsSpawn = false;
	}
}
