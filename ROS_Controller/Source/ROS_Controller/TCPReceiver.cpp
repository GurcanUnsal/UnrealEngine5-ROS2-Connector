// Fill out your copyright notice in the Description page of Project Settings.
#include "TCPReceiver.h"
#include "Sockets.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

// Sets default values for this component's properties
UTCPReceiver::UTCPReceiver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ServerSocket = -1;
    bIsServerRunning = false;
}


// Called when the game starts
void UTCPReceiver::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetOwner()->FindComponentByClass<UCarController>();

    if (!Controller)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CarController component not found!"));
        return;
    }

	StartTCPReceiver();
}

void UTCPReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    bIsServerRunning = false;
    if (ServerSocket != -1) {
        shutdown(ServerSocket, SHUT_RDWR);
        close(ServerSocket);
        ServerSocket = -1;
    }
}

void UTCPReceiver::StartTCPReceiver()
{
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket == -1) {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to create socket"));
        return;
    }

    sockaddr_in ServerAddress;
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerAddress.sin_port = htons(5000);

    if (bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1) {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bind failed"));
        close(ServerSocket);
        return;
    }

    if (listen(ServerSocket, 1) == -1) {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Listen failed"));
        close(ServerSocket);
        return;
    }

    bIsServerRunning = true;
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Server is listening on port 5000"));

    std::thread(&UTCPReceiver::ListenForConnections, this).detach();
}

void UTCPReceiver::ListenForConnections()
{
    sockaddr_in ClientAddress;
    socklen_t ClientAddressLen = sizeof(ClientAddress);

    while (bIsServerRunning && ServerSocket != -1) {
        int ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressLen);
        if (ClientSocket == -1) {
            if (bIsServerRunning) {
                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to accept connection"));
            }
            continue;
        }
        
        std::thread(&UTCPReceiver::HandleClientConnection, this, ClientSocket).detach();
    }
}

void UTCPReceiver::HandleClientConnection(int ClientSocket)
{
    struct {
        FLinear linear;
        FAngular angular;
    } velocityData;

    while (bIsServerRunning) {
        ssize_t BytesReceived = recv(ClientSocket, &velocityData, sizeof(velocityData), 0);
        if (BytesReceived <= 0) {
            break;
        }

        if (GEngine) {
            if (velocityData.linear.x > 1 || velocityData.linear.x < -1 || velocityData.linear.z > 1 || velocityData.linear.z < -1) {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Received Linear Velocity: x = %.2f, y = %.2f, z = %.2f"), velocityData.linear.x, velocityData.linear.y, velocityData.angular.z));
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Received Angular Velocity: x = %.2f, y = %.2f, z = %.2f"), velocityData.angular.x, velocityData.angular.y, velocityData.linear.z));
			
                Controller->SetLinearVelocity(velocityData.linear.x);
			    Controller->SetAngularVelocity(velocityData.linear.z);
            }
            
            
		}
    }

    close(ClientSocket);
}


