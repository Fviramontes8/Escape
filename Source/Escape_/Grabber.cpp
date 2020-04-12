// Francisco Viramontes 2020

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


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
	UE_LOG(LogTemp, Warning, TEXT("Grabber is here"));
	///Look for physics handle

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input component is found"));
		//Binding input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab called"))
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point, player location as well as player rotation

	//x, y, z
	FVector PlayerViewPointLocation;

	//pitch (p), yaw (y), roll (r)
	FRotator PlayerViewPointRotation;

	//Update Location and rotation of the player
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	//Convert data to strings
	FString PlayerViewLocationString = PlayerViewPointLocation.ToString();
	FString PlayerViewRotationString = PlayerViewPointRotation.ToString();

	//Print string to log, used for debugging purposes
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewLocationString, *PlayerViewRotationString);

	//Creates a line trace end and shows it as a red line in the game world
	FVector PlayerLineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	DrawDebugLine(GetWorld(), 
		PlayerViewPointLocation, 
		PlayerLineTraceEnd, 
		FColor(255, 0, 0), 
		false, 
		0.0f, 
		0.0f, 
		10.0f
	);

	//Set up query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("Nothing")), false, GetOwner());

	//Ray-cast out to reach distance
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(LineTraceHit, 
		PlayerViewPointLocation, 
		PlayerLineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParams
	);

	//See what we hit
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}
}

