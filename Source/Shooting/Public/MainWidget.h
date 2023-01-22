// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player UI|Score", meta=(BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player UI|Score", meta=(BindWidget))
	class UTextBlock* HighScoreText;

	UPROPERTY(EditAnywhere, Category = "Player UI|Score", meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ScoreAnim;

	void PrintCurrentScore();
	void PrintHighScore();
};
