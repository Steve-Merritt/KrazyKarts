// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKartActionComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGoKartActionComponent::UGoKartActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoKartActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGoKartActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
    {
        LastAction = CreateAction(DeltaTime);
        // process input e.g. GetOwner->ProcessInput(LastAction)
    }
}

FGoKartAction UGoKartActionComponent::CreateAction(float DeltaTime)
{
    FGoKartAction Action;
    Action.AcceleratorPressed = AcceleratorPressed;

    return Action;
}

void UGoKartActionComponent::SetInputAcceleratorPressed()
{
    AcceleratorPressed = true;
}

void UGoKartActionComponent::SetInputAcceleratorReleased()
{
    AcceleratorPressed = false;
}

