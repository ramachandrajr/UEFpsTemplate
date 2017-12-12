// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "FPSProjectile.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create a first person camera component.  
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    // Attach the camera component to our capsule component.
    FPSCameraComponent->SetupAttachment( (USceneComponent *) GetCapsuleComponent() );

    // Position the camera slightly above the eyes.
    FPSCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f + BaseEyeHeight));
    // Allow the pawn to control camera rotation.
    FPSCameraComponent->bUsePawnControlRotation = true;

 
    // Create a first person mesh component for the owning player.
    FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    // only the owning player sees this mesh.
    FPSMesh->SetOnlyOwnerSee(true);
    // Attach the FPS mesh to the FPS camera.
    FPSMesh->SetupAttachment(FPSCameraComponent);
    // Disable some environmental shadowning to preserve the illusion of having a single mesh.
    FPSMesh->bCastDynamicShadow = false;
    FPSMesh->CastShadow = false; 

    // The owning player doesn't see the regular (third-person) body mesh.
    GetMesh()->SetOwnerNoSee(true);
}



// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (GEngine)
    {
        // Put up a debug message for five seconds. 
        // The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("We are using FPSCharacter."));
    }
}



// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Fire();
}



// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Set up "movement" bindings.
    /*  The first argument corresponds to new axis mapping we created in settings
        second to object and third method which should be called if axis is activated
        truth is axis will be polled always */
    PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

    // Set up "look" bindings.
    PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

    // Set up "action" bindings.
    // Bind action methods should have void return and no input arguments
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}



void AFPSCharacter::MoveForward(float Value)
{
    // Find out which way is "forward" and record that player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}



void AFPSCharacter::MoveRight(float Value)
{
    // Find out which way is "right" and record that player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}



void AFPSCharacter::StartJump()
{
    bPressedJump = true;
}



void AFPSCharacter::StopJump()
{
    bPressedJump = false;
}



void AFPSCharacter::Fire()
{
    // Attempt to fire a projectile.
    if (!ProjectileClass) { return; }

    // Get the camera transform.
    FVector CameraLocation;
    FRotator CameraRotation;
    GetActorEyesViewPoint(CameraLocation, CameraRotation);
       

    // Transform MuzzleOffset from camera space to world space.
    FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);    
    FRotator MuzzleRotation = CameraRotation;
    // Skew the aim to be slightly upwards.
    MuzzleRotation.Pitch += 10.f;
    UWorld* World = GetWorld();

    if (!World) { return; }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = Instigator;
    // Spawn the projectile at the muzzle.
    AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

    if (!Projectile) { return; }

    // Set the Projectile's initial trajectory.
    FVector LaunchDirection = MuzzleRotation.Vector();
    Projectile->FireInDirection(LaunchDirection);
}

