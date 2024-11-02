// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightComponent.h"

#include "Components/SpotLightComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"


UFlashlightComponent::UFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructFlashlight();
}

bool UFlashlightComponent::ConstructFlashlight()
{
	if(AddSpringArm()) if(AddSpotLight()) return true;
	return false;
}

bool UFlashlightComponent::AddSpringArm()
{
	SpringArm = GetOwner()->AddComponentByClass(USpringArmComponent::StaticClass(), true, FTransform(), true);

	if (SpringArm) SpringArm->RegisterComponent();
	else UE_LOG(LogTemp, Error, TEXT("SpringArm not created"));

	// Cast<USceneComponent>(SpringArm)->SetupAttachment(
	// 	Cast<ACharacter>(GetOwner())->GetCapsuleComponent());
	Cast<USpringArmComponent>(SpringArm)->TargetArmLength = 0.0f;
	Cast<USpringArmComponent>(SpringArm)->bUsePawnControlRotation = true;
	Cast<USpringArmComponent>(SpringArm)->SetWorldLocation(FVector(0.0f, 20.0f, 40.0f));
	Cast<USpringArmComponent>(SpringArm)->bEnableCameraRotationLag = true;

	Cast<USceneComponent>(SpringArm)->AttachToComponent(
		Cast<USceneComponent>(GetOwner()->GetRootComponent()), FAttachmentTransformRules::KeepRelativeTransform);
	return true;
}

bool UFlashlightComponent::AddSpotLight()
{
	SpotLight = GetOwner()->AddComponentByClass(USpotLightComponent::StaticClass(), true, FTransform(), true);
	if (SpotLight) SpotLight->RegisterComponent();
	else UE_LOG(LogTemp, Error, TEXT("SpotLight not created"));
	Cast<USceneComponent>(SpotLight)->AttachToComponent(Cast<USceneComponent>(SpringArm), FAttachmentTransformRules::KeepRelativeTransform);
	if(MaskMaterial)
	{
		Cast<USpotLightComponent>(SpotLight)->SetLightFunctionMaterial(MaskMaterial);
	}
	return true;
}

void UFlashlightComponent::ToggleFlashlight()
{
	if (bFlashLightOn) {
		Cast<USceneComponent>(SpringArm)->SetVisibility(false, true);
		bFlashLightOn = false;
	} else {
		Cast<USceneComponent>(SpringArm)->SetVisibility(true, true);
		bFlashLightOn = true;
	}
}