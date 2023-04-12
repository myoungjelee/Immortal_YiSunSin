// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PlayerBase.generated.h"

UCLASS()
class IMMORTAL_YISUNSIN_API APlayerBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UCapsuleComponent* rootComp;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UCameraComponent* cam;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UMotionControllerComponent* leftController;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* leftLog;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UMotionControllerComponent* rightController;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UTextRenderComponent* rightLog;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|ModuleSetting")
	TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UWidgetInteractionComponent* widgetInt_L;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Components")
	class UWidgetInteractionComponent* widgetInt_R;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Input")
	class UInputAction* triggerRight;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Input")
	class UInputAction* triggerLeft;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Input")
	class UInputAction* btnX;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Input")
		class UInputAction* rightThumbstick;

	UPROPERTY(EditAnywhere, Category = "VR_Setting|Input")
		class UInputAction* leftThumbstick;

	void PressWidget_L();
	void ReleaseWidget_L();
	
	void PressWidget_R();
	void ReleaseWidget_R();

	void Move(const struct FInputActionValue& value);
	void RotateAxis(const struct FInputActionValue& value);

};
