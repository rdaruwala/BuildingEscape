// Copyright Rohan 2020

#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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
	FindPhysicsHandle();

	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if(!Input){
		UE_LOG(LogTemp, Error, TEXT("No Input Component is present on %s"), *GetOwner()->GetName());
	}

	Input->BindAction("Grabber", IE_Pressed, this, &UGrabber::Grab);
	Input->BindAction("Grabber", IE_Released, this, &UGrabber::Release);
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If physics handle is attached, move the object

	FVector PlayerLocation;
	FRotator PlayerRotation;

	FVector LineTraceEnd = GetLineTraceEnd(PlayerLocation, PlayerRotation);

	if(PhysicsHandle->GrabbedComponent){
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

	

}

void UGrabber::Grab(){
	UE_LOG(LogTemp, Warning, TEXT("Grab!"));

	FHitResult Hit;
	
	FVector LineTraceEnd = CheckForHit(OUT Hit);

	UPrimitiveComponent *ComponentToGrab = Hit.GetComponent();

	if(Hit.GetActor()){
		PhysicsHandle->GrabComponentAtLocation(
		ComponentToGrab,
		NAME_None,
		LineTraceEnd
	);

	}
}

void UGrabber::Release(){
	UE_LOG(LogTemp, Warning, TEXT("Release!"));

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle(){
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle){
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component is present on %s"), *GetOwner()->GetName());
	}
}

FVector UGrabber::CheckForHit(FHitResult &Hit){
	
	FVector PlayerLocation;
	FRotator PlayerRotation;

	FVector LineTraceEnd = GetLineTraceEnd(PlayerLocation, PlayerRotation);


	bool bGotAHit = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECC_PhysicsBody),
		FCollisionQueryParams(FName(), true, GetOwner())
	);

	return LineTraceEnd;
}

FVector UGrabber::GetLineTraceEnd(FVector &PlayerLocation, FRotator &PlayerRotation){

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation, 
		OUT PlayerRotation	
	);

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	return LineTraceEnd;
}