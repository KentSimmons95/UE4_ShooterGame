// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups.h"
#include "Components/SphereComponent.h"
#include "Gun.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
}
// Sets default values
APickups::APickups()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupAmount = 30;
	TouchSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TouchSphereComponent"));
	TouchSphere->SetSphereRadius(45.f, false);

	TouchSphere->OnComponentBeginOverlap.AddDynamic(this, &APickups::OnPickup);
	RootComponent = TouchSphere;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);
}

// Called every frame
void APickups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickups::OnPickup(class UPrimitiveComponent *HitComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	AGun* Gun = Cast<AGun>(this);
	if (ShooterCharacter)
	{
		if(ShooterCharacter == nullptr) return;
		ShooterCharacter->SetAmmo(PickupAmount);

		this->Destroy();
	}
}
