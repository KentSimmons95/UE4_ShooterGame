// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#define OUT
// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::PullTrigger()
{
	if (LoadedAmmo <= 0) return;
	--LoadedAmmo;
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor *HitActor = Hit.GetActor();

		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AGun::GetCurrentAmmo()
{
	return LoadedAmmo;
}

int32 AGun::GetAmmoPool() 
{
	return AmmoPool;
}

void AGun::SetAmmoPickup(int32 PickupAmount) 
{
	AmmoPool = AmmoPool + PickupAmount;
}


void AGun::Reload() 
{
	if(AmmoPool <= 0 || LoadedAmmo >= 30) return;

	if(AmmoPool < (30 - LoadedAmmo))
	{
		LoadedAmmo = LoadedAmmo + AmmoPool;
		AmmoPool = 0;
	}
	else
	{
		AmmoPool = AmmoPool - (30 - LoadedAmmo);
		LoadedAmmo = 30;
	}
}

bool AGun::GetCanReload() 
{
	if(AmmoPool == 0)
	{
		CanReload = false;
	}
	else if(AmmoPool > 0 && LoadedAmmo < 30)
	{
		CanReload = true;
	}
	else if(LoadedAmmo >= 30 && AmmoPool > 0)
	{
		CanReload = false;
	}

	return CanReload;
}

float AGun::GetReloadTime() 
{
	return ReloadTime;
}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;
	FVector Location;
	FRotator Rotation;
	OUT OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
	//TODO LineTrace
	//Ignore Gun + Owner for line trace collision
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController *AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}
