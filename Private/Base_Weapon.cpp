// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Weapon.h"


// Sets default values
ABase_Weapon::ABase_Weapon()
{
 
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	// Alter rotation in derived bp event graph when in hipfire vs aimming?

}

// Called when the game starts or when spawned
void ABase_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*

void ABase_Weapon::Fire()
{

	// Get Location of tip of gun each time weapon is equipped

	//check 
	if(Has ammo)
	{
		// raytrace from tip of weapon location forward vector

		// subtract ammo


	}else
	{
		Reload();
	}



}
*/

