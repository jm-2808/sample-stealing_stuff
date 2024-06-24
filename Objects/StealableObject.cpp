// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "StealableObject.h"

#include "Stealing_Stuff/Characters/ThiefCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

//==========================================================================================

DEFINE_LOG_CATEGORY(StealLogObject);

//==========================================================================================

AStealableObject::AStealableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

//==========================================================================================

void AStealableObject::BeginPlay()
{
	Super::BeginPlay();

	Collider->SetSimulatePhysics(DefaultEnablePhysics);
	Collider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Collider->SetMassOverrideInKg(NAME_None, Weight);
}

//==========================================================================================

void AStealableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsHeld())
	{
		UpdateMovement(DeltaTime);
		UpdateRotation(DeltaTime);
	}
}

//==========================================================================================

void AStealableObject::PickUp(AActor* interactingActor)
{
	if (InteractingActors.Num() == 0)
	{
		SetActorLocation(FVector(GetActorLocation() + FVector(0.0f, 0.0f, LiftOffset)));
		Collider->SetSimulatePhysics(false);
	}

	InteractionInfo info;
	info.Actor = interactingActor;

	InteractingActors.Add(info);

	OnPickUp();

	UE_LOG(StealLogObject, Display, TEXT("Object [%s] picked up [%i/%i currently holding]"), *GetName(), InteractingActors.Num(), MaximumInteractingPlayers);
}

//==========================================================================================

void AStealableObject::Drop(AActor* interactingActor)
{
	int index = GetInteractorIndexByActor(interactingActor);
	if (index > -1)
	{
		InteractingActors.RemoveAt(index);

		if (InteractingActors.Num() == 0)
		{
			SetActorLocation(FVector(GetActorLocation() + FVector(0.0f, 0.0f, -LiftOffset)));
			Collider->SetSimulatePhysics(true);
		}

		OnDrop();

		UE_LOG(StealLogObject, Display, TEXT("Object [%s] dropped [%i/%i currently holding]"), *GetName(), InteractingActors.Num(), MaximumInteractingPlayers);
	}
}

//==========================================================================================

bool AStealableObject::IsHeld()
{
	return (InteractingActors.Num() > 0);
}

//==========================================================================================

bool AStealableObject::CanBePickedUp()
{
	return (InteractingActors.Num() < MaximumInteractingPlayers);
}
	
//==========================================================================================

int AStealableObject::GetInteractorIndexByActor(AActor* interactingActor)
{
	int interactors = InteractingActors.Num();
	for (int i = 0; i < interactors; ++i)
	{
		if (InteractingActors[i].Actor == interactingActor)
		{
			return i;
		}
	}
	return -1;
}

//==========================================================================================

void AStealableObject::AddMovementInput(FVector direction, float value)
{
	MovementInput.Add(direction * value);
}

//==========================================================================================

void AStealableObject::AddRotationInput(FRotator rotation)
{
	RotationInput.Add(rotation);
}

//==========================================================================================

void AStealableObject::UpdateMovement(float deltaTime)
{
	if (MovementInput.Num() > 0)
	{
		FVector move = FVector(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < MovementInput.Num(); ++i)
		{
			move += MovementInput[i];
		}
		MovementInput.Empty();

		move *= deltaTime;
		move *= GetMovementSpeedMultiplier();

		AttachInteractingActors();

		FVector newPos = (GetActorLocation() + move);
		SetActorLocation(newPos, true);

		DetachInteractingActors();
	}
}

//==========================================================================================

void AStealableObject::UpdateRotation(float deltaTime)
{
	if (RotationInput.Num() > 0)
	{
		FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < RotationInput.Num(); ++i)
		{
			rotate = UKismetMathLibrary::ComposeRotators(rotate, RotationInput[i]);
		}
		RotationInput.Empty();

		//rotate *= deltaTime;
		rotate *= GetRotationSpeedMultiplier();

		AttachInteractingActors();

		FVector pivot = GetPivotPosition();

		FVector minus = GetActorLocation() - pivot;
		FVector rotated = rotate.RotateVector(minus);
		
		FVector newPos = rotated + pivot;
		FRotator newRot = UKismetMathLibrary::ComposeRotators(GetActorRotation(), rotate);

		SetActorLocationAndRotation(newPos, newRot, true);

		DetachInteractingActors();
	}
}

//==========================================================================================

void AStealableObject::AttachInteractingActors()
{
	int interactors = InteractingActors.Num();
	for (int i = 0; i < interactors; ++i)
	{
		InteractingActors[i].Actor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

//==========================================================================================

void AStealableObject::DetachInteractingActors()
{
	int interactors = InteractingActors.Num();
	for (int i = 0; i < interactors; ++i)
	{
		InteractingActors[i].Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

//==========================================================================================

FVector AStealableObject::GetPivotPosition()
{
	if (CanRotateAroundPlayer())
	{
		return InteractingActors[0].Actor->GetActorLocation();
	}
	else
	{
		return GetActorLocation();
	}
}

//==========================================================================================

float AStealableObject::GetMovementSpeedMultiplier()
{
	float diff = MaxMoveMultiplier - MinMoveMultiplier;
	return MinMoveMultiplier + (GetWeightAlpha() * diff);
}

//==========================================================================================

float AStealableObject::GetRotationSpeedMultiplier()
{
	float diff = MaxRotateMultiplier - MinRotateMultiplier;
	return MinRotateMultiplier + (GetWeightAlpha() * diff);
}

//==========================================================================================