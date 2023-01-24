// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ShootingFlight.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FULTbomb);

// 벡터를 인자로 넘겨받는 델리게이트 선언(FDirectionModifier)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDirectionModifier, FVector, NewDirection);


UCLASS()
class SHOOTING_API AShootingFlight : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShootingFlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player Settings")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player Settings")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Settings")
	float MoveSpeed = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Settings")
	TSubclassOf<class ABullet> BulletFactory;
	// 블루프린트, 머티리얼, 외부에서 만들어진 파일들은 TSubclassOf 통해서 참조

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_MoveHorizontal;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_MoveVertical;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_FireBullet;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_Boost;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_ULT;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputMappingContext* IMC_MyMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class USoundBase* BulletFireSound;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
	int32 BulletCount = 1;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
	float BulletSpace = 150;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
	float BulletAngle = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	bool IsTrapped = false;

	void ChangeOriginColor();
	void ReserveChangeColor(float time);

	FULTbomb PlayerBomb;
	FDirectionModifier OnEnemyDirModify;

private:
	UFUNCTION(BlueprintCallable)
	void MoveHorizontal(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void MoveVertical(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void FireBullet();

	UFUNCTION(BlueprintCallable)
	void BoostStarted();

	UFUNCTION(BlueprintCallable)
	void BoostFinished();

	UFUNCTION(BlueprintCallable)
	void ExplosionAll();

	void CheckEnemies();

	float h, v;
	FVector direction;
	float OriginMoveSpeed;
};
