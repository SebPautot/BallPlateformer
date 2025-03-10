// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SinMovement.h"

// Sets default values for this component's properties
USinMovement::USinMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void USinMovement::BeginPlay()
{
	Super::BeginPlay();

	// Get body instance from owner
	BodyInstance = GetOwner()->FindComponentByClass<UStaticMeshComponent>()->GetBodyInstance();
	StartLocation = GetOwner()->GetActorLocation();
	RunningTime = 0.f;
}


// Called every frame
void USinMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RunningTime += DeltaTime;
	// FVector NewLocation = StartLocation + EndOffset * FMath::Sin(Frequency * RunningTime);
	//Let's do it with addforce instead
	//cancel addforce

	sinVal = FMath::Sin(Frequency * RunningTime);
	BodyInstance->AddForce(GetOwner()->GetVelocity() * -1 * 1000.f / (BodyInstance->GetBodyMass() * DeltaTime));
	BodyInstance->AddImpulse(EndOffset * (sinVal - prevSinVal) * 1000.f, false);
	prevSinVal = sinVal;
}

