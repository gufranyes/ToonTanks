// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "PawnTank.h"

// Sets default values
APawnTurret::APawnTurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    
}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	GetWorld() -> GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::HandleDestruction() 
{
	Super::HandleDestruction();
	Destroy();
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!PlayerPawn || RangeToPlayer() > FireRange)
	{
		return;
	}
	RotateTurretFunction(PlayerPawn -> GetActorLocation());
}

void APawnTurret::CheckFireCondition() 
{

	if (!PlayerPawn || !PlayerPawn -> GetIsPlayerAlive())
	{
		return;
	}
	if (RangeToPlayer() <= FireRange)
	{
		Fire();
	}
}

float APawnTurret::RangeToPlayer() 
{
	if (!PlayerPawn)
	{
		return 0.0f;
	}
	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}
