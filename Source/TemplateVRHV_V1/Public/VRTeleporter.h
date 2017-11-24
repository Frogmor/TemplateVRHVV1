// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine.h"
#include "VRTeleporter.generated.h"


UENUM(BlueprintType)
enum class EMoveDirectionEnum :uint8
{
	MOVE_FORWARD UMETA(DisplayName = "TowardPlayer"),
	MOVE_BACKWARD UMETA(DisplayName = "Away from player"),
	MOVE_LEFT UMETA(DisplayName = "Left of the player"),
	MOVE_Right UMETA(DisplayName = "Right from the player"),
	MOVE_CUSTOM UMETA(DisplayName = "use a Custome Rotation for Direction")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEVRHV_V1_API UVRTeleporter : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVRTeleporter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Teleporte Beam Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		UStaticMesh *TeleportBeamMesh = nullptr;

	//Teleport beam Launch velocity Magnitude-Higher number increases range of teleportation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		float BeamMagnitude;

	//Origine of the arc
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		FVector BeamLocationOffset;

	//Ensure the length od the beam reaches target location, Uses RayScaleRate as base length unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		bool RayInstantScale;
		
	//How much the ray will scale up until if reaches target location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		float RayScaleRate;

	//The teleport NavMesh Beam Tolerance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		FVector BeamHitNavMeshTolerance;

	//Teleport beam's custom gravity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		float ArcOverrideGravity;

	//offset of pawn(internal offset steam :112, rift:250)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		FVector TeleportTargetPawnSpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		float FloorIsAtZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		UStaticMesh *TeleportTargetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		FVector TeleportTargetMeshScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Teleport Beam Parameters")
		FVector TeleportTargetMeshSpawnOffset;
};
