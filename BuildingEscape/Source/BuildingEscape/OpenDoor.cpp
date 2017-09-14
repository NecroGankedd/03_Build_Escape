// Pirate This Shit

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT


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
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate"), *GetOwner()->GetName());
	}

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	OnOpenRequest.Broadcast();
	//Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() >= 40.f) // TODO Make into a parameter 
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find overlapping Actors
	TArray<AActor*>OverlappingActors;
	if (!PressurePlate) {  return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through.
	for (const auto* Actor : OverlappingActors) 
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Actor Added: %s"), *Actor->GetName());
	}

	return TotalMass;
}

