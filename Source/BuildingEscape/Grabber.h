// Copyright Rohan 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 150.0f;

	UPhysicsHandleComponent *PhysicsHandle = nullptr;

	UInputComponent *Input = nullptr;

	void Grab();
	void Release();

	void FindPhysicsHandle();

	// Both return LineTraceEnd
	FVector CheckForHit(FHitResult &Hit);
	FVector GetLineTraceEnd(FVector &PlayerLocation, FRotator &PlayerRotation);
		
};
