// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInMeleeRange.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller and enemy character.
	auto const* Controller = Cast<AEnemyControllerBase>(OwnerComp.GetAIOwner());
	auto const* EnemyCharacter = Cast<AEnemyBase>(Controller->GetPawn());

	// Get player character.
	auto const* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Write true or false to the blackboard key depending on whether or not the player is in range.
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(),
		EnemyCharacter->GetDistanceTo(Player) <= MeleeRange);
	
}


