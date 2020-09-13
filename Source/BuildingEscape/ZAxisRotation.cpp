// Fill out your copyright notice in the Description page of Project Settings.


#include "ZAxisRotation.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UZAxisRotation::UZAxisRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UZAxisRotation::BeginPlay()
{
	Super::BeginPlay();

	RotationAngle = 0.5f;
	ComponentOwner = GetOwner();

	// ...
	
}


// Called every frame
void UZAxisRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator CurrentRotation = ComponentOwner->GetActorRotation();
	CurrentRotation.Yaw += 0.5f;
	ComponentOwner->SetActorRotation(CurrentRotation);
}

