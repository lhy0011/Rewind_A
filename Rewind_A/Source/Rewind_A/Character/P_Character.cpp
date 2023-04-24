// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Character.h"
//#include "../Afterimage.h"

#include "P_AnimInst.h"
#include "AfterIMG.h"
#include "AfterIMG_R.h"
#include "../Item/InteractableItem.h"
#include "../Item/HealingPotion.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

int CCC = 0;

// Sets default values
AP_Character::AP_Character()
    //: m_eState(EPLAYER_STATE::IDLE)
    //, m_AnimInst(nullptr)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Sci_Fi_Character_08/Mesh/Character/SK_Sci_Fi_Character_08_Full_01'"));


    //UE_LOG(LogTemp, Log, mesh__FILE__);


    if (mesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(mesh.Object);
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    }

    // 카메라, 스프링암 추가
    m_pCamera = CreateDefaultSubobject<UCameraComponent>(L"Camera");
    m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(L"SpringArm");

    // 계층 구조 설정
    m_pSpringArm->SetupAttachment(GetMesh());
    m_pCamera->SetupAttachment(m_pSpringArm);

    // 스프링암 위치, 회전, 거리값 설정
    m_pSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 155.f));
    m_pSpringArm->SetRelativeRotation(FRotator(-15.f, 90.f, 0.f));
    m_pSpringArm->TargetArmLength = 200.f;



    // 콜리전
    CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
    CollisionCapsule->InitCapsuleSize(50.f, 65.f); // 반지름, 높이
    CollisionCapsule->SetupAttachment(RootComponent);

    CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AP_Character::OnOverlapBegin);
    CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AP_Character::OnOverlapEnd);



    // 사용할 AnimInstance 설정
    ConstructorHelpers::FClassFinder<UAnimInstance> ABP(TEXT("AnimBlueprint'/Game/Rewind/Animation/ABP_Character.ABP_Character_C'"));

    if (ABP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(ABP.Class);
    }

    //ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(TEXT("/Script/Engine.AnimMontage'/Game/Rewind/Animation/AM_Attack.AM_Attack'"));
    //if (Montage.Succeeded())
    //{
    //    m_arrMontage.Add(Montage.Object);
    //}
    CHP = 10;
    CDamage = 10.f;
    CPotion = 0;


    //cooldown

    RecallUse = 1.f;
    ControlUse = 1.f;
    tp1, tp2, tp3, tp4 = 0.f;

    // UI

    //HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWIDGET"));
    //HPWidget->SetWidgetSpace(EWidgetSpace::Screen);
    //static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Rewind/UI/HP.HP_C'"));
    //if (UI_HUD.Succeeded()) {
    //    HPWidget->SetWidgetClass(UI_HUD.Class);
    //}

  //  static ConstructorHelpers::FClassFinder<UUIMainHUD>


    // 콜리전
    //GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));



    // 상호작용 변수
    InteractionDistance = 200.f;
    CurrentInteractableItem = nullptr;
}


// 플레이어 상태 변환 함수
void AP_Character::ChangeState(EPLAYER_STATE _eNextState, bool _bForce)
{

    if (m_eState == _eNextState) {
        return;
    }

    if (false == _bForce && EPLAYER_STATE::ATTACK == m_eState) {
        return;
    }

    m_eState = _eNextState;

}

void AP_Character::TakeDamage(float DamageAmount)
{
    CHP -= DamageAmount;
    if (CHP <= 0.0f)
    {
        // 캐릭터가 죽는 로직
    }
}


// Called when the game starts or when spawned
void AP_Character::BeginPlay()
{
    Super::BeginPlay();

    m_AnimInst = Cast<UP_AnimInst>(GetMesh()->GetAnimInstance());


    GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &AP_Character::RecallCountNsave, 1.f, true, 1.f);

}


// Called every frame
void AP_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // DeltaTime 얻기
    GetWorld()->GetDeltaSeconds();

    if (0.f == GetCharacterMovement()->Velocity.Size()) // 이동중이 아닌지 판단
    {
        ChangeState(EPLAYER_STATE::IDLE); // idle 상태로 전환
    }

}


