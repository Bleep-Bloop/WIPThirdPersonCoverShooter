// Fill out your copyright notice in the Description page of Project Settings.

#include "C_Player_Jax.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h" // Temp for delay test
#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h" // For raycast debugging
#include "Animation/AnimSequence.h"


// Sets default values
AC_Player_Jax::AC_Player_Jax()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and setup spring arm component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->SetupAttachment(RootComponent);

	//bUseControllerRotationYaw = true; 

	// Create and setup camera component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Set turn rates for input
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// Cleaning up AimOffset replication speed NOTE: (although, I am pretty sure the problem has to do with the predictive movement)
	NetUpdateFrequency = 1000; // Maybe too high I believe default is 100	
	MinNetUpdateFrequency = 1000; //If an Actor being considered for update has not sent a meaningful update for more than two seconds, it will start decreasing its update frequency, reaching its minimum frequency after seven seconds without a meaningful update

	Anim_CoverLo_Idle2CoverR = CreateDefaultSubobject<UAnimSequence>(TEXT("EnterCoverStandToLow"));
	Anim_CoverLo_CoverR2Idle = CreateDefaultSubobject<UAnimSequence>(TEXT("LeaveCoverLowToStand"));

	//NOTE: The skeletal mesh and anim blueprint references on the mesh component (inherited from character)
	//		are set in the derived blueprint to avoid direct content references in C++
}



// Called when the game starts or when spawned
void AC_Player_Jax::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Tidy

	ThisPlayerCharacter = this;
	PlayerMesh = GetMesh();
	CharacterMovementComp = GetCharacterMovement();
	ThisPlayerController = Cast<APlayerController>(ThisPlayerCharacter->GetController());
	ThisPlayerController->DisableInput(ThisPlayerController);


	SpringArmComp->bUsePawnControlRotation = true; // I should have this false when not moving and set it to true in the MoveForward()/MoveRight
	bUseControllerRotationYaw = false; //Toggle true when bAiming == true

	CharacterMovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	bOrientRotationToMovementStatus = CharacterMovementComp->bOrientRotationToMovement;

	bUseControllerRotationYawStatus = bUseControllerRotationYaw;

	// Quick note while problem keeps occuring TODO: Fix
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Green, "May need to recompile ABP_Player_Jax if blendspaces not working");
		GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Green, "May need to reassign enter/exit cover anims in BP_Player_Jax (asset names same)");
	}

	// Todo: Spawn weapon from here (currently in BP_Player_Jax


}



void AC_Player_Jax::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to every client (no special condition required)
	DOREPLIFETIME(AC_Player_Jax, MoveForwardValue);
	DOREPLIFETIME(AC_Player_Jax, MoveRightValue);
	DOREPLIFETIME(AC_Player_Jax, AimPitchValue);
	DOREPLIFETIME(AC_Player_Jax, AimYawValue);
	DOREPLIFETIME(AC_Player_Jax, bAiming);
	DOREPLIFETIME(AC_Player_Jax, bCrouching);
	DOREPLIFETIME(AC_Player_Jax, bInCover);
	DOREPLIFETIME(AC_Player_Jax, bCanCover);
	DOREPLIFETIME(AC_Player_Jax, Anim_CoverLo_Idle2CoverR);

}



// Called to bind functionality to input
void AC_Player_Jax::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AC_Player_Jax::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AC_Player_Jax::MoveRight);

	// "Turn" handles devices that provide an absolute delta, such as a mouse.
	// "TurnRate" is for devices that we choose to treat as a rate of change, such as an analog joystick.
	PlayerInputComponent->BindAxis("Turn", this, &AC_Player_Jax::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AC_Player_Jax::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AC_Player_Jax::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AC_Player_Jax::LookUpRate);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AC_Player_Jax::ToggleAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AC_Player_Jax::ToggleAiming);

	PlayerInputComponent->BindAction("Cover", IE_Pressed, this, &AC_Player_Jax::ToggleCover);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AC_Player_Jax::ToggleCrouch);
}



// Called every frame
void AC_Player_Jax::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO:
	// Get display ControlRotationYaw
	// Get Player Rotation
	// Lerp player rotation into ControlRotationYaw when player bAiming == true (lerp in Tick/RInterpTo outside)

}

//////////////////////////
//////// MOVEMENT ////////
//////////////////////////

void AC_Player_Jax::ToggleMovementStatus(bool bCanMove)
{
	if (bCanMove == true)
	{
		CharacterMovementComp->SetMovementMode(MOVE_Walking);
		//ThisPlayerCharacter->DisableInput(ThisPlayerController);
	}
	else
	{
		CharacterMovementComp->DisableMovement();
	}

	if (Role < ROLE_Authority)
	{
		Server_ToggleMovementStatus(bCanMove);
	}
}

