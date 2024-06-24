// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Stealing_Stuff/Characters/ModularCharacter.h"
#include "NPCCharacter.generated.h"

//==========================================================================================

UCLASS()
class STEALING_STUFF_API ANPCCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();

	// AActor interface
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// APawn interface
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

//==========================================================================================