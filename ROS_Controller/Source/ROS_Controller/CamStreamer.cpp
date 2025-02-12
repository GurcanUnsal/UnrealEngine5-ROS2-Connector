// Fill out your copyright notice in the Description page of Project Settings.

#include "CamStreamer.h"

// Sets default values for this component's properties
UCamStreamer::UCamStreamer()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCamStreamer::BeginPlay()
{
    Super::BeginPlay();
    CaptureComponent = GetOwner()->FindComponentByClass<USceneCaptureComponent2D>();
    CameraRenderTarget = CaptureComponent->TextureTarget;
    CameraRenderTarget->bGPUSharedFlag = true;
    CameraImageWidth = CameraRenderTarget->SizeX;
    CameraImageHeight = CameraRenderTarget->SizeY;
    CameraImageSize = CameraImageWidth * CameraImageHeight;
    CameraPixelData.AddUninitialized(CameraImageSize);

    UdpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &ServerAddress.sin_addr);
}

bool UCamStreamer::GetCameraPixelData()
{
    FTextureRenderTargetResource* RenderTargetResource = CameraRenderTarget->GameThread_GetRenderTargetResource();
    return RenderTargetResource->ReadPixels(CameraPixelData);
}

void UCamStreamer::SendUDPData()
{
    TArray<uint8> ImageData;
    ImageData.Reserve(CameraImageSize * 3);

    for (const FColor& Pixel : CameraPixelData)
    {
        ImageData.Add(Pixel.R);
        ImageData.Add(Pixel.G);
        ImageData.Add(Pixel.B);
    }
	sendto(UdpSocket, ImageData.GetData(), ImageData.Num(), 0, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
}

// Called every frame
void UCamStreamer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetCameraPixelData())
    {
		SendUDPData();
    }
}