// Called to bind functionality to input
void AP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AP_Character::CharacterMoveRight);
    PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &AP_Character::CharacterMoveFront);
    PlayerInputComponent->BindAxis(TEXT("RotationZ"), this, &AP_Character::CharacterRotationZ);
    PlayerInputComponent->BindAxis(TEXT("RotationY"), this, &AP_Character::CharacterRotationY);

    PlayerInputComponent->BindAction(TEXT("TempAttack"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTempAttack);
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterJump);
    PlayerInputComponent->BindAction(TEXT("SaveLocation"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterSaveLocation);
    PlayerInputComponent->BindAction(TEXT("TPLocation1"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL1);
    PlayerInputComponent->BindAction(TEXT("TPLocation2"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL2);
    PlayerInputComponent->BindAction(TEXT("TPLocation3"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL3);
    PlayerInputComponent->BindAction(TEXT("TPLocation4"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTPL4);
    PlayerInputComponent->BindAction(TEXT("TimeControl"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTimeControl);
    PlayerInputComponent->BindAction(TEXT("TimeControl"), EInputEvent::IE_Released, this, &AP_Character::CharacterTimeControlB);
    PlayerInputComponent->BindAction(TEXT("TimeRecall"), EInputEvent::IE_Pressed, this, &AP_Character::CharacterTimeRecall);
    PlayerInputComponent->BindAction(TEXT("PickUp"), EInputEvent::IE_Pressed, this, &AP_Character::Interact);
    PlayerInputComponent->BindAction(TEXT("UsePotion"), EInputEvent::IE_Pressed, this, &AP_Character::UseHealP);
}


void AP_Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bRfromSweep, const FHitResult& Sweep)
{
    //if (OtherActor && (OtherActor != this) && OtherComp) {
    //    CHP -= 1;
    //    if (GEngine) {
    //        //  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
    //    }
    //}
}

void AP_Character::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (GEngine) {
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
    }
}

void AP_Character::EnableInteraction(AInteractableItem* Item)
{
    //UE_LOG(LogTemp, Warning, TEXT("EnableInteraction() called"));
    CurrentInteractableItem = Item;
}

void AP_Character::DisableInteraction()
{
    CurrentInteractableItem = nullptr;
}

void AP_Character::Interact()
{
    //UE_LOG(LogTemp, Warning, TEXT("Interact() called"));

    if (CurrentInteractableItem != nullptr)
    {
        AHealingPotion* HealingPotion = Cast<AHealingPotion>(CurrentInteractableItem);
        if (HealingPotion)
        {
            CPotion++;
            CurrentInteractableItem->Destroy();
        }
        else {
            // Check if the item is already attached to the character.
            if (CurrentInteractableItem->ItemMesh->GetAttachParent() != GetMesh())
            {
                // Attach the item to the character's hand or another desired socket
                CurrentInteractableItem->OnInteract(GetRootComponent());
            }
            else
            {
                // The item is already attached, so handle it accordingly (e.g., use the item, drop the item, etc.).
            }

        }
    }

}


void AP_Character::UseHealP()
{
    UE_LOG(LogTemp, Warning, TEXT("UseHealP() called"));
    if (CPotion > 0) {
        CHP += 2;
        if (CHP > 10) {
            CHP = 10;
        }
        CPotion--;
    }

}

void AP_Character::RecallCountNsave()
{

    CRT = countRtime % 10;

    if (countRtime > 100000) {
        countRtime = 0;
    }

    countRtime += 1;

    switch (CRT) {
    case 0:
        RecallLocation[0] = GetActorLocation();
        SCHP[0] = CHP;
        //UE_LOG(LogTemp, Log, TEXT("0"));
        CCC = 1;
        break;
    case 1:
        RecallLocation[1] = GetActorLocation();
        SCHP[1] = CHP;
        CCC = 2;
        //UE_LOG(LogTemp, Log, TEXT("1"));
        break;
    case 2:
        RecallLocation[2] = GetActorLocation();
        SCHP[2] = CHP;
        CCC = 3;
        // UE_LOG(LogTemp, Log, TEXT("2"));
        break;
    case 3:
        RecallLocation[3] = GetActorLocation();
        SCHP[3] = CHP;
        CCC = 4;
        // UE_LOG(LogTemp, Log, TEXT("3"));
        break;
    case 4:
        RecallLocation[4] = GetActorLocation();
        SCHP[4] = CHP;
        CCC = 5;
        //  UE_LOG(LogTemp, Log, TEXT("4"));
        break;
    case 5:
        RecallLocation[5] = GetActorLocation();
        SCHP[5] = CHP;
        CCC = 6;
        // UE_LOG(LogTemp, Log, TEXT("5"));
        break;
    case 6:
        RecallLocation[6] = GetActorLocation();
        SCHP[6] = CHP;
        CCC = 7;
        //UE_LOG(LogTemp, Log, TEXT("6"));
        break;
    case 7:
        RecallLocation[7] = GetActorLocation();
        SCHP[7] = CHP;
        CCC = 8;
        //UE_LOG(LogTemp, Log, TEXT("7"));
        break;
    case 8:
        RecallLocation[8] = GetActorLocation();
        SCHP[8] = CHP;
        CCC = 9;
        // UE_LOG(LogTemp, Log, TEXT("8"));
        break;
    case 9:
        RecallLocation[9] = GetActorLocation();
        SCHP[9] = CHP;
        CCC = 1;
        // UE_LOG(LogTemp, Log, TEXT("9"));
        break;
    default:
        break;
    }

    //여기서 걍 쿨타임도 처리
    if (canRecall == false) {
        RecallUse += 0.1f;
        if (RecallUse >= RecallCooldown) {
            RecallUse = 1.f;
            canRecall = true;
        }
    }
    if (CanControl == false) {
        ControlUse += 0.01f;
        if (ControlUse >= TimeControlCool) {
            ControlUse = 1.f;
            CanControl = true;
        }
    }



}




float AP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


    return FinalDamage;
}


// 캐릭터 기본 이동
void AP_Character::CharacterMoveFront(float _fScale)
{
    // 기본 이동 속도 50
    AddMovementInput(GetActorForwardVector(),
        50.f * GetWorld()->GetDeltaSeconds() * _fScale);

    if (1.f == _fScale)
    {
        if (90.f == m_AnimInst->GetDirection())
        {
            m_AnimInst->SetDirection(45.f);
        }
        else if (-90.f == m_AnimInst->GetDirection())
        {
            m_AnimInst->SetDirection(-45.f);
        }
        else
        {
            m_AnimInst->SetDirection(0.f);
        }
    }
    else if (-1.f == _fScale)
    {
        if (90.f == m_AnimInst->GetDirection())
        {
            m_AnimInst->SetDirection(135.f);
        }
        else if (-90.f == m_AnimInst->GetDirection())
        {
            m_AnimInst->SetDirection(-135.f);
        }
        else
        {
            m_AnimInst->SetDirection(180.f);
        }
    }

    if (0.f != _fScale) // W/D 가 눌렸다면 ( 이동키가 눌렸다면) 
    {
        ChangeState(EPLAYER_STATE::MOVE);
    }

}

void AP_Character::CharacterMoveRight(float _fScale)
{
    AddMovementInput(GetActorRightVector(),
        50.f * GetWorld()->GetDeltaSeconds() * _fScale);

    if (0.f == _fScale)
    {
        m_AnimInst->SetDirection(0.f);
    }
    else if (1.f == _fScale)
    {
        m_AnimInst->SetDirection(90.f);
    }
    else if (-1.f == _fScale)
    {
        m_AnimInst->SetDirection(-90.f);
    }

    if (0.f != _fScale) // W/D 가 눌렸다면 ( 이동키가 눌렸다면) 
    {
        ChangeState(EPLAYER_STATE::MOVE);
    }
}

void AP_Character::CharacterRotationZ(float _fScale)
{
    AddControllerYawInput(_fScale);
}

void AP_Character::CharacterRotationY(float _fScale)
{
    FRotator rot = m_pSpringArm->GetRelativeRotation();
    float Pitch = rot.Pitch;
    Pitch += 100.f * GetWorld()->GetDeltaSeconds() * _fScale;

    if (Pitch < -45.f)
    {
        Pitch = -45.f;
    }
    else if (Pitch > 45.f)
    {
        Pitch = 45.f;
    }

    rot.Pitch = Pitch;
    m_pSpringArm->SetRelativeRotation(rot);

    rot.Roll;
    rot.Yaw;
}


void AP_Character::CharacterTempAttack()
{
    //if (m_arrMontage.IsEmpty()) {
    //    return;
    //}

    //UE_LOG(LogTemp, Log, TEXT("TempAttack"));

    //ChangeState(EPLAYER_STATE::ATTACK);

    //// 공격 몽타주 재생
    //// m_AnimInst->Montage_JumpToSection(TEXT(""), m_arrMontage[0]);
    //m_AnimInst->Montage_Play(m_arrMontage[0]);
}

void AP_Character::CharacterTimeControl()
{
    UE_LOG(LogTemp, Log, TEXT("TimeControl"));
    if (CanControl) {
        GetWorldSettings()->SetTimeDilation(0.3);
    }

}

void AP_Character::CharacterTimeControlB()
{
    GetWorldSettings()->SetTimeDilation(1.0);

    if (CanControl) {
        CanControl = false;
        ControlUse = 0.f;
    }

}



void AP_Character::CharacterTimeRecall()
{
    UE_LOG(LogTemp, Log, TEXT("TimeRecall"));
    if (canRecall) {

        //FActorSpawnParameters param = {};
        //param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        //param.OverrideLevel = GetLevel();
        //param.bDeferConstruction = false;

        //AAfterIMG_R* PoseCopyInst = GetWorld()->SpawnActor<AAfterIMG_R>(AAfterIMG_R::StaticClass()
        //    , GetMesh()->GetComponentLocation()
        //    , GetMesh()->GetComponentRotation()
        //    , param);
        //PoseCopyInst->SetSkeletalMeshComponent(GetMesh());
        //PoseCopyInst->SetLifeTime(3.f); // 요걸 사용하면 되겟다

        CHP = SCHP[CCC];
        SetActorLocation(RecallLocation[CCC]);
        countRtime = 0;

        canRecall = false;
        RecallUse = 0.f;

    }

}







void AP_Character::CharacterTPL1()
{
    if (!S_Location1.IsEmpty()) {
        CharacterTPLocation(1);
        S_Location1.Reset();
        tp1 = 0.f;
    }
    //CharacterTPLocation(1);
}

void AP_Character::CharacterTPL2()
{
    if (!S_Location2.IsEmpty()) {
        CharacterTPLocation(2);
        S_Location2.Reset();
        tp2 = 0.f;
    }
    // CharacterTPLocation(2);
}

void AP_Character::CharacterTPL3()
{
    if (!S_Location3.IsEmpty()) {
        CharacterTPLocation(3);
        S_Location3.Reset();
        tp3 = 0.f;
    }
    // CharacterTPLocation(3);
}

void AP_Character::CharacterTPL4()
{
    if (!S_Location4.IsEmpty()) {
        CharacterTPLocation(4);
        S_Location4.Reset();
        tp4 = 0.f;
    }
    //  CharacterTPLocation(4);
}

void AP_Character::CharacterSaveLocation()
{
    UE_LOG(LogTemp, Log, TEXT("SaveLocation"));

    SaveCurLocation();

    //AAfterimage::CreateAImg;

}

void AP_Character::CharacterTPLocation(int SN)
{
    UE_LOG(LogTemp, Log, TEXT("TP"));

    int count = 0;

    if (SN == 1) {
        tp1AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location1[0]);
    }
    else if (SN == 2) {
        tp2AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location2[0]);
    }
    else if (SN == 3) {
        tp3AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location3[0]);
    }
    else {
        tp4AIMG->DestroyAA(0.001f);
        SetActorLocation(S_Location4[0]);
    }
}

