// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int32 GetCurrentAmmo();
	int32 GetAmmoPool();
	bool CanReload;

	UFUNCTION()
	void Reload();

	//Getters
	bool GetCanReload();
	float GetReloadTime();
	//Setters

	void SetAmmoPickup(int32 PickupAmount);

private:
	UPROPERTY(VisibleAnywhere) //Pointer should be visible anywhere but not editable
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase *MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem *ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase *ImpactSound;

	//Gun Properties
	UPROPERTY(EditAnywhere, Category = GunProperties)
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = GunProperties)
	float Damage = 10.f;

	UPROPERTY(VisibleAnywhere, Category = GunProperties)
	float ReloadTime = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 AmmoPool = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 LoadedAmmo = 30.f;
	
	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);

	AController *GetOwnerController() const;
};
