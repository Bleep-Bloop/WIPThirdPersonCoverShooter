// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_CoverObject_Base.generated.h"


class UBoxComponent;
class UArrowComponent;
class AC_Player_Jax;

UCLASS()
class LETHALSPEED_API AA_CoverObject_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_CoverObject_Base();

	//More interface tests
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerMovement")  //This tells your class that you can both call and override this function in blueprints. You need this part as well if you want to be able to override C++ functionality within BP, as BlueprintNativeEvents are intended to be used.
		//bool AvailableCover(bool IsCoverAvailable, FRotator CoverRotation);
	//virtual bool AvailableCover_Implementation(bool IsCoverAvailable, FRotator CoverRotation) override; // virtual bool ReactToHighNoon_Implementation() override; This line tells your class that it has a function of this name and signature to inherit from the interface, which is how calls to the interface functions are able to interact with this class.
	/* Notice that ReactToMidnight(), the BlueprintImplementableEvent, is not defined here. A BlueprintImplementableEvent is declared (its existance) in our interface, but defined (its behaviour) in blueprints only.*/



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxComp1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxComp2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Components")
		UBoxComponent* BoxComp3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxComp4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* Arrow1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* Arrow2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* Arrow3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* Arrow4;

	AC_Player_Jax* OverlappedPlayerCharacter;


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
