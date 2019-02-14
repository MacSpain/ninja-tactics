
internal void
InitializeEnemy(game_mode_level *LevelMode, u32 EntityIndex,
                memory_arena *Arena,
                entity_action *Actions, u32 ActionCount)
{
    entity *Entity = GetEntity(LevelMode, EntityIndex);

    Entity->Type = EntityType_Enemy;
    Entity->Exists = true;
    Entity->ActionType = Action_Stay;

    for(u32 ActionIndex = 0;
        ActionIndex < ActionCount;
        ++ActionIndex)
    {
        Entity->Actions[ActionIndex] = Actions[ActionIndex];
    }
    Entity->ActionCount = ActionCount;
    Entity->ActionBias = 0;

    Entity->FacingDirection = Actions[0].Direction;
    Entity->P.AbsTileX = Actions[0].Position.AbsTileX;
    Entity->P.AbsTileY = Actions[0].Position.AbsTileY;
    Entity->P.AbsTileZ = Actions[0].Position.AbsTileZ;

    Entity->Alerted = false;
    Entity->CurrentDerail = ArrayCount(Entity->Actions);
    Entity->EndDerail = 0;
    Entity->DerailBias = 0;
    Entity->ActionBias = 0;
    Entity->CurrentBitmapID = Asset_Enemy_Left_Small;

    Entity->P.Offset.x = 0.0f;
    Entity->P.Offset.y = 0.0f;

    Entity->Height = 1.0f;
    Entity->Width = 0.75f*Entity->Height;

    Entity->tMoveMax = 0.25f;

    Entity->VisionFields.VisionField.SightTileCountX = 7;
    Entity->VisionFields.VisionField.SightTileCountY = 7;
    Entity->VisionFields.VisionField.SightTiles = PushArray(Arena,
                                                            Entity->VisionFields.VisionField.SightTileCountX*Entity->VisionFields.VisionField.SightTileCountY,
                                                            b32);

    Entity->VisionFields.NewVisionField.SightTileCountX = 7;
    Entity->VisionFields.NewVisionField.SightTileCountY = 7;
    Entity->VisionFields.NewVisionField.SightTiles = PushArray(Arena,
                                                               Entity->VisionFields.NewVisionField.SightTileCountX*Entity->VisionFields.NewVisionField.SightTileCountY,
                                                               b32);
}

internal b32
CheckEnemySightUp(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                 b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        if(!(CurrentValue == TileType_DownWall ||
             CurrentValue == TileType_DownDoor ||
             CurrentValue == TileType_LWallDWall ||
             CurrentValue == TileType_RWallDWall ||
             CurrentValue == TileType_LDoorDWall ||
             CurrentValue == TileType_RDoorDWall ||
             CurrentValue == TileType_LWallDDoor ||
             CurrentValue == TileType_RWallDDoor) &&
           !(PreviousValue == TileType_UpWall ||
             PreviousValue == TileType_UpDoor ||
             PreviousValue == TileType_LWallUWall ||
             PreviousValue == TileType_RWallUWall ||
             PreviousValue == TileType_LDoorUWall ||
             PreviousValue == TileType_RDoorUWall ||
             PreviousValue == TileType_LWallUDoor ||
             PreviousValue == TileType_RWallUDoor))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;
                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileY += 1;

                CheckEnemySightUp(TileMap, VisionField, EnemyP, CurrentTile, NextTile, Continue);
            }
        }
    }

    return Result;
}

internal b32
CheckEnemySightDown(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP,
                   tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        if(!(CurrentValue == TileType_UpWall ||
             CurrentValue == TileType_UpDoor ||
             CurrentValue == TileType_LWallUWall ||
             CurrentValue == TileType_RWallUWall ||
             CurrentValue == TileType_LDoorUWall ||
             CurrentValue == TileType_RDoorUWall ||
             CurrentValue == TileType_LWallUDoor ||
             CurrentValue == TileType_RWallUDoor) &&
           !(PreviousValue == TileType_DownWall ||
             PreviousValue == TileType_DownDoor ||
             PreviousValue == TileType_LWallDWall ||
             PreviousValue == TileType_RWallDWall ||
             PreviousValue == TileType_LDoorDWall ||
             PreviousValue == TileType_RDoorDWall ||
             PreviousValue == TileType_LWallDDoor ||
             PreviousValue == TileType_RWallDDoor))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;

                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileY -= 1;

                CheckEnemySightDown(TileMap, VisionField, EnemyP, CurrentTile, NextTile, Continue);

            }
        }
    }

    return Result;
}

internal b32
CheckEnemySightLeft(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP,
                   tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        if(!(CurrentValue == TileType_RightWall ||
             CurrentValue == TileType_RightDoor ||
             CurrentValue == TileType_RWallDWall ||
             CurrentValue == TileType_RWallUWall ||
             CurrentValue == TileType_RDoorDWall ||
             CurrentValue == TileType_RDoorUWall ||
             CurrentValue == TileType_RWallDDoor ||
             CurrentValue == TileType_RWallUDoor) &&
           !(PreviousValue == TileType_LeftWall ||
             PreviousValue == TileType_LeftDoor ||
             PreviousValue == TileType_LWallDWall ||
             PreviousValue == TileType_LWallUWall ||
             PreviousValue == TileType_LDoorDWall ||
             PreviousValue == TileType_LDoorUWall ||
             PreviousValue == TileType_LWallDDoor ||
             PreviousValue == TileType_LWallUDoor))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;

                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileX -= 1;

                CheckEnemySightLeft(TileMap, VisionField, EnemyP, CurrentTile, NextTile, Continue);

            }
        }
    }

    return Result;
}

