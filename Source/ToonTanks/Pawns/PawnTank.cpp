// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APawnTank::APawnTank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp -> SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp -> SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::HandleDestruction() 
{
	Super::HandleDestruction();

	bIsPlayerAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate();
	Move();

	if (PlayerControllerRef)
	{
		FHitResult TraceHitResult;
		PlayerControllerRef -> GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;
		RotateTurretFunction(HitLocation);
	}
	
}

bool APawnTank::GetIsPlayerAlive() 
{
	return bIsPlayerAlive;
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent -> BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent -> BindAxis("Turn", this, &APawnTank::CalculateRotationInput);
	PlayerInputComponent -> BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}
void APawnTank::CalculateMoveInput(float Value) 
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld() -> DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotationInput(float Value) 
{
	float RotateAmount = Value * RotateSpeed * GetWorld() -> DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation); 
}

void APawnTank::Move() 
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate() 
{
	AddActorLocalRotation(RotationDirection, true);
}
