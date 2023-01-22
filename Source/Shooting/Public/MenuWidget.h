// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* ButtonResume;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* ButtonRestart;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* ButtonQuit;

private:
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitGame();
};