internal b32
CheckEnemySightRight(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP,
                   tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        if(!(CurrentValue == TileType_LeftWall ||
             CurrentValue == TileType_LeftDoor ||
             CurrentValue == TileType_LWallDWall ||
             CurrentValue == TileType_LWallUWall ||
             CurrentValue == TileType_LDoorDWall ||
             CurrentValue == TileType_LDoorUWall ||
             CurrentValue == TileType_LWallDDoor ||
             CurrentValue == TileType_LWallUDoor) &&
           !(PreviousValue == TileType_RightWall ||
             PreviousValue == TileType_RightDoor ||
             PreviousValue == TileType_RWallDWall ||
             PreviousValue == TileType_RWallUWall ||
             PreviousValue == TileType_RDoorDWall ||
             PreviousValue == TileType_RDoorUWall ||
             PreviousValue == TileType_RWallDDoor ||
             PreviousValue == TileType_RWallUDoor))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;
                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileX += 1;

                CheckEnemySightRight(TileMap, VisionField, EnemyP, CurrentTile, NextTile, Continue);

            }
        }
    }

    return Result;
}

internal b32
CheckEnemySightUpLeft(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP, tile_direction TileDirection,
                   tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        tile_map_position RightSideTile = CurrentTile;
        RightSideTile.AbsTileX += 1;

        tile_map_position DownSideTile = CurrentTile;
        DownSideTile.AbsTileY -= 1;

        if(!(CurrentValue == TileType_RWallDWall ||
             CurrentValue == TileType_RDoorDWall ||
             CurrentValue == TileType_RWallDDoor) &&
           !(PreviousValue == TileType_LWallUWall ||
             PreviousValue == TileType_LDoorUWall ||
             PreviousValue == TileType_LWallUDoor) &&
           !(!CheckEnemySightUp(TileMap, VisionField, EnemyP, PreviousTile, RightSideTile, false) &&
             !CheckEnemySightLeft(TileMap, VisionField, EnemyP, PreviousTile, DownSideTile, false)) &&
           !(!CheckEnemySightUp(TileMap, VisionField, EnemyP, DownSideTile, CurrentTile, false) &&
             !CheckEnemySightLeft(TileMap, VisionField, EnemyP, RightSideTile, CurrentTile, false)) &&
           !(!CheckEnemySightLeft(TileMap, VisionField, EnemyP, RightSideTile, CurrentTile, false) &&
             !CheckEnemySightLeft(TileMap, VisionField, EnemyP, PreviousTile, DownSideTile, false)) &&
           !(!CheckEnemySightUp(TileMap, VisionField, EnemyP, PreviousTile, RightSideTile, false) &&
             !CheckEnemySightUp(TileMap, VisionField, EnemyP, DownSideTile, CurrentTile, false)))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;

                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileY += 1;
                NextTile.AbsTileX -= 1;

                tile_map_position NextTileUp = CurrentTile;
                NextTileUp.AbsTileY += 1;

                tile_map_position NextTileLeft = CurrentTile;
                NextTileLeft.AbsTileX -= 1;

                CheckEnemySightUpLeft(TileMap, VisionField, EnemyP, TileDirection, CurrentTile, NextTile, Continue);

                if(CheckEnemySightLeft(TileMap, VisionField, EnemyP, RightSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Up)
                {
                    CheckEnemySightUp(TileMap, VisionField, EnemyP, CurrentTile, NextTileUp, Continue);
                }

                if(CheckEnemySightUp(TileMap, VisionField, EnemyP, DownSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Left)
                {
                    CheckEnemySightLeft(TileMap, VisionField, EnemyP, CurrentTile, NextTileLeft, Continue);
                }

            }
        }
    }
    return Result;
}

internal b32
CheckEnemySightUpRight(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP, tile_direction TileDirection,
                   tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        tile_map_position LeftSideTile = CurrentTile;
        LeftSideTile.AbsTileX -= 1;

        tile_map_position DownSideTile = CurrentTile;
        DownSideTile.AbsTileY -= 1;

        if(!(CurrentValue == TileType_LWallDWall ||
             CurrentValue == TileType_LDoorDWall ||
             CurrentValue == TileType_LWallDDoor) &&
           !(PreviousValue == TileType_RWallUWall ||
             PreviousValue == TileType_RDoorUWall ||
             PreviousValue == TileType_RWallUDoor) &&
           !(!CheckEnemySightUp(TileMap, VisionField, EnemyP, PreviousTile, LeftSideTile, false) &&
             !CheckEnemySightRight(TileMap, VisionField, EnemyP, PreviousTile, DownSideTile, false)) &&
           !(!CheckEnemySightUp(TileMap, VisionField, EnemyP, DownSideTile, CurrentTile, false) &&
             !CheckEnemySightRight(TileMap, VisionField, EnemyP, LeftSideTile, CurrentTile, false)) &&
           !(!CheckEnemySightRight(TileMap, VisionField, EnemyP, LeftSideTile, CurrentTile, false) &&
             !CheckEnemySightRight(TileMap, VisionField, EnemyP, PreviousTile, DownSideTile, false)) &&
           !(!CheckEnemySightUp(TileMap, VisionField, EnemyP, PreviousTile, LeftSideTile, false) &&
             !CheckEnemySightUp(TileMap, VisionField, EnemyP, DownSideTile, CurrentTile, false)))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;
                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileY += 1;
                NextTile.AbsTileX += 1;

                tile_map_position NextTileUp = CurrentTile;
                NextTileUp.AbsTileY += 1;

                tile_map_position NextTileRight = CurrentTile;
                NextTileRight.AbsTileX += 1;

                CheckEnemySightUpRight(TileMap, VisionField, EnemyP, TileDirection, CurrentTile, NextTile, Continue);


                if(CheckEnemySightRight(TileMap, VisionField, EnemyP, LeftSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Up)
                {
                    CheckEnemySightUp(TileMap, VisionField, EnemyP, CurrentTile, NextTileUp, Continue);
                }

                if(CheckEnemySightUp(TileMap, VisionField, EnemyP, DownSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Right)
                {
                    CheckEnemySightRight(TileMap, VisionField, EnemyP, CurrentTile, NextTileRight, Continue);
                }
            }
        }
    }

    return Result;
}

