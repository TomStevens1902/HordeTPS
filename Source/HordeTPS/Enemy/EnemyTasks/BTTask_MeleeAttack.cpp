// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

#include "AIController.h"
#include "HordeTPS/Enemy/CombatInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if(OutOfRange)
	{
		// Finish the task.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// We are in range so get the AI's controller and the enemy itself.
	auto const* Controller = OwnerComp.GetAIOwner();
	auto const EnemyCharacter = Cast<AEnemyBase>(Controller->GetPawn());

	// If the enemy supports the ICombatInterface, cast and call the Execute_MeleeAttack function.
	if(auto const* ICombat = Cast<ICombatInterface>(EnemyCharacter))
	{
		// Check to see if the montage has finished so that we don't try and play it again.
		if(MontageHasFinished(EnemyCharacter))
		{
			ICombat->Execute_MeleeAttack(EnemyCharacter);
		}
	}

	// Finish the task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_MeleeAttack::MontageHasFinished(const AEnemyBase* const Enemy)
{
	return Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->GetMontage());
}
