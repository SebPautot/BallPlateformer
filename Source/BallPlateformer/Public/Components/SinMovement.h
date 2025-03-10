// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SinMovement.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BALLPLATEFORMER_API USinMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USinMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FBodyInstance *BodyInstance;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinMovement")
	float Frequency = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinMovement")
	FVector EndOffset = FVector(0, 0, 0);

private:
	FVector StartLocation;
	float RunningTime;
	float prevSinVal = 0;
	float sinVal = 0;
};