internal b32
CheckEnemySightDownLeft(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP, tile_direction TileDirection,
                   tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        tile_map_position RightSideTile = CurrentTile;
        RightSideTile.AbsTileX += 1;

        tile_map_position UpSideTile = CurrentTile;
        UpSideTile.AbsTileY += 1;

        if(!(CurrentValue == TileType_RWallUWall ||
             CurrentValue == TileType_RDoorUWall ||
             CurrentValue == TileType_RWallUDoor) &&
           !(PreviousValue == TileType_LWallDWall ||
             PreviousValue == TileType_LDoorDWall ||
             PreviousValue == TileType_LWallDDoor) &&
           !(!CheckEnemySightDown(TileMap, VisionField, EnemyP, PreviousTile, RightSideTile, false) &&
             !CheckEnemySightLeft(TileMap, VisionField, EnemyP, PreviousTile, UpSideTile, false)) &&
           !(!CheckEnemySightDown(TileMap, VisionField, EnemyP, UpSideTile, CurrentTile, false) &&
             !CheckEnemySightLeft(TileMap, VisionField, EnemyP, RightSideTile, CurrentTile, false)) &&
           !(!CheckEnemySightLeft(TileMap, VisionField, EnemyP, RightSideTile, CurrentTile, false) &&
             !CheckEnemySightLeft(TileMap, VisionField, EnemyP, PreviousTile, UpSideTile, false)) &&
           !(!CheckEnemySightDown(TileMap, VisionField, EnemyP, PreviousTile, RightSideTile, false) &&
             !CheckEnemySightDown(TileMap, VisionField, EnemyP, UpSideTile, CurrentTile, false)))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;

                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileY -= 1;
                NextTile.AbsTileX -= 1;

                tile_map_position NextTileDown = CurrentTile;
                NextTileDown.AbsTileY -= 1;

                tile_map_position NextTileLeft = CurrentTile;
                NextTileLeft.AbsTileX -= 1;

                CheckEnemySightDownLeft(TileMap, VisionField, EnemyP, TileDirection, CurrentTile, NextTile, Continue);

                if(CheckEnemySightLeft(TileMap, VisionField, EnemyP, RightSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Down)
                {
                    CheckEnemySightDown(TileMap, VisionField, EnemyP, CurrentTile, NextTileDown, Continue);
                }

                if(CheckEnemySightDown(TileMap, VisionField, EnemyP, UpSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Left)
                {
                    CheckEnemySightLeft(TileMap, VisionField, EnemyP, CurrentTile, NextTileLeft, Continue);
                }

            }
        }
    }

    return Result;
}

internal b32
CheckEnemySightDownRight(tile_map *TileMap, vision_field *VisionField, tile_map_position EnemyP, tile_direction TileDirection,
                   tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true)
{
    b32 Result = false;

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 DiffTileX = CurrentTile.AbsTileX - EnemyP.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - EnemyP.AbsTileY;

    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        b32 *CurrTile = &VisionField->SightTiles[(TileHalfCountY + DiffTileY)*VisionField->SightTileCountX + (TileHalfCountX + DiffTileX)];

        u32 CurrentValue = GetTileValue(TileMap, CurrentTile.AbsTileX, CurrentTile.AbsTileY, CurrentTile.AbsTileZ);
        u32 PreviousValue = GetTileValue(TileMap, PreviousTile.AbsTileX, PreviousTile.AbsTileY, PreviousTile.AbsTileZ);

        tile_map_position LeftSideTile = CurrentTile;
        LeftSideTile.AbsTileX -= 1;

        tile_map_position UpSideTile = CurrentTile;
        UpSideTile.AbsTileY += 1;

        if(!(CurrentValue == TileType_LWallUWall ||
             CurrentValue == TileType_LDoorUWall ||
             CurrentValue == TileType_LWallUDoor) &&
           !(PreviousValue == TileType_RWallDWall ||
             PreviousValue == TileType_RDoorDWall ||
             PreviousValue == TileType_RWallDDoor) &&
           !(!CheckEnemySightDown(TileMap, VisionField, EnemyP, PreviousTile, UpSideTile, false) &&
             !CheckEnemySightRight(TileMap, VisionField, EnemyP, PreviousTile, LeftSideTile, false)) &&
           !(!CheckEnemySightDown(TileMap, VisionField, EnemyP, UpSideTile, CurrentTile, false) &&
             !CheckEnemySightRight(TileMap, VisionField, EnemyP, LeftSideTile, CurrentTile, false)) &&
           !(!CheckEnemySightRight(TileMap, VisionField, EnemyP, LeftSideTile, CurrentTile, false) &&
             !CheckEnemySightRight(TileMap, VisionField, EnemyP, PreviousTile, UpSideTile, false)) &&
           !(!CheckEnemySightDown(TileMap, VisionField, EnemyP, PreviousTile, LeftSideTile, false) &&
             !CheckEnemySightDown(TileMap, VisionField, EnemyP, UpSideTile, CurrentTile, false)))
        {
            Result = true;

            if(Continue)
            {
                *CurrTile = true;

                tile_map_position NextTile = CurrentTile;
                NextTile.AbsTileY -= 1;
                NextTile.AbsTileX += 1;

                tile_map_position NextTileDown = CurrentTile;
                NextTileDown.AbsTileY -= 1;

                tile_map_position NextTileRight = CurrentTile;
                NextTileRight.AbsTileX += 1;

                CheckEnemySightDownRight(TileMap, VisionField, EnemyP, TileDirection, CurrentTile, NextTile, Continue);

                if(CheckEnemySightRight(TileMap, VisionField, EnemyP, LeftSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Down)
                {
                    CheckEnemySightDown(TileMap, VisionField, EnemyP, CurrentTile, NextTileDown, Continue);
                }

                if(CheckEnemySightDown(TileMap, VisionField, EnemyP, UpSideTile, CurrentTile, false) &&
                   TileDirection == Direction_Right)
                {
                    CheckEnemySightRight(TileMap, VisionField, EnemyP, CurrentTile, NextTileRight, Continue);
                }
            }
        }
    }

    return Result;
}

