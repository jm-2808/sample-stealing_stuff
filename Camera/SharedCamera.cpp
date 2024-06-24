// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "SharedCamera.h"

//==========================================================================================

#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//==========================================================================================

ASharedCamera::ASharedCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	Pivot->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Pivot);
}

//==========================================================================================

void ASharedCamera::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateView();
}

//==========================================================================================

void ASharedCamera::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Targets.Empty();
}

//==========================================================================================

void ASharedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerController(this, 0)->GetViewTarget() != this)
	{
		UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(this);
	}

	if (Targets.Num() > 0)
	{
		// Move camera to be above centroid of targets
		FVector centroid = UGameplayStatics::GetActorArrayAverageLocation(Targets);
		FVector location = centroid + CentroidOffset;
		FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(location, centroid);
		SetActorLocationAndRotation(location, lookAt);

		// Zoom out based on target separation
		float maxDist = MaxDistanceBetweenTargets();
		float zoom = UKismetMathLibrary::FClamp((maxDist - TargetSeparationThreshold), 0.0f, MaxCameraZoom);
		Camera->SetRelativeLocation(FVector(zoom * -1.0f, 0.0f, 0.0f));
	}
}

//==========================================================================================

void ASharedCamera::UpdateView()
{
	// Set the primary game view to this camera
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(this);
}

//==========================================================================================

float ASharedCamera::MaxDistanceBetweenTargets()
{
	float dist = 0;

	for (int x = 0; x < Targets.Num(); x++)
	{
		FVector target1Pos = Targets[x]->GetActorLocation();
		for (int y = 0; y < Targets.Num(); y++)
		{
			if (y != x)
			{
				FVector target2Pos = Targets[y]->GetActorLocation();
				FVector distVector = target1Pos - target2Pos;

				if (distVector.Size() > dist)
				{
					dist = distVector.Size();
				}
			}
		}
	}

	return dist;
}

//==========================================================================================