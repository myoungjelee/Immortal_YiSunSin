// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyShip.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
AEnemyShip::AEnemyShip()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//LineTrace �� ������ �ǰ� ����
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX")); 
	boxCollision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	boxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//FSM ������Ʈ �߰�
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	//Auto possess ai ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyShip::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called every frame
void AEnemyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyShip::SetActive(bool bActive)
{
	// Ȱ��ȭ bActive == true
	if (bActive)
	{
		//�浹 Ȱ��
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	// ��Ȱ��ȭ bActive == false
	else
	{
		//�浹 ��Ȱ��
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//źâ�� �� �ٽ� �־���
		dieDelegate.ExecuteIfBound(this);
	}

	//�޽��� Ȱ�� / ��Ȱ��
	GetMesh()->SetActive(bActive);
	//�޽��� ���̰� / �Ⱥ��̰�
	GetMesh()->SetVisibility(bActive);
	//ĳ���� �����Ʈ Ȱ�� / ��Ȱ��
	GetCharacterMovement()->SetActive(bActive);
	//fsm Ȱ�� / ��Ȱ��
	if (fsm != nullptr)
	{
		fsm->SetActive(bActive);
	}
}
