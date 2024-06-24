// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "LevelInformationAsset.generated.h"

//==========================================================================================

USTRUCT(BlueprintType)
struct FLevelTimerInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hours;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Minutes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Seconds;
};

//==========================================================================================

USTRUCT(BlueprintType)
struct FLevelScoringInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Objective;

	UPROPERTY(VisibleAnywhere)
	float OneStarMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TwoStarsMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThreeStarsMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FourStarsMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PerfectScoreMultiplier = 1.0f;
};

//==========================================================================================

USTRUCT(BlueprintType)
struct FLevelInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UWorld> Map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Floorplan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLevelTimerInformation Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLevelScoringInformation Score;
};

//==========================================================================================

UCLASS(BlueprintType)
class STEALING_STUFF_API ULevelInformationAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLevelInformation> Levels;

public:
	UFUNCTION(BlueprintPure)
	int GetEntryIndex(FString mapName);
};

//==========================================================================================