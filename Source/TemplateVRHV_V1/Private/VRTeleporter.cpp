// Fill out your copyright notice in the Description page of Project Settings.

#include "TemplateVRHV_V1.h"
#include "VRTeleporter.h"


// Sets default values for this component's properties
UVRTeleporter::UVRTeleporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVRTeleporter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVRTeleporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

