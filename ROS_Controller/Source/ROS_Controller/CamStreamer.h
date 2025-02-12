// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ActorComponent.h"
#include "CamStreamer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCamStreamer : public UActorComponent
{
    GENERATED_BODY()

public:
    UCamStreamer();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    bool GetCameraPixelData();
    void SendUDPData();

    TArray<FColor> CameraPixelData;
	
	USceneCaptureComponent2D* CaptureComponent;
	
    UTextureRenderTarget2D* CameraRenderTarget;
    int32 CameraImageWidth;
    int32 CameraImageHeight;
    int32 CameraImageSize;
    int UdpSocket;
    sockaddr_in ServerAddress;
};