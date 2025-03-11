// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/GravityModifier.h"
#include "Actors/MyBall.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UGravityModifier::UGravityModifier()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGravityModifier::BeginPlay()
{
	Super::BeginPlay();

	// Set on overlap event on owner collider
	UPrimitiveComponent *overlapTrigger = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (overlapTrigger)
	{
		overlapTrigger->OnComponentBeginOverlap.AddDynamic(this, &UGravityModifier::OnOverlapBegin);
		overlapTrigger->OnComponentEndOverlap.AddDynamic(this, &UGravityModifier::OnOverlapEnd);
	}
}

// Called every frame
void UGravityModifier::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGravityModifier::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Get the ball
	AMyBall *ball = Cast<AMyBall>(OtherActor);
	if (ball)
	{
		ball->GravityMultiplier += GravityPreMultiplyAddition;
		ball->GravityMultiplier *= GravityMultiplier;
		ball->GravityMultiplier += GravityPostMultiplyAddition;
		if (SetGravityDirection)
		{
			ball->GravityDirection = GetOwner()->GetActorUpVector() * -1;
			ball->SetActorRotation(GetOwner()->GetActorRotation());
		}
	}
}

void UGravityModifier::OnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	// Get the ball
	AMyBall *ball = Cast<AMyBall>(OtherActor);
	if (ball)
	{
		ball->GravityMultiplier -= GravityPostMultiplyAddition;
		ball->GravityMultiplier /= GravityMultiplier;
		ball->GravityMultiplier -= GravityPreMultiplyAddition;
		if (ball->GravityDirection == (GetOwner()->GetActorUpVector() * -1) && SetGravityDirection)
		{
			ball->GravityDirection = FVector::ZeroVector;
			ball->SetActorRotation(ball->baseRotation);
		}
	}
}
