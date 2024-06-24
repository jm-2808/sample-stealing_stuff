// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "LevelInformationAsset.h"

//==========================================================================================

int ULevelInformationAsset::GetEntryIndex(FString mapName)
{
	int levelCount = Levels.Num();
	for (int i = 0; i < levelCount; ++i)
	{
		if (Levels[i].Map.GetAssetName() == mapName)
		{
			return i;
		}
	}
	return -1;
}

//==========================================================================================