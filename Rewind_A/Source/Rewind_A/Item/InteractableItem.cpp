// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Character/P_Character.h"

// Sets default values
AInteractableItem::AInteractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HoldingDistance = 150.0f;
    VerticalOffset = 20.0f;
    HorizontalOffset = 0.0f;

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    RootComponent = ItemMesh;


    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Rewind/Item/SM_Chest_01a.SM_Chest_01a'"));
    if (MeshAsset.Succeeded())
    {
        ItemMesh->SetStaticMesh(MeshAsset.Object);
    }


    BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
    BoxCollider->SetupAttachment(ItemMesh);
    BoxCollider->SetBoxExtent(FVector(300, 300, 200));
    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AInteractableItem::OnOverlapBegin);
    BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AInteractableItem::OnOverlapEnd);

}

void AInteractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AP_Character* Character = Cast<AP_Character>(OtherActor);
    if (Character)
    {
        Character->EnableInteraction(this);
        //UE_LOG(LogTemp, Log, TEXT("OVERLAP"));
    }
}

void AInteractableItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AP_Character* Character = Cast<AP_Character>(OtherActor);
    if (Character)
    {
        Character->DisableInteraction();
    }
}


void AInteractableItem::OnInteract(USceneComponent* AttachTo)
{
    if (!AttachTo) return;

    UE_LOG(LogTemp, Warning, TEXT("OnInteract() called"));
    if (ItemMesh->GetAttachParent() == nullptr)
    {
        ItemMesh->SetSimulatePhysics(false);
        FVector Offset = AttachTo->GetForwardVector() * HoldingDistance + AttachTo->GetUpVector() * VerticalOffset;
        FVector NewLocation = AttachTo->GetComponentLocation() + Offset;
        FRotator NewRotation = AttachTo->GetComponentRotation();

        FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
        AttachToComponent(AttachTo, AttachmentRules);
        ItemMesh->SetWorldLocationAndRotation(NewLocation, NewRotation);
    }
    else
    {
        FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
        DetachFromActor(DetachmentRules);
        ItemMesh->SetSimulatePhysics(true);
    }
}



void AInteractableItem::EnableOutline()
{
    ItemMesh->SetRenderCustomDepth(true);
}

void AInteractableItem::DisableOutline()
{
    ItemMesh->SetRenderCustomDepth(false);
}

// Called when the game starts or when spawned
void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

