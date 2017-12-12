// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSHUD.h"
#include "Engine/Canvas.h"




void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();

    if (CrossHairTexture)
    {
        // Find the center of our canvas.
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        // Offset by half of the texture's dimensions so that the center of texture aligns with center of the Canvas.
        FVector2D CrossHairDrawPosition(
            Center.X - (CrossHairTexture->GetSurfaceWidth() * 0.5f),
            Center.Y - (CrossHairTexture->GetSurfaceHeight() * 0.5f)
        );

        // Draw the crosshair at the centerpoint.
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrossHairTexture->Resource, FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}
