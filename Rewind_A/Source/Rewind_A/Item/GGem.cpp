// Fill out your copyright notice in the Description page of Project Settings.


#include "GGem.h"

AGGem::AGGem()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Rewind/Maps/Fire/FBX/item/Ice_Gem'"));
    if (MeshAsset.Succeeded())
    {
        ItemMesh->SetStaticMesh(MeshAsset.Object);

    }
}