internal void
CheckEnemyLineOfSight(tile_map *TileMap, entity *Enemy)
{

    vision_field *VisionField = &Enemy->VisionFields.VisionField;

    for(u32 SightTileIndex = 0;
        SightTileIndex < VisionField->SightTileCountX*VisionField->SightTileCountY;
        ++SightTileIndex)
    {
        VisionField->SightTiles[SightTileIndex] = false;
    }

    Assert((VisionField->SightTileCountX % 2) == 1);
    Assert((VisionField->SightTileCountY % 2) == 1);

    s32 TileCenterX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileCenterY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    VisionField->SightTiles[TileCenterY*VisionField->SightTileCountX + TileCenterX] = true;

    tile_map_position TilePos = Enemy->P;
    tile_map_position TilePosUp = Enemy->P;
    TilePosUp.AbsTileY += 1;
    tile_map_position TilePosUpLeft = Enemy->P;
    TilePosUpLeft.AbsTileY += 1;
    TilePosUpLeft.AbsTileX -= 1;
    tile_map_position TilePosUpRight = Enemy->P;
    TilePosUpRight.AbsTileY += 1;
    TilePosUpRight.AbsTileX += 1;

    tile_map_position TilePosDown = Enemy->P;
    TilePosDown.AbsTileY -= 1;
    tile_map_position TilePosDownLeft = Enemy->P;
    TilePosDownLeft.AbsTileY -= 1;
    TilePosDownLeft.AbsTileX -= 1;
    tile_map_position TilePosDownRight = Enemy->P;
    TilePosDownRight.AbsTileY -= 1;
    TilePosDownRight.AbsTileX += 1;

    tile_map_position TilePosLeft = Enemy->P;
    TilePosLeft.AbsTileX -= 1;

    tile_map_position TilePosRight = Enemy->P;
    TilePosRight.AbsTileX += 1;

    switch(Enemy->FacingDirection)
    {
        case Direction_Up:
        {
            CheckEnemySightUp(TileMap, VisionField, Enemy->P, TilePos, TilePosUp, true);
            CheckEnemySightUpLeft(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection, TilePos, TilePosUpLeft, true);
            CheckEnemySightUpRight(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosUpRight, true);
        } break;

        case Direction_Left:
        {
            CheckEnemySightLeft(TileMap, VisionField, Enemy->P,  TilePos, TilePosLeft, true);
            CheckEnemySightUpLeft(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosUpLeft, true);
            CheckEnemySightDownLeft(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownLeft, true);
        } break;

        case Direction_Down:
        {
            CheckEnemySightDown(TileMap, VisionField, Enemy->P,  TilePos, TilePosDown, true);
            CheckEnemySightDownRight(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownRight, true);
            CheckEnemySightDownLeft(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownLeft, true);
        } break;

        case Direction_Right:
        {
            CheckEnemySightRight(TileMap, VisionField, Enemy->P,  TilePos, TilePosRight, true);
            CheckEnemySightUpRight(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosUpRight, true);
            CheckEnemySightDownRight(TileMap, VisionField, Enemy->P, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownRight, true);
        } break;
    }

    VisionField = &Enemy->VisionFields.NewVisionField;

    for(u32 SightTileIndex = 0;
        SightTileIndex < VisionField->SightTileCountX*VisionField->SightTileCountY;
        ++SightTileIndex)
    {
        VisionField->SightTiles[SightTileIndex] = false;
    }

    if(Enemy->ActionType == Action_Move)
    {
        Assert((VisionField->SightTileCountX % 2) == 1);
        Assert((VisionField->SightTileCountY % 2) == 1);

        TileCenterX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
        TileCenterY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

        VisionField->SightTiles[TileCenterY*VisionField->SightTileCountX + TileCenterX] = true;

        tile_map_position NewEnemyP = Enemy->P;
        NewEnemyP.AbsTileX += (s32)Enemy->dP.x;
        NewEnemyP.AbsTileY += (s32)Enemy->dP.y;

        TilePos = NewEnemyP;
        TilePosUp = NewEnemyP;
        TilePosUp.AbsTileY += 1;
        TilePosUpLeft = NewEnemyP;
        TilePosUpLeft.AbsTileY += 1;
        TilePosUpLeft.AbsTileX -= 1;
        TilePosUpRight = NewEnemyP;
        TilePosUpRight.AbsTileY += 1;
        TilePosUpRight.AbsTileX += 1;

        TilePosDown = NewEnemyP;
        TilePosDown.AbsTileY -= 1;
        TilePosDownLeft = NewEnemyP;
        TilePosDownLeft.AbsTileY -= 1;
        TilePosDownLeft.AbsTileX -= 1;
        TilePosDownRight = NewEnemyP;
        TilePosDownRight.AbsTileY -= 1;
        TilePosDownRight.AbsTileX += 1;

        TilePosLeft = NewEnemyP;
        TilePosLeft.AbsTileX -= 1;

        TilePosRight = NewEnemyP;
        TilePosRight.AbsTileX += 1;

        switch((tile_direction)Enemy->FacingDirection)
        {
            case Direction_Up:
            {
                CheckEnemySightUp(TileMap, VisionField, NewEnemyP, TilePos, TilePosUp, true);
                CheckEnemySightUpLeft(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection, TilePos, TilePosUpLeft, true);
                CheckEnemySightUpRight(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosUpRight, true);
            } break;

            case Direction_Left:
            {
                CheckEnemySightLeft(TileMap, VisionField, NewEnemyP,  TilePos, TilePosLeft, true);
                CheckEnemySightUpLeft(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosUpLeft, true);
                CheckEnemySightDownLeft(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownLeft, true);
            } break;

            case Direction_Down:
            {
                CheckEnemySightDown(TileMap, VisionField, NewEnemyP,  TilePos, TilePosDown, true);
                CheckEnemySightDownRight(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownRight, true);
                CheckEnemySightDownLeft(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownLeft, true);
            } break;

            case Direction_Right:
            {
                CheckEnemySightRight(TileMap, VisionField, NewEnemyP,  TilePos, TilePosRight, true);
                CheckEnemySightUpRight(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosUpRight, true);
                CheckEnemySightDownRight(TileMap, VisionField, NewEnemyP, (tile_direction)Enemy->FacingDirection,  TilePos, TilePosDownRight, true);
            } break;
        }
    }
    else
    {

        VisionField = &Enemy->VisionFields.VisionField;

        for(u32 SightTileIndex = 0;
            SightTileIndex < VisionField->SightTileCountX*VisionField->SightTileCountY;
            ++SightTileIndex)
        {
            VisionField->SightTiles[SightTileIndex] = Enemy->VisionFields.VisionField.SightTiles[SightTileIndex];
        }

    }

}

