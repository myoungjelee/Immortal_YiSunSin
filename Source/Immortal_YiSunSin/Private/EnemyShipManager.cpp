// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShipManager.h"
#include "EnemyShip.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyShipManager::AEnemyShipManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FClassFinder<AEnemyShip> tempEnemy(TEXT("/Script/Engine.Blueprint'/Game/GN_Blueprint/MG4/BP_enemyShip.BP_enemyShip_C'"));
	if (tempEnemy.Succeeded())
	{
		enemyFactory = tempEnemy.Class;
	}
}

void AEnemyShipManager::BeginPlay()
{
	Super::BeginPlay();
	
	spawnTime = FMath::RandRange(minTime, maxTime);

	//spawn pos ����
	FindSpawnPos();

	for (int32 i = 0; i < 10; i++)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//enemy ����
		AEnemyShip* e = GetWorld()->SpawnActor<AEnemyShip>(enemyFactory, GetActorLocation(), GetActorRotation(), param);
		//���� �׾��� �� ȣ��Ǵ� �Լ� ���
		e->dieDelegate.BindUObject(this, &AEnemyShipManager::AddEnemy);
		//���� ��Ȱ��ȭ ����
		e->SetActive(false);
	}
}

// Called every frame
void AEnemyShipManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//���࿡ �� źâ ������ 0�̸�
	if (enemyArray.Num() == 0)
	{
		return;
	}
	currTime += DeltaTime;
	if (currTime > spawnTime)
	{
		AActor* spawn = spawnPosArray[spawnIdx];

		//1. ������ yaw �� ������( 0 ~ 360 )4
		float yaw = FMath::RandRange(0.0f, 360.0f);
		//2. ���� ���� spawnPosArray �� yaw ���� ����
		spawn->SetActorRotation(FRotator(0, yaw, 0));
		//3. ������ ���� ������( 0 ~ 500 )
		float length = FMath::RandRange(300.0f, 1000.0f);
		//4. spawnPosArray �չ��� * ���� ����
		FVector pos = spawn->GetActorForwardVector() * length;
		//5. 4������ ���� ���� + spawnPosArray �� ��ġ		

		//�� ��ġ, ȸ�� ����
		enemyArray[0]->SetActorLocation(spawn->GetActorLocation() + pos);
		enemyArray[0]->SetActorRotation(spawn->GetActorRotation());
		//�� źâ���� 0��° �� Ȱ��ȭ 
		enemyArray[0]->SetActive(true);
		//enemyArray ���� ����
		enemyArray.RemoveAt(0);

		//0 ~ ���� �������
		//���� �ڸ��� ����
		spawnIdx++;
		spawnIdx = spawnIdx % spawnPosArray.Num();

		//����ð� �ʱ�ȭ
		currTime = 0;
		//�����ð� ���Ӱ� ����
		spawnTime = FMath::RandRange(minTime, maxTime);
	}
}

void AEnemyShipManager::FindSpawnPos()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);

	for (int32 i = 0; i < actors.Num(); i++)
	{
		//���࿡ �̸��� SpawnPos �� ���ԵǾ� �ִٸ�
		if (actors[i]->GetName().Contains(TEXT("SpawnPos")))
		{
			//spawnPosArray �� �߰��Ѵ�
			spawnPosArray.Add(actors[i]);
		}
	}
}

void AEnemyShipManager::AddEnemy(class AEnemyShip* e)
{
	enemyArray.Add(e);
}

