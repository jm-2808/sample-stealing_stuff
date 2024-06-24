// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "PickupZone.h"

//==========================================================================================

APickupZone::APickupZone()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

//==========================================================================================

void APickupZone::BeginPlay()
{
	Super::BeginPlay();

	Active = false;
}

//==========================================================================================

void APickupZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//==========================================================================================

void APickupZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AStealableObject* object = Cast<AStealableObject>(OtherActor);
	if (object != nullptr)
	{
		OverlappingObjects.Add(object);
	}
}

//==========================================================================================

void APickupZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	AStealableObject* object = Cast<AStealableObject>(OtherActor);
	if (object != nullptr)
	{
		if (OverlappingObjects.Contains(object))
		{
			OverlappingObjects.Remove(object);
		}
	}
}

//==========================================================================================

TArray<AStealableObject*> APickupZone::GetOverlappingObjects()
{
	return OverlappingObjects;
}

//==========================================================================================

TArray<AStealableObject*> APickupZone::GetStealableObjects()
{
	TArray<AStealableObject*> stealable;
	for (int i = 0; i < OverlappingObjects.Num(); ++i)
	{
		if (!(OverlappingObjects[i]->IsHeld()))
		{
			stealable.Add(OverlappingObjects[i]);
		}
	}
	return stealable;
}

//==========================================================================================

void APickupZone::StealObject(AStealableObject* object)
{
	OverlappingObjects.Remove(object);

	object->Destroy();
}

//==========================================================================================