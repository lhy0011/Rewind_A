// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterIMG.h"

// Sets default values
AAfterIMG::AAfterIMG()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PoseCopy = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseCopy"));
	SetRootComponent(m_PoseCopy);

	ConstructorHelpers::FObjectFinder<UMaterial> mtrl(TEXT("Material'/Game/Rewind/Material/MT_Fresnel'"));
	if (mtrl.Succeeded())
	{
		m_DefaltMtrl = mtrl.Object;
	}

}

// Called when the game starts or when spawned
void AAfterIMG::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAfterIMG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAfterIMG::SetSkeletalMeshComponent(USkeletalMeshComponent* _SKMC)
{
	m_PoseCopy->SetSkeletalMesh(_SKMC->SkeletalMesh);
	m_PoseCopy->CopyPoseFromSkeletalComponent(_SKMC);

	// 원본 물체가 사용하는 재질 개수를 확인
	UINT iMtrlCount = m_PoseCopy->GetNumMaterials();

	// 원본 물체랑 똑같이 각 부위별로 재질을 세팅( 동적으로 만들어서 세팅, 각 액터별로 다른 효과가 생길 상황을 염두)
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		m_PoseCopy->CreateDynamicMaterialInstance(i, m_DefaltMtrl);
	}
}

