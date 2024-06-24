// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"

#include "Engine/DataAsset.h"
#include "CharacterInformationAsset.generated.h"

//==========================================================================================

UCLASS(BlueprintType)
class STEALING_STUFF_API UCharacterInformationAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, UStaticMesh*> Hats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, USkeletalMesh*> Heads;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, USkeletalMesh*> Torsos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, USkeletalMesh*> Hands;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, USkeletalMesh*> Waists;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, USkeletalMesh*> Legs;
	
};

//==========================================================================================