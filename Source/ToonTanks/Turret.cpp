// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "Tank.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	// Register turret to Game Mode for handling Game Over condition
	GameMode->RegisterNewTower(this);
	
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
}

void ATurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (Tank && IsTargetInRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATurret::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}
	
	if (IsTargetInRange() && Tank->bAlive)
	{
		Fire();
	}
}

bool ATurret::IsTargetInRange() const
{
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = Tank->GetActorLocation();
	const float Distance = FVector::Dist(StartLocation, EndLocation);

	if (Distance > FireRange) return false;
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the turret itself during the trace
	CollisionParams.AddIgnoredActor(Tank); // Ignore the tank during the trace

	// Perform the line trace 
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
		);

#if WITH_EDITOR
	if (GIsEditor && GIsPlayInEditorWorld)
	{
		// Draw debug line
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, -1.f, 0, 1.f);
	}
#endif
	
	return !bHit ? true : HitResult.GetActor() == Tank;
}
