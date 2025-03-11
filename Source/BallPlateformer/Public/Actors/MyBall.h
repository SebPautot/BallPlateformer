// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyBall.generated.h"

class UStaticMeshComponent;

UCLASS()
class BALLPLATEFORMER_API AMyBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* MeshComponent;
	FBodyInstance* BodyInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Move(const FVector2D& MovementVector);
	
	UFUNCTION()
	void Jump();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Ball|Gravity")
	float Gravity = 9.8f;
	UPROPERTY(EditAnywhere, Category = "Ball|Gravity")
	float GravityMultiplier = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Ball|Gravity")
	FVector GravityDirection = FVector(0, 0, -1);

	UPROPERTY(EditAnywhere, Category = "Ball|Ground Movement")
	float GroundSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Ball|Ground Movement")
	bool isUsingMaxGroundSpeed = true;
	UPROPERTY(EditAnywhere, Category = "Ball|Ground Movement")
	float maxGroundSpeed = 25.f;
	UPROPERTY(EditAnywhere, Category = "Ball|Ground Movement")
	float groundDesceleration = 10.f;

	UPROPERTY(EditAnywhere, Category = "Ball|Air Movement")
	float AirSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Ball|Air Movement")
	bool isUsingMaxAirSpeed = true;
	UPROPERTY(EditAnywhere, Category = "Ball|Air Movement")
	float maxAirSpeed = 25.f;
	UPROPERTY(EditAnywhere, Category = "Ball|Air Movement")
	float airDesceleration = 10.f;

	UPROPERTY(EditAnywhere, Category = "Ball|Jump")
	float JumpHeight = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Ball|Jump")
	float DoubleJumpHeight = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Ball|Wall Jump")
	float WallJumpForce = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Ball|Wall Jump")
	float WallJumpHeight = 1000.0f;

	FRotator baseRotation;
private:
	FVector MovementDirection = FVector::ZeroVector;
	bool IsMoving = false;
	bool wasMoving = false;
	float h = 0;
	float v = 0;
	bool isGrounded = false;
	bool wasGrounded = false;
	AActor* GroundedActor = nullptr;
	FVector GroundedActorPreviousLocation = FVector::ZeroVector;
	bool isOnWall = false;
	bool wasOnWall = false;
	FVector wallNormal = FVector::ZeroVector;
	bool isOnCeiling = false;
	bool wasOnCeiling = false;
	bool hasDoubleJumped = false;
	float Mass = 1.0f;
	FVector inputVector = FVector::ZeroVector;
	AActor* GroundActor = nullptr;
	FVector initialGravityDirection = FVector::ZeroVector;
};
