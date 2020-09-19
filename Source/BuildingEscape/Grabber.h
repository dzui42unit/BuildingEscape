// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

private:
	float 						Reach = 200.f;
	
	UPROPERTY()
	UPhysicsHandleComponent*	PhysicsHandle = nullptr;
	
	UPROPERTY()
	UInputComponent*			InputHandle = nullptr;
	
	void		 				GrabAction();
	void						DropAction();
	void						FindPhysicsHandleComponent();
	void						SetInputHandleComponent();
	
	FHitResult					GetFirstPhysicsBodyInReach();
	FVector 					GetPlayersReach() const;
	// gets players world position
	FVector						GetPlayersWorldPosition() const;
	
public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
