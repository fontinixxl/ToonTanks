// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
public:
	ATank();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction() override;
	bool bAlive = true;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float Speed = 800.0f;
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float TurnRate = 45.0f;

	void Move(float Value);
	void Turn(float Value);

	class AToonTanksPlayerController* TankPlayerController;
};
