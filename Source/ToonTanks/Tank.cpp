// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "ToonTanksPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<AToonTanksPlayerController>(GetController());
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult);

		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	TankPlayerController->SetPlayerEnabledState(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::Move(const float Value)
{
	// UE_LOG(LogTemp, Display, TEXT("Input Value: %f"), Value);
	FVector DeltaLocation = FVector::ZeroVector;
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	DeltaLocation.X = Value * Speed * DeltaTime;
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotator = FRotator::ZeroRotator;
	DeltaRotator.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotator, true);
}