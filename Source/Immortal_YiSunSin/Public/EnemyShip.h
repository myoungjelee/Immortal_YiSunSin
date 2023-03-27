// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/InputComponent.h>
#include "EnemyShip.generated.h"

DECLARE_DELEGATE_OneParam(FEnemyShipDieDelegate, class AEnemyShip*);

UCLASS()
class IMMORTAL_YISUNSIN_API AEnemyShip : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
		class UEnemyFSM* fsm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 points;

	// Ȱ��/��Ȱ�� �ϴ� ���
	void SetActive(bool bActive);

	//�׾��� �� ȣ��Ǵ� �Լ��� ��� ����
	FEnemyShipDieDelegate dieDelegate;
};
