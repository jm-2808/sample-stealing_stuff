// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/Actor.h"
#include "StealableObject.generated.h"

//==========================================================================================

DECLARE_LOG_CATEGORY_EXTERN(StealLogObject, Display, All);

//==========================================================================================

UCLASS()
class STEALING_STUFF_API AStealableObject : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collider;

public:
	AStealableObject();

	// AActor interface
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Collider
public:
	UFUNCTION(BlueprintPure)
	UBoxComponent* GetCollider() { return Collider; }

	// Interaction
public:
	virtual void PickUp(AActor* interactingActor);
	UFUNCTION(BlueprintNativeEvent)
	void OnPickUp();
	void OnPickUp_Implementation() {}

	virtual void Drop(AActor* interactingActor);
	UFUNCTION(BlueprintNativeEvent)
	void OnDrop();
	void OnDrop_Implementation() {}

	bool IsHeld();
	bool CanBePickedUp();

private:
	int GetInteractorIndexByActor(AActor* interactingActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StealableObject|Interaction")
	int MaximumInteractingPlayers = 1;

private:
	struct InteractionInfo
	{
		AActor* Actor;

		InteractionInfo()
		{
		}
	};
	TArray<InteractionInfo> InteractingActors;

	const float LiftOffset = 7.0f;

	// Movement
public:
	void AddMovementInput(FVector direction, float value);
	void AddRotationInput(FRotator rotation);

	bool CanRotateAroundPlayer() { return ((InteractingActors.Num() == 1) && AllowRotateAroundPlayer); }

private:
	void UpdateMovement(float deltaTime);
	void UpdateRotation(float deltaTime);

	void AttachInteractingActors();
	void DetachInteractingActors();

	FVector GetPivotPosition();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StealableObject|Movement")
	bool AllowRotateAroundPlayer = true;

private:
	TArray<FVector> MovementInput;
	TArray<FRotator> RotationInput;

	// Physical Properties
public:
	UFUNCTION(BlueprintPure)
	float GetWeight() { return Weight; };

	UFUNCTION(BlueprintPure)
	float GetMovementSpeedMultiplier();

	UFUNCTION(BlueprintPure)
	float GetRotationSpeedMultiplier();

private:
	float GetWeightAlpha() { return (Weight - MinWeight) / (MaxWeight - MinWeight); };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StealableObject|Physical Properties", meta=(ClampMin="1000.0", ClampMax="50000.0"))
	float Weight = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="StealableObject|Physical Properties")
	bool DefaultEnablePhysics = true;

private:
	const float MaxWeight = 50000.0f;
	const float MinWeight = 1000.0f;

	const float MaxMoveMultiplier = 0.1f;
	const float MinMoveMultiplier = 1.0f;
	const float MaxRotateMultiplier = 0.3f;
	const float MinRotateMultiplier = 1.0f;

	// Scoring
public:
	UFUNCTION(BlueprintPure)
	float GetMonetaryValue() { return MonetaryValue; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StealableObject|Scoring")
	float MonetaryValue;

};

//==========================================================================================