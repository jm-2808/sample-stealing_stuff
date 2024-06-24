// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "Stealing_Stuff/Objects/StealableObject.h"

#include "GameFramework/Actor.h"
#include "PickupZone.generated.h"

//==========================================================================================

UCLASS()
class STEALING_STUFF_API APickupZone : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collider;
	
public:	
	APickupZone();

	// AActor interface
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

	// Object tracking
public:
	TArray<AStealableObject*> GetOverlappingObjects();
	TArray<AStealableObject*> GetStealableObjects();

	void StealObject(AStealableObject* object);

private:
	TArray<AStealableObject*> OverlappingObjects;

	// Activation
public:
	UFUNCTION(BlueprintCallable)
	void SetActive(bool state) { Active = state; OnActiveChanged(); }

	UFUNCTION(BlueprintPure)
	bool IsActive() { return Active; }

	UFUNCTION(BlueprintNativeEvent)
	void OnActiveChanged();
	void OnActiveChanged_Implementation() {}

private:
	bool Active;
};

//==========================================================================================