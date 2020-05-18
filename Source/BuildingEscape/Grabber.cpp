// Copyright Rohan 2020

#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Check for Physics Handle Component

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle){
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component is present on %s"), *GetOwner()->GetName());
	}

	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if(!Input){
		//UE_LOG(LogTemp, Error, TEXT("No Input Component is present on %s"), *GetOwner()->GetName());

		UE_LOG(LogTemp, Error, TEXT("No Input Component is present on %s"), *GetOwner()->GetName());
	}

	Input->BindAction("Grabber", IE_Pressed, this, &UGrabber::Grab);
	Input->BindAction("Grabber", IE_Released, this, &UGrabber::Release);
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get player's viewpoint

	// Ray-cast out a certain distance

	// See what it hits

	FVector PlayerLocation;
	FRotator PlayerRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation, 
		OUT PlayerRotation	
	);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		LineTraceEnd,
		FColor(0,255,0),
		false,
		0.0f,
		0,
		5
	);

	FHitResult Hit;

	bool bGotAHit = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECC_PhysicsBody),
		FCollisionQueryParams(FName(), true, GetOwner())
	);

	if(bGotAHit){
		UE_LOG(LogTemp, Warning, TEXT("Hit the actor: %s"), *Hit.Actor->GetName());
	}


}

void UGrabber::Grab(){
	UE_LOG(LogTemp, Warning, TEXT("Grab!"));
}

void UGrabber::Release(){
	UE_LOG(LogTemp, Warning, TEXT("Release!"));
}

