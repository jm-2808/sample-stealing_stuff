// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "ModularCharacter.h"


//==========================================================================================

#include "UObject/ConstructorHelpers.h"

#include "Components/SkeletalMeshComponent.h"

//==========================================================================================

DEFINE_LOG_CATEGORY(StealLogCharacter);

//==========================================================================================

AModularCharacter::AModularCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// CharacterInformation
	static ConstructorHelpers::FObjectFinder<UCharacterInformationAsset> character(*CharacterInformationAssetLocation);
	CharacterInformation = character.Object;
	UE_LOG(StealLogCharacter, Display, TEXT("CharacterInformation compile-time load: %s"), ((CharacterInformation != nullptr) ? TEXT("SUCCEEDED") : TEXT("FAILED")));

	// Modular character mesh components
	GetMesh()->SetVisibility(false);
	GetMesh()->SetHiddenInGame(true);

	Skeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeleton"));
	Skeleton->SetupAttachment(RootComponent);
	Skeleton->SetVisibility(false);
	Skeleton->SetHiddenInGame(true);

	Hat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
	Hat->SetupAttachment(Skeleton, TEXT("Hat_Socket"));

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Skeleton);
	Head->SetMasterPoseComponent(Skeleton);

	Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(Skeleton);
	Torso->SetMasterPoseComponent(Skeleton);

	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Hands->SetupAttachment(Skeleton);
	Hands->SetMasterPoseComponent(Skeleton);

	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Waist->SetupAttachment(Skeleton);
	Waist->SetMasterPoseComponent(Skeleton);

	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(Skeleton);
	Legs->SetMasterPoseComponent(Skeleton);
}

//==========================================================================================

void AModularCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//==========================================================================================

void AModularCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//==========================================================================================

void AModularCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//=== Appearance ===========================================================================

void AModularCharacter::SetModel(FCharacterModelInfo modelInfo)
{
	if (CharacterInformation != nullptr)
	{
		if (CharacterInformation->Hats.Contains(modelInfo.Hat))
		{
			Hat->SetStaticMesh(CharacterInformation->Hats[modelInfo.Hat]);
		}

		if (CharacterInformation->Heads.Contains(modelInfo.Head))
		{
			Head->SetSkeletalMesh(CharacterInformation->Heads[modelInfo.Head]);
		}

		if (CharacterInformation->Torsos.Contains(modelInfo.Torso))
		{
			Torso->SetSkeletalMesh(CharacterInformation->Torsos[modelInfo.Torso]);
		}

		if (CharacterInformation->Hands.Contains(modelInfo.Hands))
		{
			Hands->SetSkeletalMesh(CharacterInformation->Hands[modelInfo.Hands]);
		}

		if (CharacterInformation->Waists.Contains(modelInfo.Waist))
		{
			Waist->SetSkeletalMesh(CharacterInformation->Waists[modelInfo.Waist]);
		}

		if (CharacterInformation->Legs.Contains(modelInfo.Legs))
		{
			Legs->SetSkeletalMesh(CharacterInformation->Legs[modelInfo.Legs]);
		}
	}
}

//==========================================================================================