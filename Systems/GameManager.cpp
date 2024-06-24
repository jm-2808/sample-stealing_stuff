// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "GameManager.h"

//==========================================================================================

#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"

//==========================================================================================

DEFINE_LOG_CATEGORY(StealLogManager);

//==========================================================================================

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// LevelInformation
	static ConstructorHelpers::FObjectFinder<ULevelInformationAsset> level(*LevelInformationAssetLocation);
	LevelInformation = level.Object;
	UE_LOG(StealLogManager, Display, TEXT("LevelInformation compile-time load: %s"), ((LevelInformation != nullptr) ? TEXT("SUCCEEDED") : TEXT("FAILED")));
}

//==========================================================================================

void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	IsInGame = false;
	TimedOut = false;

	InitialiseLevelInfo();

	if (GangVan != nullptr)
	{
		GangVan->InitialiseUI();
	}

	if (GangVan != nullptr)
	{
		GangVan->InitialisePlayers(DebugPlayerCount);

		TArray<AThiefCharacter*> Players = GangVan->GetPlayers();
		for (int i = 0; i < Players.Num(); ++i)
		{
			Players[i]->SetDefaultMovementAxisAngle(AxisRotation);
		}
	}

	if (GangVan != nullptr)
	{
		GangVan->OnEntry.AddDynamic(this, &AGameManager::OnEntry);
		GangVan->OnExtraction.AddDynamic(this, &AGameManager::OnExtraction);
	}

	if (Camera != nullptr && GangVan != nullptr)
	{
		Camera->Targets.Add(GangVan);
		UE_LOG(StealLogManager, Display, TEXT("Camera now following Van"));
	}
}

//==========================================================================================

void AGameManager::EndPlay(const EEndPlayReason::Type endPlayReason)
{

}

//==========================================================================================

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInGame)
	{
		UpdateTimer(DeltaTime);
		UpdateGangVan(DeltaTime);
	}
}

//==========================================================================================

void AGameManager::InitialiseLevelInfo()
{
	if (LevelInformation != nullptr)
	{
		FString mapName = GetWorld()->GetName();
		int idx = LevelInformation->GetEntryIndex(mapName);

		if (idx >= 0)
		{
			LevelInfo = LevelInformation->Levels[idx];
		}
		else
		{
			LevelInfo.Name = TEXT("Level name not recognised");
		}

		UE_LOG(StealLogManager, Display, TEXT("Level Info loaded"));
		UE_LOG(StealLogManager, Display, TEXT("    Name: %s"), *LevelInfo.Name.ToString());
		UE_LOG(StealLogManager, Display, TEXT("    Timer: %i:%i:%.2f"), LevelInfo.Timer.Hours, LevelInfo.Timer.Minutes, LevelInfo.Timer.Seconds);
		UE_LOG(StealLogManager, Display, TEXT("    Score Objective: %.2f"), LevelInfo.Score.Objective);
	}
	else
	{
		UE_LOG(StealLogManager, Warning, TEXT("LevelInformation not loaded, failed to retrieve information"));
	}
}

//==========================================================================================

void AGameManager::OnEntry()
{
	if (GangVan != nullptr)
	{
		GangVan->ActivatePickupZone();

		if (ShowUI)
		{
			GangVan->SetGameUIVisibility(true);
			GangVan->SetGameUITimerVisibility(true);
		}

		if (Camera != nullptr)
		{
			Camera->Targets.Empty();

			TArray<AThiefCharacter*> Players = GangVan->GetPlayers();

			TArray<AActor*> targets;
			for (int i = 0; i < Players.Num(); ++i)
			{
				targets.Add(Players[i]);
			}

			Camera->Targets = targets;
			Camera->UpdateView();

			UE_LOG(StealLogManager, Display, TEXT("Camera now following Players"));
		}
	}

	IsInGame = true;
	StartLevelTimer();
}

//==========================================================================================

void AGameManager::OnExtraction()
{
	UGameplayStatics::OpenLevel(this, TEXT("LevelEnd"));
}

//==========================================================================================

void AGameManager::UpdateGangVan(float deltaTime)
{
	if (GangVan != nullptr)
	{
		if (GangVan->GetStolenMoney() >= LevelInfo.Score.Objective)
		{
			GangVan->ActivateExtractionZone();
		}
	}
}

//==========================================================================================

float AGameManager::GetScore()
{
	if (GangVan != nullptr)
	{
		return GangVan->GetStolenMoney();
	}
	else
	{
		return 0.0f;
	}
}

//==========================================================================================

void AGameManager::AddScore(float value)
{
	if (GangVan != nullptr)
	{
		GangVan->AddMoney(value);
	}
}

//==========================================================================================

void AGameManager::LoseScore(float value)
{
	if (GangVan != nullptr)
	{
		GangVan->RemoveMoney(value);
	}
}

//==========================================================================================

void AGameManager::StartLevelTimer()
{
	Timer.Set(LevelInfo.Timer.Hours, LevelInfo.Timer.Minutes, LevelInfo.Timer.Seconds);
	if (!Timer.Finished)
	{
		UE_LOG(StealLogManager, Display, TEXT("Level Timer Activated: %i:%i:%.2f"), Timer.Hours, Timer.Minutes, Timer.Seconds);
	}
}

//==========================================================================================

void AGameManager::StartPoliceTimer()
{
	Timer.Set(0, 1, 0.0f);

	UE_LOG(StealLogManager, Display, TEXT("Police Timer Activated: %i:%i:%.2f"), Timer.Hours, Timer.Minutes, Timer.Seconds);
}

//==========================================================================================

void AGameManager::StartExtractionTimer()
{
	Timer.Set(0, 0, 30.0f);

	UE_LOG(StealLogManager, Display, TEXT("Extraction Timer Activated: %i:%i:%.2f"), Timer.Hours, Timer.Minutes, Timer.Seconds);
}

//==========================================================================================

void AGameManager::UpdateTimer(float deltaTime)
{
	Timer.Update(deltaTime);
	if (Timer.Finished && !TimedOut)
	{
		TimedOut = true;
		UE_LOG(StealLogManager, Display, TEXT("Time out"));

		GangVan->SignalGangExtraction();
	}

	if (GangVan != nullptr)
	{
		GangVan->UpdateGameUITimer(Timer.Hours, Timer.Minutes, Timer.Seconds);
	}
}

//==========================================================================================