void AC_Player_Jax::Server_ToggleMovementStatus_Implementation(bool bCanMove)
{
	ToggleMovementStatus(bCanMove);
}

bool AC_Player_Jax::Server_ToggleMovementStatus_Validate(bool bCanMove)
{
	return true;
}


void AC_Player_Jax::MoveForward(float Value)
{
	MoveForwardValue = Value;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	if (Role < ROLE_Authority)
	{
		Server_MoveForward(Value);
	}
}

void AC_Player_Jax::Server_MoveForward_Implementation(float Value)
{
	MoveForward(Value);
}

bool AC_Player_Jax::Server_MoveForward_Validate(float Value)
{
	return true;
}



void AC_Player_Jax::MoveRight(float Value)
{
	MoveRightValue = Value;

	if ((Controller != NULL) && (Value != 0.0f)) //Note: Might play around with value doesnt equal or could just leave replication thing outside 
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
	if (Role < ROLE_Authority)
	{
		Server_MoveRight(Value);
	}
}

void AC_Player_Jax::Server_MoveRight_Implementation(float Value)
{
	MoveRight(Value);
}

bool AC_Player_Jax::Server_MoveRight_Validate(float Value)
{
	return true;
}



void AC_Player_Jax::ToggleCrouch()
{

	if (CanCrouch() == true)
	{
		Crouch();
		bCrouching = true;
	}
	else
	{
		UnCrouch();
		bCrouching = false;
	}

	if (Role < ROLE_Authority)
	{
		Server_ToggleCrouch();
	}
}

void AC_Player_Jax::Server_ToggleCrouch_Implementation()
{
	ToggleCrouch();
}

bool AC_Player_Jax::Server_ToggleCrouch_Validate()
{
	return true;
}


void AC_Player_Jax::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the Rate information.
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void AC_Player_Jax::LookUpRate(float Rate)
{
	// Calculate delta for this frame from the Rate information.
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AC_Player_Jax::AddControllerYawInput(float Rate)
{
	Super::AddControllerYawInput(Rate);

	SetAimOffsetValues();
}

void AC_Player_Jax::AddControllerPitchInput(float Rate)
{
	Super::AddControllerPitchInput(Rate);

	SetAimOffsetValues();
}


void AC_Player_Jax::ToggleUseControllerRotationYaw(bool bStatus)
{
	bUseControllerRotationYaw = bStatus;

	if (Role < ROLE_Authority)
	{
		Server_ToggleUseControllerRotationYaw(bStatus);
	}
}

void AC_Player_Jax::Server_ToggleUseControllerRotationYaw_Implementation(bool bStatus)
{
	ToggleUseControllerRotationYaw(bStatus);
}

bool AC_Player_Jax::Server_ToggleUseControllerRotationYaw_Validate(bool bStatus)
{
	return true;
}



void AC_Player_Jax::SetAimOffsetValues()
{

	Target = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()); //maybe just rotator var declaration like below

	Current = UKismetMathLibrary::MakeRotator(0, AimPitchValue, AimYawValue);

	RInterptReturnValue = UKismetMathLibrary::RInterpTo(Current, Target, GetWorld()->DeltaTimeSeconds, 15);

	AimPitchValue = FMath::ClampAngle(RInterptReturnValue.Pitch, -90.f, 90.f);
	AimYawValue = FMath::ClampAngle(RInterptReturnValue.Yaw, -90.f, 90.f);

	if (Role < ROLE_Authority)
	{
		Server_SetAimOffsetValues(AimPitchValue, AimYawValue);
	}
}

void AC_Player_Jax::Server_SetAimOffsetValues_Implementation(float PitchValue, float YawValue)
{
	AimPitchValue = PitchValue;
	AimYawValue = YawValue;
}

bool AC_Player_Jax::Server_SetAimOffsetValues_Validate(float PitchValue, float YawValue)
{
	return true;
}



void AC_Player_Jax::ToggleAiming()
{

	// TODO:

	// Fix rotation snapping:
	// Raycast to aiming location
	// Get location of hit
	// Get character rotation
	// VInterpTo character rotation to rotation of hit (lerp needs to be in Tick)
	// Continue with rest of function

	//Pseudo
	/* Raycasting to hit
	FHitResult* HitResult = new FHitResult; //Making new space of memory / declared as pointer because we are passing it to a funciton and funciton needs to have access to hit result so we need to pass by reference
	FVector StartTrace = CameraComp->GetComponentLocation();
	FVector ForwardVector = CameraComp->GetForwardVector();
	FVector EndTrace = ((ForwardVector * 5000.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, true);
	}

	// TODO: Figure out what I'm doing wrong with FMath::VInterpTo
	FRotator LookToRotation = UKismetMathLibrary::FindLookAtRotation(StartTrace, HitResult->GetActor()->GetActorLocation());
	FRotator LookToRotationSet = FRotator(0, LookToRotation.Yaw, 0);
	SetActorRotation(LookToRotationSet); FMath::VInterpTo this
	*/


	bAiming = !bAiming;
	
	bUseControllerRotationYaw = !bUseControllerRotationYaw; // NOTE: Don't toggle untill RInterpTo rotation
	
	bOrientRotationToMovementStatus = !bOrientRotationToMovementStatus; // NOTE: Toggle off to prevent jittering when strafing
	CharacterMovementComp->bOrientRotationToMovement = bOrientRotationToMovementStatus;

	if (Role < ROLE_Authority)
	{
		Server_ToggleAiming();
	}
}

