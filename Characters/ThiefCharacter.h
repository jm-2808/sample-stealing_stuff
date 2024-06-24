// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Stealing_Stuff/Systems/ColourInformationAsset.h"

#include "Stealing_Stuff/Objects/StealableObject.h"

#include "Stealing_Stuff/Characters/ModularCharacter.h"
#include "ThiefCharacter.generated.h"

//==========================================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPausePressed, class AThiefCharacter*, Instigator);

//==========================================================================================

DECLARE_LOG_CATEGORY_EXTERN(StealLogThief, Display, All);

//==========================================================================================

UCLASS()
class STEALING_STUFF_API AThiefCharacter : public AModularCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RingWidgetPivot;

public:
	AThiefCharacter();

	// AActor interface
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// APawn interface
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Pause
public:
	UFUNCTION()
	void Pause() { OnPausePressed.Broadcast(this); }

	FOnPausePressed OnPausePressed;

	// Appearance
public:
	UFUNCTION(BlueprintNativeEvent)
	void SetColour(FGradientColour colour);
	void SetColour_Implementation(FGradientColour colour) {}

	// Movement
public:
	void SetDefaultMovementAxisAngle(float value);
	void SetMovementAxisAngle(float value);

protected:
	void MoveForward(float value);
	void MoveRight(float value);

	void Dash();

	UFUNCTION(BlueprintNativeEvent)
	void OnDash();
	void OnDash_Implementation() {}

	UFUNCTION(BlueprintPure)
	float GetForwardValue() { return ForwardValue; }

	UFUNCTION(BlueprintPure)
	float GetRightValue() { return RightValue; }

private:
	float GetMovementSpeed();
	float GetRotationSpeed();

private:
	const float DefaultMovementSpeed = 600.0f;
	const float DefaultRotationRate = 4.0f;
	float DefaultMovementAxisRotation;
	float MovementAxisRotation;

	const float DashImpulseForce = 2500.0f;
	const float DashCooldown = 1.0f;
	float DashCooldownTimer;

	float ForwardValue;
	float RightValue;

	// Interaction
public:
	void PickUpLeft_Pressed();
	void PickUpLeft_Released();
	void PickUpRight_Pressed();
	void PickUpRight_Released();

	void Interact();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnPickupObject();
	void OnPickupObject_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnDropObject();
	void OnDropObject_Implementation() {}

	UFUNCTION(BlueprintPure)
	bool IsHoldingObject() { return (HeldObject != nullptr); }

private:
	void PickUpObject();
	void DropObject();

	void UpdatePlayerOnPickup(FHitResult hit);
	void UpdatePlayerOnDrop();

	AStealableObject* FindStealableObject(FHitResult& hit);
	bool LineTrace(FVector start, FVector direction, float length, FHitResult& hit, bool debugLine = false);

private:
	bool LeftHold;
	bool RightHold;

	UPROPERTY()
	AStealableObject* HeldObject;

	float PickupDistance = 80.0f;

	// Emotes
public:
	void Emote();
};

//==========================================================================================