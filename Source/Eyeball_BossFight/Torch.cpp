// Fill out your copyright notice in the Description page of Project Settings.

#include "Torch.h"
#include "Components/PointLightComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATorch::ATorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	torchColourTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TorchColourTimeline"));
	TorchColourInterpFunction.BindUFunction(this, FName("TorchColourTimelineFloatReturn"));

	torchIntensityTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TorchIntensityTimeline"));
	TorchIntensityInterpFunction.BindUFunction(this, FName("TorchIntensityTimelineFloatReturn"));
}

void ATorch::ChangeTorchColour(FLinearColor colour)
{
	torchColour = colour;
	torchColourTimeline->PlayFromStart();
}

void ATorch::ChangeTorchIntensity(float intensity)
{
	torchIntensity = intensity;
	torchIntensityTimeline->PlayFromStart();
}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();

	if (colourChangeCurve)
	{
		torchColourTimeline->AddInterpFloat(colourChangeCurve, TorchColourInterpFunction, FName("Alpha"));

		torchColour = FindComponentByClass<UPointLightComponent>()->GetLightColor();

		//Setting Timeline settings
		torchColourTimeline->SetLooping(false);
		torchColourTimeline->SetIgnoreTimeDilation(true);
	}

	if (intensityChangeCurve)
	{
		torchIntensityTimeline->AddInterpFloat(intensityChangeCurve, TorchIntensityInterpFunction, FName("Alpha"));

		torchIntensity = FindComponentByClass<UPointLightComponent>()->AttenuationRadius;

		//Setting Timeline settings
		torchIntensityTimeline->SetLooping(false);
		torchIntensityTimeline->SetIgnoreTimeDilation(true);
	}
}

void ATorch::TorchColourTimelineFloatReturn(float value)
{
	FLinearColor torchLightColour = FindComponentByClass<UPointLightComponent>()->GetLightColor();
	FLinearColor lerpColour = UKismetMathLibrary::LinearColorLerp(torchLightColour, torchColour, value);

	FindComponentByClass<UPointLightComponent>()->SetLightColor(lerpColour);
}

void ATorch::TorchIntensityTimelineFloatReturn(float value)
{
	float currentTorchIntensity = FindComponentByClass<UPointLightComponent>()->AttenuationRadius;
	float lerpIntensity = UKismetMathLibrary::Lerp(currentTorchIntensity, torchIntensity, value);

	FindComponentByClass<UPointLightComponent>()->SetAttenuationRadius(lerpIntensity);
}
