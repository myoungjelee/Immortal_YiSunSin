// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMoving.h"
#include <Components/BoxComponent.h>

// Sets default values
AShipMoving::AShipMoving()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(box);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	mesh->SetupAttachment(box);
}

// Called when the game starts or when spawned
void AShipMoving::BeginPlay()
{
	Super::BeginPlay();
	
	rot = GetActorRotation();
}

// Called every frame
void AShipMoving::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float roll = FMath::Sin(GetGameTimeSinceCreation()) * 5;
	FRotator moverot = FRotator(rot.Pitch, rot.Yaw, roll);
	SetActorRotation(moverot);

	// ���� ��ġ ����
	FVector currLoc = GetActorLocation();

	// Z �� ���� (���� ������ �������ϰ�)
	float posZ = FMath::Lerp(currLoc.Z, currLoc.Z+5, FMath::Sin(GetGameTimeSinceCreation()));
	FVector TargetLocation = FVector(currLoc.X, currLoc.Y, posZ);

	// ��ġ ����
	FVector moveLoc = FMath::Lerp(currLoc, TargetLocation, DeltaTime * 5.0f);

	// ���� ��ġ ����
	SetActorLocation(moveLoc);
}

