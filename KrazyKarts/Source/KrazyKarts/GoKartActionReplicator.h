// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartActionComponent.h"
#include "GoKartActionReplicator.generated.h"

USTRUCT()
struct FActionState
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FGoKartAction LastAction;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartActionReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoKartActionReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // RPCs
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SendAction(const FGoKartAction& Action);

    UPROPERTY(ReplicatedUsing = OnRep_ServerState)
    FActionState ServerState;

    UFUNCTION()
    void OnRep_ServerState();
    void AutonomousProxy_OnRep_ServerState();
    void SimulatedProxy_OnRep_ServerState();

    UPROPERTY()
    UGoKartActionComponent* ActionComponent;

    void UpdateServerState(const FGoKartAction& LastAction);
	
};
