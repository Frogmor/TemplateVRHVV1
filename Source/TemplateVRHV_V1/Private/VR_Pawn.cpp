// Fill out your copyright notice in the Description page of Project Settings.

#include "TemplateVRHV_V1.h"
#include "VR_Pawn.h"
#include "IHeadMountedDisplay.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"


// Sets default values
AVR_Pawn::AVR_Pawn(const class FObjectInitializer &PCIP):Super(PCIP)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnableGravity = false;
	oculusLocationOffset = FVector(0.0f,0.0f,150.0f);

	this->BaseEyeHeight = 0.f;

	RootComponent = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));

	scene = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("VRBaseScene"));
	scene->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	scene->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);

	camera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	camera->AttachToComponent(scene, FAttachmentTransformRules::KeepRelativeTransform);
	camera->SetFieldOfView(110.f);
	camera->AttachToComponent(scene, FAttachmentTransformRules::KeepRelativeTransform);


	capsuleCollision= PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("capsule colision"));
	capsuleCollision->SetCapsuleHalfHeight(96.f);
	capsuleCollision->SetCapsuleRadius(96.f);
	capsuleCollision->SetRelativeLocation(FVector(0.f,0.f,-110.f));
	capsuleCollision->AttachToComponent(camera, FAttachmentTransformRules::KeepRelativeTransform);

	motionLeftController = PCIP.CreateDefaultSubobject<UMotionControllerComponent>(this, TEXT("Motion controler left"));
	motionLeftController->Hand = EControllerHand::Left;
	motionLeftController->AttachToComponent(scene, FAttachmentTransformRules::KeepRelativeTransform);
	motionLeftController->SetRelativeLocation(FVector(0.f, 0.f, 110.f));

	motionRightController = PCIP.CreateDefaultSubobject<UMotionControllerComponent>(this, TEXT("Motion controler Right"));
motionRightController->Hand = EControllerHand::Right;
motionRightController->AttachToComponent(scene, FAttachmentTransformRules::KeepRelativeTransform);
motionRightController->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
}

void AVR_Pawn::overridePawnValues(float PawnBaseEyeHeight, float FOV, float CapsuleHalfHeight, float CapsuleRadius, FVector CapsuleRelativeLocation, FVector SceneLocation, FVector LeftControllerLocation, FVector RightControllerLocation)
{
	//set pawn base eye height
	this->BaseEyeHeight = PawnBaseEyeHeight;

	//set camera FOV
	camera->SetFieldOfView(FOV);

	//set capsulte collision setings
	capsuleCollision->SetCapsuleHalfHeight(CapsuleHalfHeight);
	capsuleCollision->SetCapsuleRadius(CapsuleRadius);
	capsuleCollision->SetRelativeLocation(CapsuleRelativeLocation);

	//Set scene location
	scene->SetRelativeLocation(SceneLocation);

	motionLeftController->SetRelativeLocation(LeftControllerLocation);
	motionLeftController->SetRelativeLocation(RightControllerLocation);
}

void AVR_Pawn::RotatePawn(float RotationRate, float XAxisInput, float YAxisInput)
{
	if (XAxisInput != 0.f)
		this->AddActorLocalRotation(FRotator(0.f, XAxisInput*RotationRate, 0.f));

	if (YAxisInput != 0.f)
		this->AddActorLocalRotation(FRotator(0.f, YAxisInput*RotationRate, 0.f));

}

bool AVR_Pawn::isHMDWorn()
{
	if (GEngine->HMDDevice.IsValid()) {
		if (GEngine->HMDDevice->GetHMDWornState() == EHMDWornState::Worn) {
			return true;
		}
	}
	return false;
}

void AVR_Pawn::printDebugMessage(FString Message, bool OverWriteExisting, float Duration, FColor Color)
{
	int32 key;
	if (OverWriteExisting)
		key = 0;
	else
		key = 1;

	GEngine->AddOnScreenDebugMessage(key, Duration, Color, Message);
}

// Called when the game starts or when spawned
void AVR_Pawn::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine->HMDDevice.IsValid()) {

		//Override height offset for oculus
		switch (GEngine->HMDDevice->GetHMDDeviceType()) {
		case EHMDDeviceType::DT_OculusRift:
			this->SetActorLocation(this->GetActorLocation() + oculusLocationOffset);
			GEngine->HMDDevice->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
			break;
		default:break;
		}

		//set tracking origin (oculus & vive)
		GEngine->HMDDevice->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

// Called every frame
void AVR_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get current position of the Camera
	FVector cameraPosition = camera->GetComponentTransform().GetLocation();

	//Apply gravity if enabled and camera is positionned at head of player
	if (EnableGravity && camera->IsValidLowLevel() && cameraPosition.Z > this->GetActorLocation().Z) {

		//set line trace for gravity variables
		FHitResult RayHit(EForceInit::ForceInit);
		FCollisionQueryParams RayTraceParams(FName(TEXT("GravityRayTrace")), true, this->GetOwner());

		//initialize Gravity Trace Hit Result var
		RayTraceParams.bTraceComplex = true;
		RayTraceParams.bTraceAsyncScene = true;
		RayTraceParams.bReturnPhysicalMaterial = false;

		HitResult = GetWorld()->LineTraceSingleByChannel(
			RayHit, 
			cameraPosition,
			cameraPosition + FVector(0.f, 0.f, FMath::Abs(gravityVariable.floorTraceRange)*-1.f), 
			ECollisionChannel::ECC_Visibility, RayTraceParams);

		//check if we need to float the pawn over enevent terrain
		if (gravityVariable.respondToUneventTerrain 
			&& HitResult 
			&& RayHit.GetComponent()->CanCharacterStepUpOn == ECanBeCharacterBase::ECB_Yes && (RayHit.Distance + gravityVariable.floorTraceTolerance) < gravityVariable.floorTraceRange){
		this->TeleportTo(this->GetActorLocation() + FVector(0.f, 0.f, gravityVariable.floorTraceRange - RayHit.Distance), this->GetActorRotation());
		}

		if (HitResult ||
			RayHit.GetComponent()->CanCharacterStepUpOn != ECanBeCharacterBase::ECB_Yes) {
			this->TeleportTo(this->GetActorLocation() +
				(gravityVariable.gravityDirection*gravityVariable.gravityStrength),
				this->GetActorRotation());
		}
	}

}

// Called to bind functionality to input
void AVR_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

