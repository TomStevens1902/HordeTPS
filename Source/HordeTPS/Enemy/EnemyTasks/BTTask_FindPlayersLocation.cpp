// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayersLocation.h"

UBTTask_FindPlayersLocation::UBTTask_FindPlayersLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find the nearest player's location";
}

EBTNodeResult::Type UBTTask_FindPlayersLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get player character
	if (auto* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		//Get player location to use as origin
		FVector const PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
		if (SearchRandom)
		{
			// Get the navigation system and generate a random location.
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation NavLocation;
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, NavLocation))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
					// Finish with success
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			// Finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
