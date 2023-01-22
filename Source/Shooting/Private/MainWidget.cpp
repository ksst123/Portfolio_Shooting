// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "ShootingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UMainWidget::PrintCurrentScore() {

	AShootingGameModeBase* MyGameMode = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (MyGameMode != nullptr)
	{
		// 현재 점수(정수) -> FText(문자열) 형태로 변환
		FText ScoreToText = FText::AsNumber(MyGameMode->GetCurrentScore());
		// 해서 ScoreText 텍스트 블록의 값으로 설정
		ScoreText->SetText(ScoreToText);

		// ScoreAnim 애니메이션을 실행
		PlayAnimation(ScoreAnim, 0.0f, 1, EUMGSequencePlayMode::Forward);
	}
}

void UMainWidget::PrintHighScore() {
	AShootingGameModeBase* MyGameMode = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (MyGameMode != nullptr)
	{
		// 최고 점수(정수) -> FText(문자열) 형태로 변환
		FText HighScoreToText = FText::AsNumber(MyGameMode->GetHighScore());
		// 해서 HighScoreText 텍스트 블록의 값으로 설정
		HighScoreText->SetText(HighScoreToText);
	}
}
