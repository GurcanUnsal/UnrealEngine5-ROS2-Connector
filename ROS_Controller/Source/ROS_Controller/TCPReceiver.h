// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarController.h"
#include "Components/ActorComponent.h"
#include "TCPReceiver.generated.h"

USTRUCT(BlueprintType)
struct FLinear
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category="Velocity")
    float x;

    UPROPERTY(BlueprintReadWrite, Category="Velocity")
    float y;

    UPROPERTY(BlueprintReadWrite, Category="Velocity")
    float z;
};

USTRUCT(BlueprintType)
struct FAngular
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category="Velocity")
    float x;

    UPROPERTY(BlueprintReadWrite, Category="Velocity")
    float y;

    UPROPERTY(BlueprintReadWrite, Category="Velocity")
    float z;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROS_CONTROLLER_API UTCPReceiver : public UActorComponent
{
    GENERATED_BODY()

public:
    UTCPReceiver();
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void StartTCPReceiver();
    void ListenForConnections();
    void HandleClientConnection(int ClientSocket);

    int ServerSocket;
    bool bIsServerRunning;

	UCarController* Controller;
};
