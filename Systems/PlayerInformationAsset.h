// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "ColourInformationAsset.h"
#include "Stealing_Stuff/Characters/ModularCharacter.h"
#include "Stealing_Stuff/UserInterface/HeistHUDWidget.h"
#include "Stealing_Stuff/UserInterface/PauseMenuWidget.h"

#include "Engine/DataAsset.h"
#include "PlayerInformationAsset.generated.h"

//==========================================================================================

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGradientColour Colour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterModelInfo Model;
};

//==========================================================================================

UCLASS(BlueprintType)
class STEALING_STUFF_API UPlayerInformationAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPlayerInfo> Players;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LastCompletedLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Score;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UHeistHUDWidget> GameUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPauseMenuWidget> PauseUI;
};

//==========================================================================================