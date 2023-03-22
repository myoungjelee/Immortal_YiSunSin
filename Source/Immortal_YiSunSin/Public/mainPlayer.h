// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <HeadMountedDisplayTypes.h>
#include "mainPlayer.generated.h"

UCLASS()
class IMMORTAL_YISUNSIN_API AmainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AmainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* cam;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
		class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* controllerLeft;

	UPROPERTY(EditAnywhere)
		class UMotionControllerComponent* controllerRight;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
		class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
		class USkeletalMeshComponent* rightHand;

	//Input
	UPROPERTY(EditAnywhere)
		class UInputMappingContext* inputMapping;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* thumbstickLeft;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* thumbstickRight;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* triggerLeft;
	
	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* triggerRight;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* btnX;

	UPROPERTY(EditAnywhere, Category = Inputs)
		FColor debugColor;

public:
	void Recenter();
	void Move(const struct FInputActionValue& value);
	void RotateAxis(const struct FInputActionValue& value);
	void OnTriggerLeft();
	void OnTriggerRight();
	void OpenWidget();
};