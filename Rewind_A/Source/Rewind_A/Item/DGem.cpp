// Fill out your copyright notice in the Description page of Project Settings.


#include "DGem.h"

ADGem::ADGem()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Rewind/Maps/Fire/FBX/item/FireGolem_eye'"));
    if (MeshAsset.Succeeded())
    {
        ItemMesh->SetStaticMesh(MeshAsset.Object);

    }
}
