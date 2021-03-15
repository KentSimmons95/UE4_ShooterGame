// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"


void AKillEmAllGameMode::BeginPlay() 
{
    Super::BeginPlay();
    GetNumberOfControllerPawns();
}
void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController != nullptr)
    {
        EndGame(false);
        GetNumberOfControllerPawns();
    }
   for(AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
   {
       if(!Controller->IsDead())
       {
           return;
       }
   }
   EndGame(true);
}

//Get number of enemies in level to keep track of
int32 AKillEmAllGameMode::GetNumberOfControllerPawns() 
{
    int32 Counter = 0;
    for(AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
   {
       ++Counter;
       UE_LOG(LogTemp, Warning, TEXT("%i"), Counter);
   }
   return Counter;
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
