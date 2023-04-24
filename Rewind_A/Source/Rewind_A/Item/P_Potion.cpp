// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Potion.h"
#include "../Character/P_Character.h"
#include <Engine/Classes/Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AP_Potion::AP_Potion()
{
 //	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//ConstructorHelpers::FObjectFinder<AActor> mesh(TEXT("SkeletalMesh'/Game/Sci_Fi_Character_08/Mesh/Character/SK_Sci_Fi_Character_08_Full_01'"));

	//if (mesh.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(mesh.Object);

	//}



	//CanPickUpVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CanPickUpVolume"));
	//RootComponent = CanPickUpVolume;
	//CanPickUpVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AP_Potion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP_Potion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

