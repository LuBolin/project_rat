
#include "PlayerCharacter.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FIA_MousePan.AddDynamic(this, &APlayerCharacter::PanCamera);
	FIA_PlaneMove.AddDynamic(this, &APlayerCharacter::MoveInDirection);
	FIA_Jump.AddDynamic(this, &APlayerCharacter::JumpWrapper);
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::PanCamera(FVector2f Input)
{
	const FRotator CurrentRotation = Controller->GetControlRotation();
	float NewPitch = CurrentRotation.Pitch + Input.Y;
	// Normalize and clamp the pitch to avoid gimbal flips
	NewPitch = FMath::ClampAngle(NewPitch, -90.f, 0.f);
	float NewYaw = CurrentRotation.Yaw + Input.X;
	// Apply new rotation: keep yaw and roll
	FRotator NewControlRot = FRotator(NewPitch, NewYaw, CurrentRotation.Roll);
	Controller->SetControlRotation(NewControlRot);
}


void APlayerCharacter::MoveInDirection(FVector2f Direction)
{
	if (!Controller || Direction.IsNearlyZero())
		return;

	// Get yaw-only rotation (ignore pitch/roll)
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

	// Forward and Right vectors from yaw
	// UE world forward is X, right is Y
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Move in direction relative to camera
	AddMovementInput(ForwardVector, Direction.Y);
	AddMovementInput(RightVector, Direction.X); 
}

void APlayerCharacter::JumpWrapper(bool bPressed)
{
	// us bPressed later for varied jump height
	// the whole cutting gravity until release thing
	Super::Jump();
}