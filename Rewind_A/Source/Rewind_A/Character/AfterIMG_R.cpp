// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterIMG_R.h"

// Sets default values
AAfterIMG_R::AAfterIMG_R()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//m_PoseCopy = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseCopy"));
	//SetRootComponent(m_PoseCopy);

	//ConstructorHelpers::FObjectFinder<UMaterial> mtrl(TEXT("MaterialInstanceConstant'/Game/Rewind/Material/MI_FieldRadiusPreview1'"));
	//if (mtrl.Succeeded())
	//{
	//	m_DefaltMtrl = mtrl.Object;
	//}

}

// Called when the game starts or when spawned
void AAfterIMG_R::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAfterIMG_R::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAfterIMG_R::SetSkeletalMeshComponent(USkeletalMeshComponent* _SKMC)
{
	m_PoseCopy->SetSkeletalMesh(_SKMC->SkeletalMesh);
	m_PoseCopy->CopyPoseFromSkeletalComponent(_SKMC);

	// ���� ��ü�� ����ϴ� ���� ������ Ȯ��
	UINT iMtrlCount = m_PoseCopy->GetNumMaterials();

	// ���� ��ü�� �Ȱ��� �� �������� ������ ����( �������� ���� ����, �� ���ͺ��� �ٸ� ȿ���� ���� ��Ȳ�� ����)
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		m_PoseCopy->CreateDynamicMaterialInstance(i, m_DefaltMtrl);
	}
}

