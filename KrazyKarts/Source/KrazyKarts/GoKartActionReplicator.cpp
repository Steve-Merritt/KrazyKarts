// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKartActionReplicator.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

FString GetOwnerRoleText(ENetRole Role)
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

// Sets default values for this component's properties
UGoKartActionReplicator::UGoKartActionReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    //SetIsReplicated(true);
}


// Called when the game starts
void UGoKartActionReplicator::BeginPlay()
{
	Super::BeginPlay();

    ActionComponent = GetOwner()->FindComponentByClass<UGoKartActionComponent>();
}


// Called every frame
void UGoKartActionReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (ActionComponent == nullptr) return;

    FGoKartAction LastAction = ActionComponent->GetLastAction();

    // If we are a client and in control of the pawn...
    if (GetOwnerRole() == ROLE_AutonomousProxy)
    {
        // Send the move to the server from the autonomous proxy (local client, non-host).
        Server_SendAction(LastAction);
    }

    // If we are the server and in control of the pawn...
    if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
    {
        UpdateServerState(LastAction);
    }

    DrawDebugString(GetWorld(), FVector(0, 0, 120), GetOwnerRoleText(GetOwner()->GetRemoteRole()), GetOwner(), FColor::Blue, DeltaTime);
}

void UGoKartActionReplicator::Server_SendAction_Implementation(const FGoKartAction& Action)
{
    // Simulate on the server with Action data from remote clients.
    DrawDebugString(
        GetWorld(), FVector(0, 0, 150),
        FString::Printf(TEXT("SendAction_Impl %d"), Action.AcceleratorPressed ? 1 : 0),
        GetOwner(), FColor::Orange, 10.f);

    // Send the canonical state to the clients.
    UpdateServerState(Action);
}

bool UGoKartActionReplicator::Server_SendAction_Validate(const FGoKartAction& Action)
{
    return true;
}

void UGoKartActionReplicator::UpdateServerState(const FGoKartAction& LastAction)
{
    // Send the canonical state to the clients.
    ServerState.LastAction = LastAction;
}

void UGoKartActionReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UGoKartActionReplicator, ServerState);
}

void UGoKartActionReplicator::OnRep_ServerState()
{
    switch (GetOwnerRole())
    {
    case ROLE_AutonomousProxy:  // local client controlled
        AutonomousProxy_OnRep_ServerState();
        break;
    case ROLE_SimulatedProxy:   // remote client controlled
        SimulatedProxy_OnRep_ServerState();
        break;
    default:
        break;
    }
}

void UGoKartActionReplicator::AutonomousProxy_OnRep_ServerState()
{
    DrawDebugString(
        GetWorld(), FVector(0, 0, 140),
        FString::Printf(TEXT("AutonomousProxy_OnRep_ServerState %d"), ServerState.LastAction.AcceleratorPressed ? 1 : 0),
        GetOwner(), FColor::Cyan, 10.f);
}

void UGoKartActionReplicator::SimulatedProxy_OnRep_ServerState()
{
    DrawDebugString(
        GetWorld(), FVector(0, 0, 140),
        FString::Printf(TEXT("SimulatedProxy_OnRep_ServerState %d"), ServerState.LastAction.AcceleratorPressed ? 1 : 0),
        GetOwner(), FColor::Cyan, 10.f);
}

