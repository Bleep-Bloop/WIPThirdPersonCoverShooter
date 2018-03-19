// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Player_Jax.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class UAnimSequence;




UCLASS()
class LETHALSPEED_API AC_Player_Jax : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Player_Jax();


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UAnimSequence* AnimToPlayReplicate;


	////////////////////////////////
	////////// COMPONENTS //////////
	////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		AC_Player_Jax * ThisPlayerCharacter;

	/** Character skeletal mesh for accessing/playing animations */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player/Components")
		USkeletalMeshComponent* PlayerMesh;

	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player/Components/Camera", meta = (AllowPrivateAccess = "true")) //Question: Why AllowPrivateAccess?
		USpringArmComponent* SpringArmComp;

	/** Follow camera */
	UPROPERTY(VisibleANywhere, BlueprintReadOnly, Category = "Components/Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComp;

	/** Player Controller */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player/Components")
		APlayerController* ThisPlayerController;

	/** Character Movement Component for rotation/crouch/jump/input */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player/Components")
		UCharacterMovementComponent* CharacterMovementComp;



	//////////////////////////////////
	/////////// ANIMATIONS ///////////
	////////////////////////////////// 

	/** TODO: Move all animations to AnimInst_Player_Jax class */
	/** Set in BP to avoid direct asset references in cpp */

	UPROPERTY(EditDefaultsOnly, Category = "Animations/Cover", Replicated)
		UAnimSequence* Anim_CoverLo_Idle2CoverR; 

	UPROPERTY(EditDefaultsOnly, Category = "Animations/Cover", Replicated)
		UAnimSequence* Anim_CoverLo_CoverR2Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations/Cover")
		UAnimSequence* Anim_LowCoverRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations/Cover")
		UAnimSequence* Anim_LowCoverRightToLowCoverLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations/Cover")
		UAnimSequence* Anim_LowCoverLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations/Cover")
		UAnimSequence* Anim_LowCoverLeftToLowCoverRight;

	/*
	* Play animation and disable movement for length of animation
	* @param AnimToPlay
	 *@param PlayerMesh
	*/
	void StopMovementPlayAnimation(UAnimSequence* AnimToPlay, USkeletalMeshComponent* PlayerMesh);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_StopMovementPlayAnimation(UAnimSequence* AnimToPlay, USkeletalMeshComponent* IncomingPlayerMesh);



	////////////////////////////////
	//////////  MOVEMENT  //////////
	////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerMovement")
		bool bMovementDisabled;

	/** Activates/deactivates CharacterMovementComponent MovementMode */
	void ToggleMovementStatus(bool bCanMove);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleMovementStatus(bool bCanMove);


	/** Handles forwards/backwards movement */
	void MoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveForward(float Value);


	/** Handles right/left movement */
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveRight(float Value);


	/** Used to control movement blendspaces */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		float MoveForwardValue;

	/** Used to control movement blendspaces */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		float MoveRightValue;


	// TODO: Override &APawn::Crouch()/Uncrouch() or leave
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		bool bCrouching;

	/** Called to toggle crouch blendspace */
	void ToggleCrouch();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleCrouch();


	/** Used to allign character rotation to movement when not aiming */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		bool bOrientRotationToMovementStatus;

	/** Used to allign character rotation when aiming */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		bool bUseControllerRotationYawStatus;

	/** Seperate function to call from blueprint (double check BPs and delete) */
	UFUNCTION(BlueprintCallable, Category = "PlayerMovement")
		void ToggleUseControllerRotationYaw(bool bStatus);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleUseControllerRotationYaw(bool bStatus);


	/** Used to start iron sights blendspaces */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		bool bAiming;

	/** Toggles bAiming */
	void ToggleAiming();

	/** Call to replicate bAiming */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleAiming();



	////////////////////////////////
	///////////  CAMERA  ///////////
	////////////////////////////////

	/*
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate.
	 */
	void TurnAtRate(float Rate);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseTurnRate;


	/*
	 * Called via input to look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate.
	 */
	void LookUpRate(float Rate);

	/** Base look up/down rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseLookUpRate;

	/*
	* Called via input to look left/right at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate.
	*/
	void AddControllerYawInput(float Rate) override;

	/*
	* Called via input to look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate.
	*/
	void AddControllerPitchInput(float Rate) override;

	/** Used to aim ironsights/hipfire blendspaces */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement/Camera", Replicated)
		float AimPitchValue;

	/** Used to aim ironsights/hipfire blendspaces */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement/Camera", Replicated)
		float AimYawValue;

	// TODO: Unnecessary remove and fix 
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement/Camera", Replicated)
		float ServerAimPitchValue;

	// TODO: Unnecessary remove and fix 
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement/Camera", Replicated)
		float ServerAimYawValue;


	/** Call to set and replicate AimPitch & AimYaw Values */
	UFUNCTION()
		void SetAimOffsetValues();

	/** Replicates AimPitch * AimYaw Values */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetAimOffsetValues(float PitchValue, float YawValue); //Inside this set aimpitchvalue and aimyawvalue I guess


	/** TODO: Take locations and lerp to when ToggleAiming */
	/** LookAt raycast setup */
	FRotator Target;
	FRotator Current;
	FRotator RInterptReturnValue;



	/////////////////////////////////
	////////////  COVER  ////////////
	/////////////////////////////////

public:

	/** Status of cover */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		bool bInCover;

	/** Potential status of cover */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerMovement", Replicated)
		bool bCanCover;

	/** Rotation of cover object */
	FRotator TargetCoverRotation;


	/** Use to enter/exit cover */
	void ToggleCover();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleCover();

	void  ToggleInCover(bool bInCoverStatus);

	/** Call to replicate bInCover */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleInCover(bool bInCoverStatus);

	void ToggleCanCover(bool bCanCoverStatus);

	/** Call to replicate bInCover */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleCanCover(bool bCanCoverStatus);

	void IsCoverAvailable(bool bCoverAvailable, FRotator CoverRotation);



	//////////////////////////////////
	/////////////  MISC  /////////////
	//////////////////////////////////

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Mark the properties we wish to replicated
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const override;

	/** Temporarily used to test function calls */
	void TestPrint();

};
