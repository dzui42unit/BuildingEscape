// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float YawDelta = 90;
	
	float TargetYaw;
	float StartingYaw;
	float DoorLastOpenedTime = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 1;

	UPROPERTY(EditAnywhere)
	ATriggerVolume*	PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor*			ActorThatOpened;

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void	TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void			OpenDoor(float DeltaTime);
	void			CloseDoor(float DeltaTime);
};
