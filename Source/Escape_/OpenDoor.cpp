// Francisco Viramontes 2020

#include "OpenDoor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/World.h"


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

	//Getting object details
	Owner = GetOwner();

	//Getting the pawn that the player contols
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpeningDoor()
{

	//Make the object rotate 80 degrees on the x-axis
	Rotation = FRotator(0.0f, OpenAngle, 0.0f);
	Owner->SetActorRotation(Rotation);
}

void UOpenDoor::ClosingDoor()
{
	//Sets rotation to 0 degrees and all axis
	Rotation = FRotator(0.0f, 0.0f, 0.0f);
	Owner->SetActorRotation(Rotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpeningDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if ((!PressurePlate->IsOverlappingActor(ActorThatOpens)) && (GetWorld()->GetTimeSeconds() + DoorCloseDelay > LastDoorOpenTime))
	{
		ClosingDoor();
	}
}

