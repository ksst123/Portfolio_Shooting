// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct() {
	
	ButtonResume->OnClicked.AddDynamic(this, &UMenuWidget::ResumeGame);
	ButtonRestart->OnClicked.AddDynamic(this, &UMenuWidget::RestartGame);
	ButtonQuit->OnClicked.AddDynamic(this, &UMenuWidget::QuitGame);
}

void UMenuWidget::ResumeGame() {

	// 게임 일시정지 해제
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	// 마우스 커서 화면 출력 해제
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	// AddToViewport의 반대 함수
	this->RemoveFromParent();
}

void UMenuWidget::RestartGame() {

	// 레벨을 다시 로드
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LV_ShootingMap"));

	// 현재 위젯을 제거
	this->RemoveFromParent();

	// 마우스 커서 화면 출력 해제
	// GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);


}

void UMenuWidget::QuitGame() {

	// 앱을 종료
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