void AP_Character::CharacterJump()
{
    UE_LOG(LogTemp, Log, TEXT("Jump called"));

    Jump();
}

void AP_Character::SaveCurPose() // AfterIMG
{
    FActorSpawnParameters param = {};
    param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    param.OverrideLevel = GetLevel();
    param.bDeferConstruction = false;

    //AAfterIMG* PoseCopyInst = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
    //    , GetMesh()->GetComponentLocation()
    //    , GetMesh()->GetComponentRotation()
    //    , param);
/*    PoseCopyInst->SetSkeletalMeshComponent(GetMesh());
    PoseCopyInst->SetLifeTime(100.f);*/ // 요걸 사용하면 되겟다

    if (tp1 < 0.5f) {
        tp1AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp1AIMG->SetSkeletalMeshComponent(GetMesh());
        //tp1AIMG->SetLifeTime(50.f);
    }
    else if (tp2 < 0.5f) {
        tp2AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp2AIMG->SetSkeletalMeshComponent(GetMesh());
        //tp2AIMG->SetLifeTime(100.f);
    }
    else if (tp3 < 0.5f) {
        tp3AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp3AIMG->SetSkeletalMeshComponent(GetMesh());
       // tp3AIMG->SetLifeTime(100.f);
    }
    else if (tp4 < 0.5f) {
        tp4AIMG = GetWorld()->SpawnActor<AAfterIMG>(AAfterIMG::StaticClass()
            , GetMesh()->GetComponentLocation()
            , GetMesh()->GetComponentRotation()
            , param);
        tp4AIMG->SetSkeletalMeshComponent(GetMesh());
       // tp4AIMG->SetLifeTime(100.f);
    }
  
}

void AP_Character::SaveCharacterState()
{


}

void AP_Character::SaveCurLocation()
{
    if (S_Location1.IsEmpty()) {
        S_Location1.Add(GetActorLocation());
        SaveCurPose();
        tp1 = 1.f;
       
    }
    else if (S_Location2.IsEmpty()) {
        S_Location2.Add(GetActorLocation());
        SaveCurPose();
        tp2 = 1.f;
    }
    else if (S_Location3.IsEmpty()) {
        S_Location3.Add(GetActorLocation());
        SaveCurPose();
        tp3 = 1.f;
    }
    else if (S_Location4.IsEmpty()) {
        S_Location4.Add(GetActorLocation());
        SaveCurPose();
        tp4 = 1.f;
    }
}