internal void
ValidateEnemySightDownLeft(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightDownLeft(TileMap, VisionField, X - 1, Y - 1);
        }
        else
        {
            ValidateEnemySightDownLeft(TileMap, VisionField, X - 1, Y - 1, false);
        }
    }
}

internal void
ValidateEnemySightDownRight(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightDownRight(TileMap, VisionField, X + 1, Y - 1);
        }
        else
        {
            ValidateEnemySightDownRight(TileMap, VisionField, X + 1, Y - 1, false);
        }
    }
}

internal void
ValidateEnemySightUpRight(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightUpRight(TileMap, VisionField, X + 1, Y + 1);
        }
        else
        {
            ValidateEnemySightUpRight(TileMap, VisionField, X + 1, Y + 1, false);
        }
    }
}

internal void
ValidateEnemySightUpLeft(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightUpLeft(TileMap, VisionField, X - 1, Y + 1);
        }
        else
        {
            ValidateEnemySightUpLeft(TileMap, VisionField, X - 1, Y + 1, false);
        }
    }
}

internal void
ValidateEnemySightUp(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightUp(TileMap, VisionField, X, Y + 1);
            ValidateEnemySightUpLeft(TileMap, VisionField, X - 1, Y + 1);
            ValidateEnemySightUpRight(TileMap, VisionField, X + 1, Y + 1);
        }
        else
        {
            ValidateEnemySightUp(TileMap, VisionField, X, Y + 1, false);
            ValidateEnemySightUpLeft(TileMap, VisionField, X - 1, Y + 1, false);
            ValidateEnemySightUpRight(TileMap, VisionField, X + 1, Y + 1, false);
        }
    }
}

internal void
ValidateEnemySightDown(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightDown(TileMap, VisionField, X, Y - 1);
            ValidateEnemySightDownLeft(TileMap, VisionField, X - 1, Y - 1);
            ValidateEnemySightDownRight(TileMap, VisionField, X + 1, Y - 1);
        }
        else
        {
            ValidateEnemySightDown(TileMap, VisionField, X, Y - 1, false);
            ValidateEnemySightDownLeft(TileMap, VisionField, X - 1, Y - 1, false);
            ValidateEnemySightDownRight(TileMap, VisionField, X + 1, Y - 1, false);
        }
    }
}

internal void
ValidateEnemySightLeft(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightLeft(TileMap, VisionField, X - 1, Y);
            ValidateEnemySightUpLeft(TileMap, VisionField, X - 1, Y + 1);
            ValidateEnemySightDownLeft(TileMap, VisionField, X - 1, Y - 1);
        }
        else
        {
            ValidateEnemySightLeft(TileMap, VisionField, X - 1, Y, false);
            ValidateEnemySightDownLeft(TileMap, VisionField, X - 1, Y - 1, false);
            ValidateEnemySightUpLeft(TileMap, VisionField, X - 1, Y + 1, false);
        }
    }
}

