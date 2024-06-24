// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "ColourInformationAsset.h"

//==========================================================================================

FGradientColour UColourInformationAsset::GetDefaultColour()
{
	FGradientColour col;
	col.Points.Add(FLinearColor(1.0f, 1.0f, 1.0f));
	col.Points.Add(FLinearColor(1.0f, 1.0f, 1.0f));
	return col;
}

//==========================================================================================

FGradientColour UColourInformationAsset::GetColourByName(FName name)
{
	if (Colours.Num() > 0)
	{
		for (int i = 0; i < Colours.Num(); ++i)
		{
			if (Colours[i].Name == name)
			{
				return Colours[i];
			}
		}
	}

	return GetDefaultColour();
}

//==========================================================================================

FGradientColour UColourInformationAsset::GetColourByIndex(int index)
{
	if (Colours.Num() > 0)
	{
		if (index >= 0 && index < Colours.Num())
		{
			return Colours[index];
		}
	}

	return GetDefaultColour();
}

//==========================================================================================