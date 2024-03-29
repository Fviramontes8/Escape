// Francisco Viramontes 2020

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE__API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FindPhysicsComponent();
	void FindInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach;
	UPhysicsHandleComponent* PhysicsHandle;
	UInputComponent* InputComponent;

	void Grab();
	void Release();

	//Hits for first physics in reach
	const FHitResult GetPhysicsBodyInReach(bool bDebug);
	FTwoVectors GetLineTracePoints(bool bDebug) const;

};
