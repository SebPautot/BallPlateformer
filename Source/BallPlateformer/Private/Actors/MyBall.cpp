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
		MeshComponent->SetStaticMesh(SphereMesh.Object);
	}
	BodyInstance = MeshComponent->GetBodyInstance();
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);

	// enable hit event generation
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &AMyBall::OnHit);
	MeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	// Disable rotation
	MeshComponent->BodyInstance.bLockXRotation = true;
	MeshComponent->BodyInstance.bLockYRotation = true;
	MeshComponent->BodyInstance.bLockZRotation = true;

	// Remove resistance from physics material
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

	if(!isGrounded)
	{
		GroundedActor = nullptr;
	}else if(GroundedActor != nullptr){
		FVector NewLocation = GetActorLocation() - GroundedActorPreviousLocation; // The currentLoc - A part of the equation | pretty way of saying NewLoc = CurrentLoc + (B - A)
		GroundedActorPreviousLocation = GroundedActor->GetActorLocation(); // A becomes B
		NewLocation += GroundedActorPreviousLocation; // + B
		SetActorLocation(NewLocation); // NewLoc = CurrentLoc + (B - A)
	}

	// getting posseslime vibes from this code

	float currentSpeed = isGrounded ? GroundSpeed : AirSpeed;
	float maxSpeed = isGrounded ? maxGroundSpeed : maxAirSpeed;
	float desceleration = isGrounded ? groundDesceleration : airDesceleration;
	bool isUsingMaxSpeed = isGrounded ? isUsingMaxGroundSpeed : isUsingMaxAirSpeed;

	FVector velocity = GetVelocity();
	FVector velocityDirection = GetVelocity().GetSafeNormal();
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
	GravityDirection.Normalize();
	FVector gravity = GravityDirection * Gravity * GravityMultiplier * Mass * 100.f; // no deltatime because it's already applied in the physics engine through addforce

	BodyInstance->AddForce(gravity);

	wasGrounded = isGrounded;
	wasOnWall = isOnWall;
	wasOnCeiling = isOnCeiling;
	// physics variables reset to their OG value so they can be used and filled-in in the next frame by OnHit
	isGrounded = false;
	isOnWall = false;
	isOnCeiling = false;
}

// Called to bind functionality to input
void AMyBall::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyBall::Move(const FVector2D &MovementVector)
{
	inputVector = FVector(MovementVector.X, MovementVector.Y, 0);
	inputVector.Normalize();
}

void AMyBall::Jump()
{

	if (isGrounded)
	{
		FVector jumpImpulse = FVector(0, 0, (/* sqrt(-2.f * gravity * jumpheight)*/ FMath::Sqrt(FMath::Abs(2.f * GetWorld()->GetGravityZ() * JumpHeight * Mass))));
		BodyInstance->AddImpulse(jumpImpulse, true);
		isGrounded = false;
	}
	else if (isOnWall)
	{
		FVector jumpImpulse = FVector(0, 0, (/* sqrt(-2.f * gravity * jumpheight)*/ FMath::Sqrt(FMath::Abs(2.f * GetWorld()->GetGravityZ() * WallJumpHeight * Mass))));
		jumpImpulse += wallNormal * WallJumpForce;
		BodyInstance->AddImpulse(jumpImpulse, true);
		isOnWall = false;
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
	// dot product to know if we are grounded, on a wall or on a ceiling
	FVector hitNormal = Hit.Normal;
	FVector upVector = GetActorUpVector();
	if (FVector::DotProduct(hitNormal, upVector) > 0.9f)
	{
		isGrounded = true;
		hasDoubleJumped = false;

		if(!wasGrounded)
		{
			GroundedActor = OtherActor;
			GroundedActorPreviousLocation = OtherActor->GetActorLocation();
		}
	}
	else if (FVector::DotProduct(hitNormal, upVector * -1) > 0.9f)
	{
		isOnCeiling = true;
	}
	else
	{
		isOnWall = true;
		wallNormal = hitNormal;
	}
}