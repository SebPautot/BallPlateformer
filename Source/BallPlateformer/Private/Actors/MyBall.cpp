// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/MyBall.h"
#include "Components/SphereComponent.h"

// Sets default values
AMyBall::AMyBall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMesh.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("SphereMesh loaded"));
		MeshComponent->SetStaticMesh(SphereMesh.Object);
	}
	BodyInstance = MeshComponent->GetBodyInstance();
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);

	// enable hit event generation
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &AMyBall::OnHit);

	// Disable rotation
	MeshComponent->BodyInstance.bLockXRotation = true;
	MeshComponent->BodyInstance.bLockYRotation = true;
	MeshComponent->BodyInstance.bLockZRotation = true;

	//Remove resistance from physics material
	MeshComponent->SetLinearDamping(0.0f);
	MeshComponent->SetAngularDamping(0.0f);

	// set gravity
	MeshComponent->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void AMyBall::BeginPlay()
{
	Super::BeginPlay();
	// set mass to 1
	MeshComponent->SetMassOverrideInKg(NAME_None, 1.0f, true);
}

// Called every frame
void AMyBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//getting posseslime vibes from this code

	float currentSpeed = isGrounded ? GroundSpeed : AirSpeed;
	float maxSpeed = isGrounded ? maxGroundSpeed : maxAirSpeed;
	float desceleration = isGrounded ? groundDesceleration : airDesceleration;
	bool isUsingMaxSpeed = isGrounded ? isUsingMaxGroundSpeed : isUsingMaxAirSpeed;

	FVector velocityDirection = GetVelocity().GetSafeNormal();
	FVector velocity = GetVelocity();
	velocity.Z = 0;

	// apply drag
	FVector drag = velocityDirection * desceleration;
	FVector newVelocity = velocity - drag * DeltaTime;
	
	// evaluate new velocity, to know if velocity changed direction due to drag
	if (FVector::DotProduct(velocity, newVelocity) < 0)
	{
		newVelocity = FVector::ZeroVector;
	}

	newVelocity.Z = GetVelocity().Z;

	// set new velocity after drag posseslime vibes
	BodyInstance->SetLinearVelocity(newVelocity, false);

	// apply movement
	BodyInstance->AddImpulse(inputVector * currentSpeed * DeltaTime, false);

	// clamp velocity posseslime vibes
	if (isUsingMaxSpeed)
	{
		FVector clampedVelocity = GetVelocity();
		clampedVelocity.Z = 0;
		clampedVelocity = clampedVelocity.GetClampedToMaxSize(maxSpeed);
		clampedVelocity.Z = GetVelocity().Z;
		BodyInstance->SetLinearVelocity(clampedVelocity, false);
	}

	// add gravity still posseslime vibes
	FVector gravity = FVector(0, 0, GetWorld()->GetGravityZ() * GravityMultiplier);
	BodyInstance->AddForce(gravity);
}

// Called to bind functionality to input
void AMyBall::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyBall::Move(const FVector2D &MovementVector)
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));
	UE_LOG(LogTemp, Warning, TEXT("MovementVector: %s"), *MovementVector.ToString());

	inputVector = FVector(MovementVector.X, MovementVector.Y, 0);
	inputVector.Normalize();
}

void AMyBall::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));

	if (isGrounded)
	{
		FVector jumpImpulse = FVector(0, 0, (/* sqrt(-2.f * gravity * jumpheight)*/ FMath::Sqrt(FMath::Abs(2.f * GetWorld()->GetGravityZ() * JumpHeight * Mass))));
		BodyInstance->AddImpulse(jumpImpulse, true);
		isGrounded = false;
	}
	else if (!hasDoubleJumped)
	{
		FVector jumpImpulse = FVector(0, 0, (/* sqrt(-2.f * gravity * jumpheight)*/ FMath::Sqrt(FMath::Abs(2.f * GetWorld()->GetGravityZ() * DoubleJumpHeight * Mass))));
		BodyInstance->AddImpulse(jumpImpulse, true);
		hasDoubleJumped = true;
	}
}

void AMyBall::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	// dot product to know if we are grounded
	FVector hitNormal = Hit.Normal;
	FVector upVector = GetActorUpVector();
	if (FVector::DotProduct(hitNormal, upVector) > 0.9f)
	{
		isGrounded = true;
		hasDoubleJumped = false;
	}
}
