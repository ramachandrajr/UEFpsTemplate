// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"


/* Forward declarations */
class USphereComponent;
class UProjectileMovementComponent;

    


UCLASS()
class FPSPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere collision component.
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComponent;

    // Projectile movement component.
    UPROPERTY(VisibleDefaultsOnly, Category = MOvement)
    UProjectileMovementComponent* ProjectileMovementComponent;

    // Function that initializes the projectile's velocity in the shoot direction.
    void FireInDirection(const FVector& ShootDirection);

    // New
    // Function that is called when the projectile hits something.
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
