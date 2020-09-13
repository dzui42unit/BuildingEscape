// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	StartingYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = StartingYaw + YawDelta;

	// initialize the ActorThatOpened at the beginning of the game to be able to calculate the overlap when the game starts
	ActorThatOpened = GetWorld()->GetFirstPlayerController()->GetPawn();

	// check if the PressurePlate was set up, just to output message to the Log
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Actor %s has a OpenDoor component set up, but not PressurePlate set"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// check if the PressurePlate was initialized to prevent NULL pointer access
	if (!PressurePlate) {
		return ;
	}

	if (PressurePlate->IsOverlappingActor(ActorThatOpened)) {	
		// get the moment of time when the door started to open
		DoorLastOpenedTime = GetWorld()->GetTimeSeconds();
		OpenDoor(DeltaTime);
	} else {
		// check if DoorCloseDelay time has passed since the door was opened and then close it again
		if (GetWorld()->GetTimeSeconds() - DoorLastOpenedTime >= DoorCloseDelay) {
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
 	FRotator ActorRotation = GetOwner()->GetActorRotation();
	
	// frame rate dependand way to implement linear interpolation
	// float NewYaw = FMath::Lerp(ActorRotation.Yaw, TargetYaw, DeltaTime * 1.0f);
	// framerate independant ways to implement Linear interpolation
	// float NewYaw = FMath::FInterpConstantTo(ActorRotation.Yaw, TargetYaw, DeltaTime, 45);
	float NewYaw = FMath::FInterpTo(ActorRotation.Yaw, TargetYaw, DeltaTime, DoorOpenSpeed);

	ActorRotation.Yaw = NewYaw;
	GetOwner()->SetActorRelativeRotation(ActorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
 	FRotator ActorRotation = GetOwner()->GetActorRotation();
	 
	float NewYaw = FMath::FInterpTo(ActorRotation.Yaw, StartingYaw, DeltaTime, DoorCloseSpeed);

	ActorRotation.Yaw = NewYaw;
	GetOwner()->SetActorRelativeRotation(ActorRotation);
}