// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "SharedCamera.generated.h"

//==========================================================================================

UCLASS()
class STEALING_STUFF_API ASharedCamera : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Pivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	
public:	
	ASharedCamera();

	// AActor interface
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Camera
public:
	UFUNCTION(BlueprintCallable, Category = Camera)
	void UpdateView();

	UFUNCTION(BlueprintCallable, Category = Camera)
	UCameraComponent* GetCamera() { return Camera; }

private:
	float MaxDistanceBetweenTargets();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Targets;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetSeparationThreshold = 1200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxCameraZoom = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector CentroidOffset;
};

//==========================================================================================