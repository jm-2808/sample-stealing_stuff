// Fill out your copyright notice in the Description page of Project Settings.
//==========================================================================================

#include "ThiefCharacter.h"

//==========================================================================================

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

//==========================================================================================

DEFINE_LOG_CATEGORY(StealLogThief);

//==========================================================================================

AThiefCharacter::AThiefCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	RingWidgetPivot = CreateDefaultSubobject<USceneComponent>(TEXT("RingWidgetPivot"));
	RingWidgetPivot->SetupAttachment(RootComponent);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCapsuleComponent()->SetCapsuleRadius(20);

	SetDefaultMovementAxisAngle(0.0f);
}

//==========================================================================================

void AThiefCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DashCooldownTimer = 0.0f;

	ForwardValue = 0.0f;
	RightValue = 0.0f;

	LeftHold = false;
	RightHold = false;

	HeldObject = nullptr;
}

//==========================================================================================

void AThiefCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Dash cooldown
	if (DashCooldownTimer > 0.0f)
	{
		DashCooldownTimer -= DeltaTime;
		if (DashCooldownTimer < 0.0f)
		{
			DashCooldownTimer = 0.0f;
		}
	}

	// Update Movement Axis
	if (IsHoldingObject())
	{
		SetMovementAxisAngle(GetActorRotation().Yaw);
	}

	// Update Ring Widget
	if (IsHoldingObject())
	{
		if ((ForwardValue != 0.0f) || (RightValue != 0.0f))
		{
			// Movement direction
			//float forward = ((ForwardValue >= 0.0f) ? 0.0f : 180.0f);
			//float right = (90.0f * RightValue) * (HeldObject->CanRotateAroundPlayer() ? 1.0f : -1.0f);

			// Joystick direction
			float forward = ((ForwardValue >= 0.0f) ? 0.0f : 180.0f);
			float right = (90.0f * RightValue) * ((ForwardValue >= 0.0f) ? 1.0f : -1.0f);

			FRotator rotation = FRotator(0.0f, forward + right, 0.0f);
			RingWidgetPivot->SetRelativeRotation(rotation);
		}
		else
		{
			RingWidgetPivot->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

//==========================================================================================

void AThiefCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AThiefCharacter::Pause);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThiefCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThiefCharacter::MoveRight);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AThiefCharacter::Dash);

	PlayerInputComponent->BindAction("PickUpLeft", IE_Pressed, this, &AThiefCharacter::PickUpLeft_Pressed);
	PlayerInputComponent->BindAction("PickUpLeft", IE_Released, this, &AThiefCharacter::PickUpLeft_Released);
	PlayerInputComponent->BindAction("PickUpRight", IE_Pressed, this, &AThiefCharacter::PickUpRight_Pressed);
	PlayerInputComponent->BindAction("PickUpRight", IE_Released, this, &AThiefCharacter::PickUpRight_Released);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AThiefCharacter::Interact);
	PlayerInputComponent->BindAction("Emote", IE_Pressed, this, &AThiefCharacter::Emote);
}

//=== Movement =============================================================================

void AThiefCharacter::SetDefaultMovementAxisAngle(float value)
{
	DefaultMovementAxisRotation = value;

	if (!IsHoldingObject())
	{
		SetMovementAxisAngle(DefaultMovementAxisRotation);
	}

	UE_LOG(StealLogThief, Display, TEXT("Axis Updated [Default: %f | Current: %f]"), DefaultMovementAxisRotation, MovementAxisRotation);
}

//==========================================================================================

void AThiefCharacter::SetMovementAxisAngle(float value)
{
	MovementAxisRotation = value;
}

//==========================================================================================

void AThiefCharacter::MoveForward(float value)
{
	ForwardValue = value;

	if ((Controller != nullptr) && (value != 0.0f))
	{
		if (!IsHoldingObject())
		{
			FRotator rot = FRotator(0, MovementAxisRotation, 0);
			FVector forward = UKismetMathLibrary::GetForwardVector(rot);
			AddMovementInput(forward, value);
		}
		else
		{
			FRotator rot = FRotator(0, MovementAxisRotation, 0);
			FVector forward = UKismetMathLibrary::GetForwardVector(rot);

			HeldObject->AddMovementInput(forward, (GetMovementSpeed() * value));
		}
	}
}

//==========================================================================================

void AThiefCharacter::MoveRight(float value)
{
	RightValue = value;

	if ((Controller != nullptr) && (value != 0.0f))
	{
		if (!IsHoldingObject())
		{
			FRotator rot = FRotator(0, MovementAxisRotation, 0);
			FVector right = UKismetMathLibrary::GetRightVector(rot);
			AddMovementInput(right, value);
		}
		else
		{
			float angle = (GetRotationSpeed() * value) * (HeldObject->CanRotateAroundPlayer() ? 1.0f : -1.0f);
			FRotator rotation = FRotator(0.0f, angle, 0.0f);

			HeldObject->AddRotationInput(rotation);
		}
	}
}

//==========================================================================================

void AThiefCharacter::Dash()
{
	if (!IsHoldingObject())
	{
		if (DashCooldownTimer <= 0.0f)
		{
			FVector launchVelocity = GetActorForwardVector() * DashImpulseForce;
			LaunchCharacter(launchVelocity, false, false);

			OnDash();

			DashCooldownTimer = DashCooldown;
		}
	}
}

