// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float TurretInterpSpeed = 15.f;
	class AToonTanksGameMode* GameMode;
	
	virtual void BeginPlay() override;
	void RotateTurret(const FVector& LookAtTarget) const;
	void Fire();

public:
	UFUNCTION()
	virtual void HandleDestruction();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Base Pawn Components", meta=(AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Base Pawn Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Base Pawn Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Base Pawn Components", meta=(AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<class AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category="Combat")
	UParticleSystem* DestroyedParticle;
	UPROPERTY(EditAnywhere, Category="Combat")
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, Category="Combat")
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
	
	class UHealthComponent* HealthComponent;

};