internal void
ValidateEnemySightRight(tile_map *TileMap, vision_field *VisionField, u32 X, u32 Y, b32 Valid = true)
{
    if(!Valid)
    {
        VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
    }

    if(X > 0 &&
       X < VisionField->SightTileCountX-1 &&
       Y > 0 &&
       Y < VisionField->SightTileCountY-1)
    {
        if(VisionField->SightTiles[Y*VisionField->SightTileCountX + X])
        {
            ValidateEnemySightRight(TileMap, VisionField, X + 1, Y);
            ValidateEnemySightDownRight(TileMap, VisionField, X + 1, Y - 1);
            ValidateEnemySightUpRight(TileMap, VisionField, X + 1, Y + 1);
        }
        else
        {
            ValidateEnemySightRight(TileMap, VisionField, X + 1, Y, false);
            ValidateEnemySightDownRight(TileMap, VisionField, X + 1, Y - 1, false);
            ValidateEnemySightUpRight(TileMap, VisionField, X + 1, Y + 1, false);
        }
    }
}

internal void
ValidateEnemyLineOfSight(tile_map *TileMap, entity *Enemy)
{

    vision_field *VisionField = &Enemy->VisionFields.VisionField;

    Assert((VisionField->SightTileCountX % 2) == 1);
    Assert((VisionField->SightTileCountY % 2) == 1);

    s32 TileCenterX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileCenterY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    if(!VisionField->SightTiles[TileCenterY*VisionField->SightTileCountX + TileCenterX])
    {
        for(u32 Y = 0;
            Y < VisionField->SightTileCountY;
            ++Y)
        {
            for(u32 X = 0;
                X < VisionField->SightTileCountX;
                ++X)
            {
                VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
            }
        }
    }
    else
    {
        switch(Enemy->FacingDirection)
        {
            case Direction_Up:
            {
                ValidateEnemySightUp(TileMap, VisionField, TileCenterX, TileCenterY + 1);
                ValidateEnemySightUpLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY + 1);
                ValidateEnemySightUpRight(TileMap, VisionField, TileCenterX + 1, TileCenterY + 1);
            } break;

            case Direction_Left:
            {
                ValidateEnemySightLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY);
                ValidateEnemySightUpLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY + 1);
                ValidateEnemySightDownLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY - 1);
            } break;

            case Direction_Down:
            {
                ValidateEnemySightDown(TileMap, VisionField, TileCenterX, TileCenterY - 1);
                ValidateEnemySightDownRight(TileMap, VisionField, TileCenterX + 1, TileCenterY - 1);
                ValidateEnemySightDownLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY - 1);
            } break;

            case Direction_Right:
            {
                ValidateEnemySightRight(TileMap, VisionField, TileCenterX + 1, TileCenterY);
                ValidateEnemySightUpRight(TileMap, VisionField, TileCenterX + 1, TileCenterY + 1);
                ValidateEnemySightDownRight(TileMap, VisionField, TileCenterX + 1, TileCenterY - 1);
            } break;
        }
    }
    VisionField = &Enemy->VisionFields.NewVisionField;

    if(Enemy->ActionType == Action_Move)
    {
        Assert((VisionField->SightTileCountX % 2) == 1);
        Assert((VisionField->SightTileCountY % 2) == 1);

        TileCenterX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
        TileCenterY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

        tile_map_position NewEnemyP = Enemy->P;
        NewEnemyP.AbsTileX += (s32)Enemy->dP.x;
        NewEnemyP.AbsTileY += (s32)Enemy->dP.y;

        if(!VisionField->SightTiles[TileCenterY*VisionField->SightTileCountX + TileCenterX])
        {
            for(u32 Y = 0;
                Y < VisionField->SightTileCountY;
                ++Y)
            {
                for(u32 X = 0;
                    X < VisionField->SightTileCountX;
                    ++X)
                {
                    VisionField->SightTiles[Y*VisionField->SightTileCountX + X] = false;
                }
            }
        }
        else
        {

            switch((tile_direction)Enemy->FacingDirection)
            {
                case Direction_Up:
                {
                    ValidateEnemySightUp(TileMap, VisionField, TileCenterX, TileCenterY + 1);
                    ValidateEnemySightUpLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY + 1);
                    ValidateEnemySightUpRight(TileMap, VisionField, TileCenterX + 1, TileCenterY + 1);
                } break;

                case Direction_Left:
                {
                    ValidateEnemySightLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY);
                    ValidateEnemySightUpLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY + 1);
                    ValidateEnemySightDownLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY - 1);
                } break;

                case Direction_Down:
                {
                    ValidateEnemySightDown(TileMap, VisionField, TileCenterX, TileCenterY - 1);
                    ValidateEnemySightDownRight(TileMap, VisionField, TileCenterX + 1, TileCenterY - 1);
                    ValidateEnemySightDownLeft(TileMap, VisionField, TileCenterX - 1, TileCenterY - 1);
                } break;

                case Direction_Right:
                {
                    ValidateEnemySightRight(TileMap, VisionField, TileCenterX + 1, TileCenterY);
                    ValidateEnemySightUpRight(TileMap, VisionField, TileCenterX + 1, TileCenterY + 1);
                    ValidateEnemySightDownRight(TileMap, VisionField, TileCenterX + 1, TileCenterY - 1);
                } break;
            }
        }
    }

}

