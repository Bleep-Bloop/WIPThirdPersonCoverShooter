// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCoverObject.h"


// Sets default values
ABaseCoverObject::ABaseCoverObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCoverObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCoverObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

