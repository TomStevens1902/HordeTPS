// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"



UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find a Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Gets the AI controller and its pawn
	if(auto* const EnemyController = Cast<AEnemyControllerBase>(OwnerComp.GetAIOwner()))
	{
		if (auto* const EnemyPawn = EnemyController->GetPawn())
		{
			// Obtain the enemy pawn's position to use as origin.
			auto const Origin = EnemyPawn->GetActorLocation();
			// Get the navigation system and generate a random location.
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Location;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
				}

				// Finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