void AC_Player_Jax::Server_ToggleAiming_Implementation()
{
	ToggleAiming();
}

bool AC_Player_Jax::Server_ToggleAiming_Validate()
{
	return true;
}

/////////////////
///// COVER /////
/////////////////

/**
 * Toggles bInCover
 * @param bInCoverStatus	If player is in/out of cover
 */
void AC_Player_Jax::ToggleInCover(bool bInCoverStatus)
{
	bInCover = bInCoverStatus;

	if (Role < ROLE_Authority)
	{
		Server_ToggleInCover(bInCoverStatus);
	}
}

void AC_Player_Jax::Server_ToggleInCover_Implementation(bool bInCoverStatus)
{
	ToggleInCover(bInCoverStatus);
}

bool AC_Player_Jax::Server_ToggleInCover_Validate(bool bInCoverStatus)
{
	return true;
}


void AC_Player_Jax::ToggleCanCover(bool bCanCoverStatus)
{
	bCanCover = bCanCoverStatus;

	if (Role < ROLE_Authority)
	{
		Server_ToggleCanCover(bCanCoverStatus);
	}
}

void AC_Player_Jax::Server_ToggleCanCover_Implementation(bool bCanCoverStatus)
{
	ToggleCanCover(bCanCoverStatus);
}

bool AC_Player_Jax::Server_ToggleCanCover_Validate(bool bCanCoverStatus)
{
	return true;
}



void AC_Player_Jax::IsCoverAvailable(bool bCoverAvailable, FRotator CoverRotation)
{
	ToggleCanCover(bCoverAvailable);
	bCanCover = bCoverAvailable;
	TargetCoverRotation = CoverRotation;
}

//* Called to enter/exit cover*/
void AC_Player_Jax::ToggleCover()
{
	if (bInCover == true)
	{
		ToggleInCover(false);
		ToggleMovementStatus(true);
		StopMovementPlayAnimation(Anim_CoverLo_CoverR2Idle, PlayerMesh);
	}
	else
	{
		if (bCanCover == true)
		{
			ToggleInCover(true);
			ToggleMovementStatus(false);
			StopMovementPlayAnimation(Anim_CoverLo_Idle2CoverR, PlayerMesh);
		}
	}

		if (Role < ROLE_Authority)
		{
			Server_ToggleCover();
		}
}

void AC_Player_Jax::Server_ToggleCover_Implementation()
{
	ToggleCover();
}

bool AC_Player_Jax::Server_ToggleCover_Validate()
{
	return true;
}



/////////////////////
///// ANIMATION ///// Move this to AnimInst_Player_Jax 
///////////////////// 

void AC_Player_Jax::StopMovementPlayAnimation(UAnimSequence* AnimToPlay, USkeletalMeshComponent* IncomingPlayerMeshPlayerMesh)
{
	float AnimLength;

	if (AnimToPlay != nullptr)
	{
		AnimLength = AnimToPlay->GetPlayLength();


		FLatentActionInfo LatentInfo;
		UKismetSystemLibrary::Delay(this, AnimLength, LatentInfo);

		PlayerMesh->PlayAnimation(AnimToPlay, false);

		if (Role < ROLE_Authority)
		{
			Server_StopMovementPlayAnimation(AnimToPlay, PlayerMesh);
		}
	}
}

void AC_Player_Jax::Server_StopMovementPlayAnimation_Implementation(UAnimSequence* AnimToPlay, USkeletalMeshComponent* IncomingPlayerMeshPlayerMesh)
{
	StopMovementPlayAnimation(AnimToPlay, PlayerMesh);
}

bool AC_Player_Jax::Server_StopMovementPlayAnimation_Validate(UAnimSequence* AnimToPlay, USkeletalMeshComponent* IncomingPlayerMeshPlayerMesh)
{
	return true;
}


/////////////////
///// DEBUG /////
/////////////////

// Quick printing for debug purposes
void AC_Player_Jax::TestPrint()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "TestPrint()");
	}
}