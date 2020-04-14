// Francisco Viramontes 2020

#include "OpenDoor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Classes/Components/PrimitiveComponent.h"
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
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Getting the pawn that the player contols
	AActor* ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (GetTotalMassOnPlate() > TriggerMass)
	//if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpeningDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		ClosingDoor();
	}

	/*
	if ((!PressurePlate->IsOverlappingActor(ActorThatOpens)) && (GetWorld()->GetTimeSeconds() + DoorCloseDelay > LastDoorOpenTime))
	{
		ClosingDoor();
	}
	*/
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

float UOpenDoor::GetTotalMassOnPlate() 
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;

	//Updates Overlapping Actors
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (const auto* Actor : OverlappingActors)
	{
		UPrimitiveComponent* OverlappedActor = Actor->FindComponentByClass<UPrimitiveComponent>();
		TotalMass += OverlappedActor->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s found on the plate"), *i->GetName());
	}

	//UE_LOG(LogTemp, Warning, TEXT("%f mass found"), TotalMass);
	return TotalMass;
}