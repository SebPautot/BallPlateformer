// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/MyPlayerController.h"
#include "GameFramework/Pawn.h"
//inputs
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
//end inputs
#include "Actors/MyBall.h"

void AMyPlayerController::BeginPlay()
{
    PlayerPawn = GetPawn();

    Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
        UE_LOG(LogTemp, Warning, TEXT("subsystem loaded"));
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        UE_LOG(LogTemp, Warning, TEXT("input function loaded"));
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::Jump);
        //Movement
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMyPlayerController::Move);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyPlayerController::Move);
    }
}

void AMyPlayerController::Move(const FInputActionValue &Value)
{
    if(AMyBall* Ball = Cast<AMyBall>(PlayerPawn))
        Ball->Move(Value.Get<FVector2D>());
}

void AMyPlayerController::Jump(const FInputActionValue &Value)
{
    if (AMyBall* Ball = Cast<AMyBall>(PlayerPawn))
        Ball->Jump();
}
