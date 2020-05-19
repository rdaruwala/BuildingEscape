// Copyright Rohan 2020


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
	
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("Actor %s has OpenDoor but no pressure plate"), *GetOwner()->GetName());
	}

	FindAudioComponent();

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate && TotalMassOfActors() >= MassToOpenDoor){
		OpenDoor(DeltaTime);

		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else{
		if(DoorLastOpened + DoorCloseDelay < GetWorld()->GetTimeSeconds()){
			CloseDoor(DeltaTime);
		}
		
	}
	

	// ...
}

void UOpenDoor::OpenDoor(float DeltaTime){
	FRotator Rotation = GetOwner()->GetActorRotation();
	CurrentYaw = Rotation.Yaw;

	Rotation.Yaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime , FMath::Abs((OpenAngle - InitialYaw))/DoorOpenSpeedTime);

	GetOwner()->SetActorRotation(Rotation);

	bCloseDoorSound = false;
	if(!AudioComponent){return;}
	if(!bOpenDoorSound){
		AudioComponent->Play();
		bOpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime){
	FRotator Rotation = GetOwner()->GetActorRotation();
	CurrentYaw = Rotation.Yaw;

	Rotation.Yaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime , FMath::Abs((OpenAngle - InitialYaw))/DoorCloseSpeedTime);

	GetOwner()->SetActorRotation(Rotation);

	bOpenDoorSound = false;
	if(!AudioComponent){return;}
	if(!bCloseDoorSound){
		AudioComponent->Play();
		bCloseDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors() const{
	float TotalMass = 0.0f;

	// Find all overlapping actors

	if(!PressurePlate){return -1;}

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for(int32 i = 0; i < OverlappingActors.Num(); i++){
		TotalMass += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();\
	}


	return TotalMass;
}

void UOpenDoor::FindAudioComponent(){
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if(!AudioComponent){
		UE_LOG(LogTemp, Error, TEXT("No Audio Component Found"));
	}
}

