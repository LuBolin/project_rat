
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	CameraBoom->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; // so we can lerp the rotation
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FIA_MousePan.AddDynamic(this, &APlayerCharacter::PanCamera);
	FIA_PlaneMove.AddDynamic(this, &APlayerCharacter::MoveInDirection);
	FIA_Jump.AddDynamic(this, &APlayerCharacter::JumpWrapper);

	Controller->SetControlRotation(FRotator(-45.f, 0.f, 0.f));
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
	if (!Controller)
		return;

	FRotator CurrentRotation = Controller->GetControlRotation();
	float NewPitch = CurrentRotation.Pitch + Input.Y;
	float NewYaw   = CurrentRotation.Yaw + Input.X;
	NewPitch = FMath::Clamp(NewPitch, -80.f, 10.f); // -80 for top down, 10 for slight bottom up

	FRotator NewControlRot = FRotator(NewPitch, NewYaw, 0.f);
	Controller->SetControlRotation(NewControlRot);
}

void APlayerCharacter::MoveInDirection(FVector2f Direction)
{
	if (!Controller || Direction.IsNearlyZero())
		return;

	FVector ForwardDir = FollowCamera->GetForwardVector();
	FVector RightDir   = FollowCamera->GetRightVector();
	ForwardDir.Z = 0.f; ForwardDir.Normalize();
	RightDir.Z = 0.f; RightDir.Normalize();

	const FVector MoveDirection = ForwardDir * Direction.Y + RightDir * Direction.X;

	AddMovementInput(MoveDirection.GetSafeNormal());

	if (!MoveDirection.IsNearlyZero())
	{
		FRotator TargetRotation = MoveDirection.Rotation();
		TargetRotation.Pitch = 0.f;
		TargetRotation.Roll = 0.f;

		SetActorRotation(FMath::RInterpTo(
			GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.f));
	}
}

void APlayerCharacter::JumpWrapper(bool bPressed)
{
	// us bPressed later for varied jump height
	// the whole cutting gravity until release thing
	Super::Jump();
}