internal void
CheckIfNinjaVisible(game_mode_level *LevelMode, tile_map *TileMap, entity *Enemy,
                    entity *RedNinja, entity *BlueNinja)
{
    vision_field *VisionField = &Enemy->VisionFields.VisionField;
    vision_field *NewVisionField = &Enemy->VisionFields.NewVisionField;

    s32 TileCenterX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileCenterY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    s32 TileHalfCountX = FloorR32ToS32((r32)VisionField->SightTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)VisionField->SightTileCountY/2);

    tile_map_position CleanEnemyP = {};
    CleanEnemyP.AbsTileX = Enemy->P.AbsTileX;
    CleanEnemyP.AbsTileY = Enemy->P.AbsTileY;
    CleanEnemyP.AbsTileZ = Enemy->P.AbsTileZ;

    tile_map_position CleanNewEnemyP = {};
    CleanNewEnemyP.AbsTileX = Enemy->P.AbsTileX + (s32)Enemy->dP.x;
    CleanNewEnemyP.AbsTileY = Enemy->P.AbsTileY + (s32)Enemy->dP.y;
    CleanNewEnemyP.AbsTileZ = Enemy->P.AbsTileZ;

    tile_map_position CleanRedP = {};
    CleanRedP.AbsTileX = RedNinja->P.AbsTileX;
    CleanRedP.AbsTileY = RedNinja->P.AbsTileY;
    CleanRedP.AbsTileZ = RedNinja->P.AbsTileZ;

    tile_map_position CleanBlueP = {};
    CleanBlueP.AbsTileX = BlueNinja->P.AbsTileX;
    CleanBlueP.AbsTileY = BlueNinja->P.AbsTileY;
    CleanBlueP.AbsTileZ = BlueNinja->P.AbsTileZ;

    tile_map_position RedSmokeP = {};
    if(RedNinja->Emitters.ParticleEmitter.Active)
    {
        RedSmokeP = RedNinja->Emitters.ParticleEmitter.TilePos;
        RedSmokeP.Offset = V2(0, 0);
    }
    tile_map_position BlueSmokeP = {};
    if(BlueNinja->Emitters.ParticleEmitter.Active)
    {
        BlueSmokeP = BlueNinja->Emitters.ParticleEmitter.TilePos;
        BlueSmokeP.Offset = V2(0, 0);
    }

    s32 RedSmokeTileCenterX = FloorR32ToS32((r32)RedNinja->Emitters.ParticleEmitter.SimTileCountX/2);
    s32 RedSmokeTileCenterY = FloorR32ToS32((r32)RedNinja->Emitters.ParticleEmitter.SimTileCountY/2);

    s32 BlueSmokeTileCenterX = FloorR32ToS32((r32)BlueNinja->Emitters.ParticleEmitter.SimTileCountX/2);
    s32 BlueSmokeTileCenterY = FloorR32ToS32((r32)BlueNinja->Emitters.ParticleEmitter.SimTileCountY/2);

    s32 RedSmokeTileHalfCountX = FloorR32ToS32((r32)RedNinja->Emitters.ParticleEmitter.SimTileCountX/2);
    s32 RedSmokeTileHalfCountY = FloorR32ToS32((r32)RedNinja->Emitters.ParticleEmitter.SimTileCountY/2);

    s32 BlueSmokeTileHalfCountX = FloorR32ToS32((r32)BlueNinja->Emitters.ParticleEmitter.SimTileCountX/2);
    s32 BlueSmokeTileHalfCountY = FloorR32ToS32((r32)BlueNinja->Emitters.ParticleEmitter.SimTileCountY/2);

    particle_emitter *RedEmitter = &RedNinja->Emitters.ParticleEmitter;
    particle_emitter *BlueEmitter = &BlueNinja->Emitters.ParticleEmitter;
    for(s32 EnemyVisionTileIndexY = -TileHalfCountY;
        EnemyVisionTileIndexY < TileHalfCountY;
        ++EnemyVisionTileIndexY)
    {
        for(s32 EnemyVisionTileIndexX = -TileHalfCountX;
            EnemyVisionTileIndexX < TileHalfCountX;
            ++EnemyVisionTileIndexX)
        {
            if(VisionField->SightTiles[(EnemyVisionTileIndexY + TileHalfCountY)*VisionField->SightTileCountX + EnemyVisionTileIndexX + TileHalfCountX])
            {
                tile_map_position TestP = CleanEnemyP;
                TestP.AbsTileX += EnemyVisionTileIndexX;
                TestP.AbsTileY += EnemyVisionTileIndexY;

                tile_map_difference RedDiff = Subtract(TileMap, &TestP, &RedSmokeP);
                tile_map_difference BlueDiff = Subtract(TileMap, &TestP, &BlueSmokeP);

                s32 RedX = FloorR32ToS32(RedDiff.dxy.x);
                s32 RedY = FloorR32ToS32(RedDiff.dxy.y);
                s32 BlueX = FloorR32ToS32(BlueDiff.dxy.x);
                s32 BlueY = FloorR32ToS32(BlueDiff.dxy.y);

                if(RedEmitter->Active &&
                   RedEmitter->TurnLifetime > 0)
                {
                    if(RedX > -RedSmokeTileHalfCountX &&
                       RedX < RedSmokeTileHalfCountX &&
                       RedY > -RedSmokeTileHalfCountY &&
                       RedY < RedSmokeTileHalfCountY)
                    {
                        if(RedEmitter->SimTiles[(RedY + RedSmokeTileHalfCountY)*RedEmitter->SimTileCountX + RedX + RedSmokeTileHalfCountX].Valid)
                        {
                            VisionField->SightTiles[(EnemyVisionTileIndexY + TileHalfCountY)*VisionField->SightTileCountX + EnemyVisionTileIndexX + TileHalfCountX] = false;
                        }
                    }


                }

                if(BlueEmitter->Active &&
                   BlueEmitter->TurnLifetime > 0)
                {
                    if(BlueX > -BlueSmokeTileHalfCountX &&
                       BlueX < BlueSmokeTileHalfCountX &&
                       BlueY > -BlueSmokeTileHalfCountY &&
                       BlueY < BlueSmokeTileHalfCountY)
                    {
                        if(BlueEmitter->SimTiles[(BlueY + BlueSmokeTileHalfCountY)*BlueEmitter->SimTileCountX + BlueX + BlueSmokeTileHalfCountX].Valid)
                        {
                            VisionField->SightTiles[(EnemyVisionTileIndexY + TileHalfCountY)*VisionField->SightTileCountX + EnemyVisionTileIndexX + TileHalfCountX] = false;
                        }
                    }

                }

            }

            if(NewVisionField->SightTiles[(EnemyVisionTileIndexY + TileHalfCountY)*NewVisionField->SightTileCountX + EnemyVisionTileIndexX + TileHalfCountX])
            {

                tile_map_position NewTestP = CleanNewEnemyP;
                NewTestP.AbsTileX += EnemyVisionTileIndexX;
                NewTestP.AbsTileY += EnemyVisionTileIndexY;

                tile_map_difference NewRedDiff = Subtract(TileMap, &NewTestP, &RedSmokeP);
                tile_map_difference NewBlueDiff = Subtract(TileMap, &NewTestP, &BlueSmokeP);

                s32 NewRedX = FloorR32ToS32(NewRedDiff.dxy.x);
                s32 NewRedY = FloorR32ToS32(NewRedDiff.dxy.y);
                s32 NewBlueX = FloorR32ToS32(NewBlueDiff.dxy.x);
                s32 NewBlueY = FloorR32ToS32(NewBlueDiff.dxy.y);

                if(RedEmitter->Active &&
                   RedEmitter->TurnLifetime > 0)
                {
                    if(NewRedX > -RedSmokeTileHalfCountX &&
                       NewRedX < RedSmokeTileHalfCountX &&
                       NewRedY > -RedSmokeTileHalfCountY &&
                       NewRedY < RedSmokeTileHalfCountY)
                    {
                        if(RedEmitter->SimTiles[(NewRedY + RedSmokeTileHalfCountY)*RedEmitter->SimTileCountX + NewRedX + RedSmokeTileHalfCountX].Valid)
                        {
                            NewVisionField->SightTiles[(EnemyVisionTileIndexY + TileHalfCountY)*NewVisionField->SightTileCountX + EnemyVisionTileIndexX + TileHalfCountX] = false;
                        }
                    }
                }

                if(BlueEmitter->Active &&
                   BlueEmitter->TurnLifetime > 0)
                {
                    if(NewBlueX > -BlueSmokeTileHalfCountX &&
                       NewBlueX < BlueSmokeTileHalfCountX &&
                       NewBlueY > -BlueSmokeTileHalfCountY &&
                       NewBlueY < BlueSmokeTileHalfCountY)
                    {
                        if(BlueEmitter->SimTiles[(NewBlueY + BlueSmokeTileHalfCountY)*BlueEmitter->SimTileCountX + NewBlueX + BlueSmokeTileHalfCountX].Valid)
                        {
                            NewVisionField->SightTiles[(EnemyVisionTileIndexY + TileHalfCountY)*NewVisionField->SightTileCountX + EnemyVisionTileIndexX + TileHalfCountX] = false;
                        }
                    }
                }
            }
        }
    }

    ValidateEnemyLineOfSight(TileMap, Enemy);

    tile_map_difference RedDiff = Subtract(TileMap, &CleanRedP, &CleanEnemyP);
    tile_map_difference BlueDiff = Subtract(TileMap, &CleanBlueP, &CleanEnemyP);

    s32 RedX = FloorR32ToS32(RedDiff.dxy.x);
    s32 RedY = FloorR32ToS32(RedDiff.dxy.y);
    s32 BlueX = FloorR32ToS32(BlueDiff.dxy.x);
    s32 BlueY = FloorR32ToS32(BlueDiff.dxy.y);

    if((RedX == 0) &&
       (RedY == 0) ||
       (BlueX == 0) &&
       (BlueY == 0))
    {
        LevelMode->GameOver = true;
        Enemy->Actions[Enemy->ActionCount].Type = Action_Move;
        Enemy->Actions[Enemy->ActionCount].Position = {};
        Enemy->Actions[Enemy->ActionCount].Position.Offset = V2(0, 0);
        Enemy->ChasingPlayer = true;

    }

    if(!(RedX < -TileHalfCountX ||
       RedX > TileHalfCountX ||
       RedY < -TileHalfCountY ||
         RedY > TileHalfCountY))
    {
        if(VisionField->SightTiles[(TileCenterY + RedY)*VisionField->SightTileCountX + (TileCenterX + RedX)])
        {
            LevelMode->GameOver = true;
            Enemy->Actions[Enemy->ActionCount].Type = Action_Move;
            Enemy->Actions[Enemy->ActionCount].Position = {};
            Enemy->Actions[Enemy->ActionCount].Position.Offset = V2((r32)RedX, (r32)RedY);
            Enemy->ChasingPlayer = true;
        }
    }

    if(!(BlueX < -TileHalfCountX ||
       BlueX > TileHalfCountX ||
       BlueY < -TileHalfCountY ||
       BlueY > TileHalfCountY))
    {
        if(VisionField->SightTiles[(TileCenterY + BlueY)*VisionField->SightTileCountX + (TileCenterX + BlueX)])
        {
            LevelMode->GameOver = true;
            Enemy->Actions[Enemy->ActionCount].Type = Action_Move;
            Enemy->Actions[Enemy->ActionCount].Position = {};
            Enemy->Actions[Enemy->ActionCount].Position.Offset = V2((r32)BlueX, (r32)BlueY);
            Enemy->ChasingPlayer = true;
        }
    }

}
