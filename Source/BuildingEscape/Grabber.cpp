// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetInputHandleComponent();
}

// find the physics handle and initialize it
void UGrabber::FindPhysicsHandleComponent()
{	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Physics component was Not found"));
	}
}

// set up input component
void UGrabber::SetInputHandleComponent()
{
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabAction);
		InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::DropAction);
	}
}

FHitResult	UGrabber::GetFirstPhysicsBodyInReach()
{
	// get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;

	/* degub line 
		DrawDebugLine(
			GetWorld(),
			PlayerViewPointLocation,
			LineTraceEnd,
			FColor(0, 255, 0),
			false,
			0.f,
			0,
			5
		);
	*/

	// Performe ray-casting to reach some objects
	// returns the result of the ray-caster hit
	FHitResult Hit;
	// structure that contains the list of object types the query is interested in
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPosition(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {
		return ;
	}

	// if we have grabbed component
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FVector UGrabber::GetPlayersReach() const
{
	// get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	return (PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach);
}

FVector UGrabber::GetPlayersWorldPosition() const
{
	// get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	return (PlayerViewPointLocation);
}

void	UGrabber::GrabAction()
{
	// try to reach any actor with a physics collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	// if we hit - attach physics handle
	if (HitResult.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("Body %s reached"), *HitResult.GetActor()->GetName());
		if (!PhysicsHandle) {
			return ;
		}
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);
	}
}

void	UGrabber::DropAction()
{
	if (!PhysicsHandle) {
		return ;
	}
	// TODO remove physics handle
	PhysicsHandle->ReleaseComponent();
}