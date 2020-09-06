// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	FString OwnerName = GetOwner()->GetName();
	FVector OnwerPosition = GetOwner()->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("WorldPosition component is attached to %s and its position is: %s"), *OwnerName, *OnwerPosition.ToCompactString());

	/*
		Another way to get actors position, by getting transform of the actor and getting its location respectivelly
		
		FVector OwnerPosition2 = GetOwner()->GetActorTransform().GetLocation();
		UE_LOG(LogTemp, Warning, TEXT("WorldPosition component is attached to %s and its position is: %s"), *OwnerName, *OwnerPosition2.ToCompactString());
	*/
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

