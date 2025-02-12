#include "CarController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/FloatingPawnMovement.h"

UCarController::UCarController()
{
    PrimaryComponentTick.bCanEverTick = true;
    LinearVelocity = 0.0f;
    AngularVelocity = 0.0f;
}

void UCarController::BeginPlay()
{
    Super::BeginPlay();
}

void UCarController::SetLinearVelocity(float XValue)
{
    LinearVelocity = XValue;
}

void UCarController::SetAngularVelocity(float ZValue)
{
    AngularVelocity = ZValue;
}

void UCarController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    MoveForward(LinearVelocity);
    TurnRight(AngularVelocity);
}

void UCarController::MoveForward(float Value)
{
    if (AActor* Owner = GetOwner())
    {
        FVector ForwardDirection = Owner->GetActorForwardVector();
        Owner->AddActorWorldOffset(ForwardDirection * Value * GetWorld()->DeltaTimeSeconds, true);
    }
}

void UCarController::TurnRight(float Value)
{
    if (AActor* Owner = GetOwner())
    {
        FRotator NewRotation = Owner->GetActorRotation();
        NewRotation.Yaw += Value * GetWorld()->DeltaTimeSeconds;
        Owner->SetActorRotation(NewRotation);
    }
}
