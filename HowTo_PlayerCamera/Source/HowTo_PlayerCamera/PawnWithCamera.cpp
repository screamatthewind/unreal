// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnWithCamera.h"
#include "HowTo_PlayerCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include <Runtime/Engine/Classes/Engine/SceneCapture2D.h>

// Sets default values
APawnWithCamera::APawnWithCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//Attach our components
	StaticMeshComp->SetupAttachment(RootComponent);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = false;
	SpringArmComp->CameraLagSpeed = 0.0f;

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();

//	SetupColorCaptureComponent(ColorCaptureComponents);
}

// Called every frame
void APawnWithCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);

		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		CameraComp->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		SpringArmComp->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}

	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	//Rotate our camera's pitch, but limit it so we're always looking downward
	{
		FRotator NewRotation = SpringArmComp->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 0.0f);
		// NewRotation.Pitch = NewRotation.Pitch + CameraInput.Y;
		SpringArmComp->SetWorldRotation(NewRotation);
	}

	//Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 100 units per second
			MovementInput = MovementInput.GetSafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}

	FBoxSphereBounds bounds;

	AActor* AMultiGpGate_Mesh = GetActorByName("multiGpGate_Mesh");

	FBox bb = AMultiGpGate_Mesh->GetComponentsBoundingBox();
	FBox bb2 = AMultiGpGate_Mesh->CalculateComponentsBoundingBoxInLocalSpace();
	FVector bbSize = bb.GetSize();

	DrawDebugBox(GetWorld(), AMultiGpGate_Mesh->GetActorLocation(), AMultiGpGate_Mesh->GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);

	// GrabScreenshot();
}

void APawnWithCamera::GrabScreenshot() {
	// Generate a filename based on the current date
	const FDateTime Now = FDateTime::Now();

	// store screenshot in Project directory next to main UProject/EXE based on the build type
#if WITH_EDITOR
	const FString ImageDirectory = FString::Printf(TEXT("%s/%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
#else
	const FString ImageDirectory = FString::Printf(TEXT("%s/../%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
#endif

	const FString ImageFilename = FString::Printf(TEXT("%s/Screenshot_%d%02d%02d_%02d%02d%02d_%03d.png"), *ImageDirectory, Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());

	FScreenshotRequest::RequestScreenshot(ImageFilename, false, false);
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Hook up events for "ZoomIn"
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &APawnWithCamera::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APawnWithCamera::YawCamera);
}

//Input functions
void APawnWithCamera::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APawnWithCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APawnWithCamera::ZoomIn()
{
	bZoomingIn = true;
}

void APawnWithCamera::ZoomOut()
{
	bZoomingIn = false;
}

TArray<FString> APawnWithCamera::GetActors()
{
	TArray<FString> actors;

	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(AActor::StaticClass()))
		{
			actors.Add(Itr->GetActorLabel());
		}
	}
	return actors;
}

AActor *APawnWithCamera::GetActorByName(FString FindActorName)
{
	AActor *result = nullptr;

	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(AActor::StaticClass()))
		{
			if (Itr->GetActorLabel().Equals(FindActorName))
			{
				result = *Itr;
				break;
			}
		}
	}
	return result;
}

void APawnWithCamera::SetupColorCaptureComponent(ASceneCapture2D* captureComponent) {

	// Create RenderTargets
	UTextureRenderTarget2D* renderTarget2D = NewObject<UTextureRenderTarget2D>();

	// Set FrameWidth and FrameHeight
	renderTarget2D->TargetGamma = 1.2f;// for Vulkan //GEngine->GetDisplayGamma(); // for DX11/12

	// Setup the RenderTarget capture format
	renderTarget2D->InitAutoFormat(256, 256); // some random format, got crashing otherwise
	int32 frameWidth = 640;
	int32 frameHeight = 480;
	renderTarget2D->InitCustomFormat(frameWidth, frameHeight, PF_B8G8R8A8, true); // PF_B8G8R8A8 disables HDR which will boost storing to disk due to less image information
	renderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	renderTarget2D->bGPUSharedFlag = true; // demand buffer on GPU

	// Assign RenderTarget
	captureComponent->GetCaptureComponent2D()->TextureTarget = renderTarget2D;

	// Set Camera Properties
	captureComponent->GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	captureComponent->GetCaptureComponent2D()->ShowFlags.SetTemporalAA(true);
	// lookup more showflags in the UE4 documentation.. 
}


