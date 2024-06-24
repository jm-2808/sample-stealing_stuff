// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#pragma once

//==========================================================================================

#include "CoreMinimal.h"

#include "LevelInformationAsset.h"

#include "Stealing_Stuff/Camera/SharedCamera.h"
#include "Stealing_Stuff/Vehicles/GangVan.h"

#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

//==========================================================================================

DECLARE_LOG_CATEGORY_EXTERN(StealLogManager, Display, All);

//==========================================================================================

struct LevelTimer
{
	int Hours;
	int Minutes;
	float Seconds;

	bool Finished;

	LevelTimer()
	{
		Hours = 0;
		Minutes = 0;
		Seconds = 0.0f;

		Finished = true;
	}

	void Update(float deltaTime)
	{
		if (!Finished)
		{
			Seconds -= deltaTime;
			if (Seconds <= 0.0f)
			{
				if (Minutes > 0)
				{
					Minutes -= 1;
					Seconds = 60.0f;
				}
				else
				{
					Seconds = 0.0f;
					Finished = true;
				}
			}
		}
	}

	void Set(int h, int m, float s)
	{
		Hours = h;
		Minutes = m;
		Seconds = s;

		Finished = !(Hours > 0 || Minutes > 0 || Seconds > 0.0f);
	}
};

//==========================================================================================

UCLASS()
class STEALING_STUFF_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameManager();

	// AActor interface
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	// Information
private:
	void InitialiseLevelInfo();

	/// Level
private:
	UPROPERTY()
	ULevelInformationAsset* LevelInformation;
	const FString LevelInformationAssetLocation = "/Game/Maps/LevelInfo";

	UPROPERTY()
	FLevelInformation LevelInfo;

	// Gang Van
public:
	UFUNCTION()
	void OnEntry();

	UFUNCTION()
	void OnExtraction();

private:
	void UpdateGangVan(float deltaTime);

private:
	bool IsInGame;

	// Score
public:
	UFUNCTION(BlueprintPure)
	float GetScore();

	UFUNCTION(BlueprintCallable)
	void AddScore(float value);

	UFUNCTION(BlueprintCallable)
	void LoseScore(float value);

	// Timer
public:
	UFUNCTION(BlueprintCallable)
	void StartLevelTimer();

	UFUNCTION(BlueprintCallable)
	void StartPoliceTimer();

	UFUNCTION(BlueprintCallable)
	void StartExtractionTimer();

private:
	void UpdateTimer(float deltaTime);

private:
	LevelTimer Timer;
	bool TimedOut;

	// Parameters
public:
	UPROPERTY(EditAnywhere)
	int DebugPlayerCount = 1.0f;

	UPROPERTY(EditAnywhere)
	float AxisRotation = 0.0f;

	UPROPERTY(EditAnywhere)
	ASharedCamera* Camera;

	UPROPERTY(EditAnywhere)
	AGangVan* GangVan;

	UPROPERTY(EditAnywhere)
	bool ShowUI = true;
};

//==========================================================================================