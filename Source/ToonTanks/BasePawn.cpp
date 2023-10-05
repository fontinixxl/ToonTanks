// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "HealthComponent.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	HealthComponent = FindComponentByClass<UHealthComponent>();
	if (HealthComponent && IsValid(HealthComponent))
	{
		HealthComponent->OnActorDeathDelegate.AddDynamic(this, &ABasePawn::HandleDestruction);
	}
}

void ABasePawn::RotateTurret(const FVector& LookAtTarget) const
{
	const FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	const FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	
	TurretMesh->SetWorldRotation(FMath::RInterpTo(
		TurretMesh->GetComponentRotation(),
		LookAtRotation,
		GetWorld()->DeltaTimeSeconds,
		TurretInterpSpeed)
		);
}

void ABasePawn::Fire()
{
	const FVector SpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
	const FRotator SpawnPointRotation = ProjectileSpawnPoint->GetComponentRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnPointLocation, SpawnPointRotation, SpawnParams);
}

void ABasePawn::HandleDestruction()
{
	if (DestroyedParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyedParticle, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
	
	GameMode->HandleActorDestroyed(this);
	HealthComponent->OnActorDeathDelegate.RemoveDynamic(this, &ABasePawn::HandleDestruction);
}


