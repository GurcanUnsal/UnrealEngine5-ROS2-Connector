#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CarController.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROS_CONTROLLER_API UCarController : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UCarController();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    void MoveForward(float Value);
    void TurnRight(float Value);

    void SetLinearVelocity(float XValue);
    void SetAngularVelocity(float ZValue);

private:
    float LinearVelocity;
    float AngularVelocity;
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
