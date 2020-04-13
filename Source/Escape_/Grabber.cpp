// Francisco Viramontes 2020

#include "Grabber.h"
#include "Engine/World.h"
#include "Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Looks for physics handle
	FindPhysicsComponent();
	//Looks for input handle
	FindInputComponent();
}

void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::FindInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetPhysicsBodyInReach(false);
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab, 
			NAME_None, 
			ComponentToGrab->GetOwner()->GetActorLocation(), 
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerLineTraceEnd = GetLineTracePoints().v2;
		PhysicsHandle->SetTargetLocation(PlayerLineTraceEnd);
	}
}

//Checks if there is an physics body in reach of the player
const FHitResult UGrabber::GetPhysicsBodyInReach(bool debug=false)
{
	FTwoVectors PlayerViewLocationAndLineTraceEnd = GetLineTracePoints();

	FVector PlayerViewPointLocation = PlayerViewLocationAndLineTraceEnd.v1;
	FVector PlayerLineTraceEnd = PlayerViewLocationAndLineTraceEnd.v2;

	if (debug)
	{
		//Creates a line trace end and shows it as a red line in the game world
		DrawDebugLine(
			GetWorld(),
			PlayerViewPointLocation,
			PlayerLineTraceEnd,
			FColor(255, 0, 0),
			false,
			0.0f,
			0.0f,
			10.0f
		);
	}

	//Set up query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("Nothing")), false, GetOwner());

	//Ray-cast out to reach distance
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		LineTraceHit,
		PlayerViewPointLocation,
		PlayerLineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	if (debug)
	{
		//See what we hit
		AActor* ActorHit = LineTraceHit.GetActor();
		if (ActorHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
		}
	}

	return LineTraceHit;
}

FTwoVectors UGrabber::GetLineTracePoints() const
{
	//x, y, z
	FVector PlayerViewPointLocation;

	//pitch (p), yaw (y), roll (r)
	FRotator PlayerViewPointRotation;

	//Update Location and rotation of the player and wrtie to PlayerViewPointLocation, and PlayerViewPointRotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	//Logs player location and rotation
	bool debug = false;
	if (debug)
	{
		//Convert data to strings
		FString PlayerViewLocationString = PlayerViewPointLocation.ToString();
		FString PlayerViewRotationString = PlayerViewPointRotation.ToString();

		//Print string to log, used for debugging purposes
		UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewLocationString, *PlayerViewRotationString);
	}

	FVector PlayerLineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	return FTwoVectors(PlayerViewPointLocation, PlayerLineTraceEnd);
}