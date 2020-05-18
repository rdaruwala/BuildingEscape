// Copyright Rohan 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.0f;
	float InitialYaw;
	float CurrentYaw;

	float DoorLastOpened = 0;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeedTime = 2;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeedTime = 2;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor *ActorThatOpens;

	bool onPressurePad = false;
};
