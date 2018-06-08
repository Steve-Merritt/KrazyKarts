// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartActionComponent.generated.h"

USTRUCT()
struct FGoKartAction
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    bool AcceleratorPressed;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
    UGoKartActionComponent();

    void SetInputAcceleratorPressed();
    void SetInputAcceleratorReleased();

    FGoKartAction GetLastAction() const { return LastAction; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
    FGoKartAction CreateAction(float DeltaTime);

	bool AcceleratorPressed = false;

    FGoKartAction LastAction;
	
};
