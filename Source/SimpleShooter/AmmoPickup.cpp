// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"
#include "Components/SphereComponent.h"
#include "Gun.h"
#include "Pickups.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

void AAmmoPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AAmmoPickup::OnPickup(class UPrimitiveComponent *HitComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
	
}
