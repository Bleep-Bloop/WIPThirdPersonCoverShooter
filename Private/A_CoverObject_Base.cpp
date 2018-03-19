// Fill out your copyright notice in the Description page of Project Settings.

#include "A_CoverObject_Base.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine.h"
#include "C_Player_Jax.h"




// Sets default values
AA_CoverObject_Base::AA_CoverObject_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));

	/////BOTH OBJECTS NEED bGenerateOverlapEvents active so double check player

	// These are created but still need to be placed properly around mesh inside derived blueprint
	BoxComp1 = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverBox1"));
	BoxComp2 = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverBox2"));
	BoxComp3 = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverBox3"));
	BoxComp4 = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverBox4"));

	Arrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow1"));
	Arrow1->AttachToComponent(BoxComp1, FAttachmentTransformRules::KeepRelativeTransform);
	Arrow2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow2"));
	Arrow2->AttachToComponent(BoxComp2, FAttachmentTransformRules::KeepRelativeTransform);
	Arrow3 = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow3"));
	Arrow3->AttachToComponent(BoxComp3, FAttachmentTransformRules::KeepRelativeTransform);
	Arrow4 = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow4"));
	Arrow4->AttachToComponent(BoxComp4, FAttachmentTransformRules::KeepRelativeTransform);

	
}

void AA_CoverObject_Base::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// RESEARCH: Interface setup (not working)
	//II_CoverObject* TheInterface = Cast<II_CoverObject>(ThisCoverObject);
	//if (TheInterface)
	//{
		//Don't call your functions directly, use the 'Execute_' prefix
		//the Execute_ReactToHighNoon and Execute_ReactToMidnight are generated on compile
		//you may need to compile before these functions will appear
		//TheInterface->Execute_ReactToHighNoon(pointerToAnyUObject);
		//TheInterface->Execute_ReactToMidnight(pointerToAnyUObject);	
	//}

	OverlappedPlayerCharacter = Cast<AC_Player_Jax>(OtherActor);

	if ((OverlappedComp == BoxComp1) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		FRotator ArrowRotation = Arrow1->GetComponentRotation();
		OverlappedPlayerCharacter->IsCoverAvailable(true, ArrowRotation);

	}
	else if ((OverlappedComp == BoxComp2) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		FRotator ArrowRotation = Arrow2->GetComponentRotation();
		OverlappedPlayerCharacter->IsCoverAvailable(true, ArrowRotation);
	}
	else if ((OverlappedComp == BoxComp3) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		FRotator ArrowRotation = Arrow3->GetComponentRotation();
		OverlappedPlayerCharacter->IsCoverAvailable(true, ArrowRotation);
	}
	else if ((OverlappedComp == BoxComp4) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		FRotator ArrowRotation = Arrow4->GetComponentRotation();
		OverlappedPlayerCharacter->IsCoverAvailable(true, ArrowRotation);
	}

}


void AA_CoverObject_Base::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{



	if ((OverlappedComp == BoxComp1) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		OverlappedPlayerCharacter->ToggleCanCover(false);
	}
	else if ((OverlappedComp == BoxComp2) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		OverlappedPlayerCharacter->ToggleCanCover(false);
	}
	else if ((OverlappedComp == BoxComp3) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		OverlappedPlayerCharacter->ToggleCanCover(false);
	}
	else if ((OverlappedComp == BoxComp4) && (OtherActor != nullptr) && (OtherActor->IsA<AC_Player_Jax>()) && (OtherComp != nullptr) && (OverlappedPlayerCharacter != nullptr))
	{
		OverlappedPlayerCharacter->ToggleCanCover(false);
	}

}


// Called when the game starts or when spawned
void AA_CoverObject_Base::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp1->bGenerateOverlapEvents = true;
	BoxComp2->bGenerateOverlapEvents = true;
	BoxComp3->bGenerateOverlapEvents = true;
	BoxComp4->bGenerateOverlapEvents = true;

	BoxComp1->OnComponentBeginOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapBegin);
	BoxComp2->OnComponentBeginOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapBegin);
	BoxComp3->OnComponentBeginOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapBegin);
	BoxComp4->OnComponentBeginOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapBegin);

	BoxComp1->OnComponentEndOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapEnd);
	BoxComp2->OnComponentEndOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapEnd);
	BoxComp3->OnComponentEndOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapEnd);
	BoxComp4->OnComponentEndOverlap.AddDynamic(this, &AA_CoverObject_Base::OnOverlapEnd);

	


}


// Called every frame
void AA_CoverObject_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
bool AA_CoverObject_Base::AvailableCover_Implementation(bool IsCoverAvailable, FRotator CoverRotation)
{
	// Default behaivour  for how this object would react
	// Toggle bCoverAvailable on character
	

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(15, 5.f, FColor::Blue, "Finally");
	}

	return true;
}
*/
