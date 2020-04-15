// Francisco Viramontes 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

//Make sure that this is the last include file in this file
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE__API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpeningDoor();

	void ClosingDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = 80.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.0f;
	float TriggerMass = 9.0f;
	float LastDoorOpenTime;

	//For the rotation of objects
	FRotator Rotation;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	AActor* Owner = nullptr;

	//Returns total mass in kg
	float GetTotalMassOnPlate();
};
