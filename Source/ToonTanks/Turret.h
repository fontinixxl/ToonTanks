// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Turret.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATurret : public ABasePawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	// Check if thank is withing distance and on sight (no colliders in between)
	bool IsTargetInRange() const;

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void HandleDestruction() override;

private:
	UPROPERTY(EditDefaultsOnly, Category= "Turret", meta=(AllowPrivateAccess = "true"))
	float FireRange = 500.f;
	class ATank* Tank;

	FTimerHandle FireRateTimerHandle;
	float FireRate = 2.f;
	void CheckFireCondition();
};
