// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Item\CharacterFragment_CropHarvest.h"
#include "NativeGameplayTags.h"
#include "Actors/InteractableActors/Tiles/Farm_TileActor.h"

void UCharacterFragment_CropHarvest::OnInteractSuccess(AActor* DetectedActor)
{
    if (!DetectedActor)
    {
        return;
    }

    // 타일 액터 일시 타일안에 있는 씨앗 갖고와서 처리 만약 필요시 인터페이스 분리예정
    if (AFarm_TileActor* TileActor = Cast<AFarm_TileActor>(DetectedActor))
    {
        PlayMontage();
        TileActor->GetSeed()->SetLifeSpan(MontageLength);
    }

}

