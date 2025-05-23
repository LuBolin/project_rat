#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.generated.h"

// input events for states to 'subscribe' to in C++
// basically broadcast the IA delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIA_MousePan, FVector2f, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIA_PlaneMove, FVector2f, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIA_Jump, bool, bPressed);


UCLASS()
class PROJECT_RAT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region BlueprintHierarchy
protected:
	// From ACharacter:
	// TObjectPtr<UCapsuleComponent> CapsuleComponent
	// TObjectPtr<USkeletalMeshComponent> Mesh
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

#pragma endregion
	
	
#pragma region Lifecycle
public:
	APlayerCharacter();
	
	static APlayerCharacter* Instance;

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

#pragma endregion


#pragma region Input
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Input")
	FIA_MousePan FIA_MousePan;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Input")
	FIA_PlaneMove FIA_PlaneMove;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Input")
	FIA_Jump FIA_Jump;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion


	
#pragma region Camera
protected:
	UFUNCTION()
	void PanCamera(FVector2f Input);

	UPROPERTY()
	float MinPitch = -90.0f; // above horizontal
	
	UPROPERTY()
	float MaxPitch = 0.0f; // below horizontal
	
#pragma endregion


	

#pragma region Movement
protected:
	UFUNCTION()
	void MoveInDirection(FVector2f Direction);

	// Jump is declared in ACharacter, and overriding forces us
	// to use the ACharacter's UFUNCTION descriptor, which is not ideal
	UFUNCTION()
	void JumpWrapper(bool bPressed);
	
#pragma endregion
};