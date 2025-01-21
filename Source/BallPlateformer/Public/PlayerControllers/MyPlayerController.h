// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class ABaseCharacter;

/**
 * 
 */
UCLASS()
class BALLPLATEFORMER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

	UInputMappingContext *GetDefaultMappingContext();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext *InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *JumpAction;

	void Move(const FInputActionValue &Value);

	void Jump(const FInputActionValue &Value);

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem *Subsystem;

	APawn *PlayerPawn;
};