//==========================================================================================

float AThiefCharacter::GetMovementSpeed()
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

//==========================================================================================

float AThiefCharacter::GetRotationSpeed()
{
	return DefaultRotationRate; //GetCharacterMovement()->RotationRate.Yaw;
}

//=== Interaction ==========================================================================

void AThiefCharacter::PickUpLeft_Pressed()
{
	LeftHold = true;
	if (RightHold)
	{
		PickUpObject();
	}
}
void AThiefCharacter::PickUpLeft_Released()
{
	LeftHold = false;
	if (IsHoldingObject())
	{
		DropObject();
	}
}

//==========================================================================================

void AThiefCharacter::PickUpRight_Pressed()
{
	RightHold = true;
	if (LeftHold)
	{
		PickUpObject();
	}
}
void AThiefCharacter::PickUpRight_Released()
{
	RightHold = false;
	if (IsHoldingObject())
	{
		DropObject();
	}
}

//==========================================================================================

void AThiefCharacter::PickUpObject()
{
	// Find object
	FHitResult hit;
	AStealableObject* object = FindStealableObject(hit);

	// Pick up object
	if (object != nullptr)
	{
		if (object->CanBePickedUp())
		{
			HeldObject = object;
			HeldObject->PickUp(this);

			UpdatePlayerOnPickup(hit);

			OnPickupObject();

			UE_LOG(StealLogThief, Display, TEXT("Object picked up [Name: %s | Weight: %.2f]"), *HeldObject->GetName(), HeldObject->GetWeight());
		}
	}
}

//==========================================================================================

void AThiefCharacter::DropObject()
{
	// Drop object
	if (HeldObject != nullptr)
	{
		HeldObject->Drop(this);
		HeldObject = nullptr;

		UpdatePlayerOnDrop();

		OnDropObject();

		UE_LOG(StealLogThief, Display, TEXT("Object dropped"));
	}
}

//==========================================================================================

void AThiefCharacter::UpdatePlayerOnPickup(FHitResult hit)
{
	// Look at object
	FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HeldObject->GetActorLocation());
	lookAt.Pitch = 0.0f;
	SetActorRotation(lookAt);

	// Move away if too close
	FVector capsulePoint = (GetActorLocation() + GetActorRotation().RotateVector(FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), 0.0f, 0.0f)));
	FVector colliderPoint;

	HeldObject->GetCollider()->GetClosestPointOnCollision(capsulePoint, colliderPoint);

	capsulePoint.Z = 0;
	colliderPoint.Z = 0;

	FVector dist = capsulePoint - colliderPoint;
	UE_LOG(StealLogThief, Display, TEXT("Capsule to Collider distance [%.2f]"), dist.Size());

	if (dist.Size() < 15.0f)
	{
		FVector move = (GetActorForwardVector() * -1.0f) * (15.0f - dist.Size());
		SetActorLocation(GetActorLocation() + move);

		UE_LOG(StealLogThief, Display, TEXT("Distance from Object Collider shifted [%.2f -> 15.0]"), dist.Size());
	}
}

//==========================================================================================

void AThiefCharacter::UpdatePlayerOnDrop()
{
	SetMovementAxisAngle(DefaultMovementAxisRotation);
	RingWidgetPivot->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

//==========================================================================================

AStealableObject* AThiefCharacter::FindStealableObject(FHitResult& hit)
{
	FVector capsuleRadius = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), 0.0f, 0.0f);
	FVector capsuleBounds = (GetActorLocation() + GetActorRotation().RotateVector(capsuleRadius));

	TArray<FVector> rays = {
		FVector(0.0f, 0.0f, 60.0f),		// Head
		FVector(0.0f, 0.0f, 30.0f),		// Chest
		FVector(0.0f, 0.0f, 0.0f),		// Center
		FVector(0.0f, 0.0f, -30.0f),	// Knees
		FVector(0.0f, 0.0f, -60.0f)		// Feet
	};

	TArray<FRotator> rotations = {
		FRotator(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 45.0f, 0.0f),
		FRotator(0.0f, -45.0f, 0.0f)
	};

	AStealableObject* object = nullptr;

	for (int x = 0; x < rays.Num(); ++x)
	{
		FVector start = capsuleBounds + rays[x];
		for (int y = 0; y < rotations.Num(); ++y)
		{
			FVector dir = rotations[y].RotateVector(GetActorForwardVector());
			if (LineTrace(start, dir, PickupDistance, hit, false))
			{
				object = Cast<AStealableObject>(hit.GetActor());
				if (object != nullptr)
				{
					UE_LOG(StealLogThief, Display, TEXT("Object detected"));
					break;
				}
			}
		}

		if (object != nullptr)
		{
			break;
		}
	}

	return object;
}

//==========================================================================================

bool AThiefCharacter::LineTrace(FVector start, FVector direction, float length, FHitResult& hit, bool debugLine)
{
	FVector end = start + (direction * length);
	if (debugLine)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), start, end, FColor::Red, 1.0f, 2.0f);
	}
	return GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility);
}

//==========================================================================================

void AThiefCharacter::Interact()
{
	UE_LOG(StealLogThief, Display, TEXT("Interact pressed"));
}

//=== Emotes ===============================================================================

void AThiefCharacter::Emote()
{

}

//==========================================================================================