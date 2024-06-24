// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "ExtractionZone.h"

//==========================================================================================

AExtractionZone::AExtractionZone()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

//==========================================================================================

void AExtractionZone::BeginPlay()
{
	Super::BeginPlay();

	Active = false;
}

//==========================================================================================

void AExtractionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//==========================================================================================

void AExtractionZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AThiefCharacter* object = Cast<AThiefCharacter>(OtherActor);
	if (object != nullptr)
	{
		OverlappingPlayers.Add(object);
	}
}

//==========================================================================================

void AExtractionZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	AThiefCharacter* object = Cast<AThiefCharacter>(OtherActor);
	if (object != nullptr)
	{
		if (OverlappingPlayers.Contains(object))
		{
			OverlappingPlayers.Remove(object);
		}
	}
}

//==========================================================================================

TArray<AThiefCharacter*> AExtractionZone::GetOverlappingPlayers()
{
	return OverlappingPlayers;
}

//==========================================================================================