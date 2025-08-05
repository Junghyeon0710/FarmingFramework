// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Interaction/Farm_HighlightableStaticMesh.h"

void UFarm_HighlightableStaticMesh::Highlight_Implementation()
{
    check(HighlightMaterial);
    SetOverlayMaterial(HighlightMaterial);
}

void UFarm_HighlightableStaticMesh::Unhighlight_Implementation()
{
    SetOverlayMaterial(nullptr);
}
