// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Weapon.generated.h"

//Add Struct and enum 

UCLASS()
class LETHALSPEED_API ABase_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons/Components")
		USkeletalMeshComponent* WeaponMesh;


	//void Fire();

	/*UFUNCTION(Server, Reliable, WithValidation)
		void Server_Fire()*/


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
