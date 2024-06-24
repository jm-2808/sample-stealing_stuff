// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Stealing_Stuff/Systems/CharacterInformationAsset.h"

#include "GameFramework/Character.h"
#include "ModularCharacter.generated.h"

//==========================================================================================

USTRUCT(BlueprintType)
struct FCharacterModelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Hat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Torso;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Hands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Waist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Legs;
};

//==========================================================================================

DECLARE_LOG_CATEGORY_EXTERN(StealLogCharacter, Display, All);

//==========================================================================================

UCLASS()
class STEALING_STUFF_API AModularCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Skeleton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Hat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Torso;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Hands;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Waist;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Legs;

public:
	AModularCharacter();

	// AActor interface
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// APawn interface
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Appearance
public:
	UFUNCTION(BlueprintCallable)
	void SetModel(FCharacterModelInfo modelInfo);

private:
	UPROPERTY()
	UCharacterInformationAsset* CharacterInformation;
	const FString CharacterInformationAssetLocation = "/Game/Blueprints/CharacterInfo";

};

//==========================================================================================