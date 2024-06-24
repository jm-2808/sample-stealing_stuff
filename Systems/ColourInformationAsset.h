// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "ColourInformationAsset.generated.h"

//==========================================================================================

USTRUCT(BlueprintType)
struct FGradientColour
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> Points;
};

//==========================================================================================

UCLASS(BlueprintType)
class STEALING_STUFF_API UColourInformationAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGradientColour> Colours;

public:
	UFUNCTION(BlueprintCallable)
	FGradientColour GetDefaultColour();

	UFUNCTION(BlueprintCallable)
	FGradientColour GetColourByName(FName name);

	UFUNCTION(BlueprintCallable)
	FGradientColour GetColourByIndex(int index);
};

//==========================================================================================