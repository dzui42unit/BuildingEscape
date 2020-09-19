// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
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

	SetupAudioComponent();

	StartingYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = StartingYaw + YawDelta;

	CheckPressurePlate();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// check if the PressurePlate was initialized to prevent NULL pointer access
	if (!CheckPressurePlate()) {
		return ;
	}

	if (TotalActorsMass() >= MassOpenDoor) {	
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

	if (AudioComponent && DoorClosed) {
		DoorClosed = false;
		AudioComponent->Play();
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
 	FRotator ActorRotation = GetOwner()->GetActorRotation();
	 
	float NewYaw = FMath::FInterpTo(ActorRotation.Yaw, StartingYaw, DeltaTime, DoorCloseSpeed);

	ActorRotation.Yaw = NewYaw;
	GetOwner()->SetActorRelativeRotation(ActorRotation);

	if (AudioComponent && !DoorClosed) {
		DoorClosed = true;
		AudioComponent->Play();
	}	
}

float UOpenDoor::TotalActorsMass() const
{
	float TotalMass = 0.f;

	if (!CheckPressurePlate()) {
		return TotalMass;
	}

	// find all overlapping actors
	TArray<AActor *>	OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);
	// sum up their masses
	for (AActor *Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void	UOpenDoor::SetupAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("Object %s missing audio component"), *GetOwner()->GetName());
	}
}

bool	UOpenDoor::CheckPressurePlate() const
{
	// check if the PressurePlate was set up, just to output message to the Log
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Actor %s has a OpenDoor component set up, but not PressurePlate set"), *(GetOwner()->GetName()));
		return false;
	}
	return true;
}