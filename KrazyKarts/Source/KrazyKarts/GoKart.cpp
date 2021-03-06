// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "ROLE_None";
	case ROLE_SimulatedProxy:
		return "ROLE_SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "ROLE_AutonomousProxy";
	case ROLE_Authority:
		return "ROLE_Authority";
	default:
		return "Error";
	}
}

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = false;

	MovementComponent = CreateDefaultSubobject<UGoKartMovementComponent>(TEXT("MovementComponent"));
	MovementReplicator = CreateDefaultSubobject<UGoKartMovementReplicator>(TEXT("MovementReplicator"));

    ActionComponent = CreateDefaultSubobject<UGoKartActionComponent>(TEXT("ActionComponent"));
    ActionReplicator = CreateDefaultSubobject<UGoKartActionReplicator>(TEXT("ActionReplicator"));
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();

	// Simulate 1s of lag
	//if (HasAuthority())
	//{
	//	NetUpdateFrequency = 1;
	//}
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
}



// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
    PlayerInputComponent->BindAction("Accelerator", IE_Pressed, this, &AGoKart::AcceleratorPressed);
    PlayerInputComponent->BindAction("Accelerator", IE_Released, this, &AGoKart::AcceleratorReleased);
    PlayerInputComponent->BindAction("Brake", IE_Pressed, this, &AGoKart::BrakePressed);
    PlayerInputComponent->BindAction("Brake", IE_Released, this, &AGoKart::BrakeReleased);

    // Axis mappings
    PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

void AGoKart::MoveForward(float Value)
{
	if (MovementComponent == nullptr) return;
	MovementComponent->SetThrottle(Value);
}

void AGoKart::MoveRight(float Value)
{
	if (MovementComponent == nullptr) return;
    MovementComponent->SetSteeringThrow(Value);
}

void AGoKart::AcceleratorPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("AcceleratorPressed"));
    ActionComponent->SetInputAcceleratorPressed();
}

void AGoKart::AcceleratorReleased()
{
    UE_LOG(LogTemp, Warning, TEXT("AcceleratorReleased"));
    ActionComponent->SetInputAcceleratorReleased();
}

void AGoKart::BrakePressed()
{
    
}

void AGoKart::BrakeReleased()
{

}