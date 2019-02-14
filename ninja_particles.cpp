
internal b32
CheckObstaclesUp(tile_map *TileMap, particle_emitter *Emitter,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                 b32 Continue = true, particle_obstacle_rules Rule = ParticleRule_Envelop)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        tile_map_position NextTileA = CurrentTile;
                        NextTileA.AbsTileY += 1;
                        NextTileA.AbsTileX -= 1;
                        tile_map_position NextTileB = CurrentTile;
                        NextTileB.AbsTileY += 1;
                        tile_map_position NextTileC = CurrentTile;
                        NextTileC.AbsTileY += 1;
                        NextTileC.AbsTileX += 1;
                        CheckObstaclesUpLeft(TileMap, Emitter, CurrentTile, NextTileA, Continue, Rule);
                        CheckObstaclesUp(TileMap, Emitter, CurrentTile, NextTileB, Continue, Rule);
                        CheckObstaclesUpRight(TileMap, Emitter, CurrentTile, NextTileC, Continue, Rule);
                        
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY += 1;
                        
                        CheckObstaclesUp(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        
                    } break;
                }
    
            }
        }
    }

    return Result;
}

internal b32
CheckObstaclesDown(tile_map *TileMap, particle_emitter *Emitter,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true, particle_obstacle_rules Rule = ParticleRule_Envelop)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        
                        tile_map_position NextTileA = CurrentTile;
                        NextTileA.AbsTileY -= 1;
                        NextTileA.AbsTileX -= 1;
                        tile_map_position NextTileB = CurrentTile;
                        NextTileB.AbsTileY -= 1;
                        tile_map_position NextTileC = CurrentTile;
                        NextTileC.AbsTileY -= 1;
                        NextTileC.AbsTileX += 1;
                        CheckObstaclesDownLeft(TileMap, Emitter, CurrentTile, NextTileA, Continue, Rule);
                        CheckObstaclesDown(TileMap, Emitter, CurrentTile, NextTileB, Continue, Rule);
                        CheckObstaclesDownRight(TileMap, Emitter, CurrentTile, NextTileC, Continue, Rule);
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY -= 1;
                        
                        CheckObstaclesDown(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        
                    } break;
                }
    
            }
        }
    }
    
    return Result;
}

internal b32
CheckObstaclesLeft(tile_map *TileMap, particle_emitter *Emitter,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                   b32 Continue = true, particle_obstacle_rules Rule = ParticleRule_Envelop)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        
                        tile_map_position NextTileA = CurrentTile;
                        NextTileA.AbsTileY -= 1;
                        NextTileA.AbsTileX -= 1;
                        tile_map_position NextTileB = CurrentTile;
                        NextTileB.AbsTileX -= 1;
                        tile_map_position NextTileC = CurrentTile;
                        NextTileC.AbsTileY += 1;
                        NextTileC.AbsTileX -= 1;
                        CheckObstaclesDownLeft(TileMap, Emitter, CurrentTile, NextTileA, Continue, Rule);
                        CheckObstaclesLeft(TileMap, Emitter, CurrentTile, NextTileB, Continue, Rule);
                        CheckObstaclesUpLeft(TileMap, Emitter, CurrentTile, NextTileC, Continue, Rule);
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileX -= 1;
                        
                        CheckObstaclesLeft(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        
                    } break;
                }
                
            }
        }
    }

    return Result;
}

internal b32
CheckObstaclesRight(tile_map *TileMap, particle_emitter *Emitter,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                    b32 Continue = true, particle_obstacle_rules Rule = ParticleRule_Envelop)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        tile_map_position NextTileA = CurrentTile;
                        NextTileA.AbsTileY -= 1;
                        NextTileA.AbsTileX += 1;
                        tile_map_position NextTileB = CurrentTile;
                        NextTileB.AbsTileX += 1;
                        tile_map_position NextTileC = CurrentTile;
                        NextTileC.AbsTileY += 1;
                        NextTileC.AbsTileX += 1;
                        CheckObstaclesDownRight(TileMap, Emitter, CurrentTile, NextTileA, Continue, Rule);
                        CheckObstaclesRight(TileMap, Emitter, CurrentTile, NextTileB, Continue, Rule);
                        CheckObstaclesUpRight(TileMap, Emitter, CurrentTile, NextTileC, Continue, Rule);
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileX += 1;
                        
                        CheckObstaclesRight(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        
                    } break;
                }
                
            }
        }
    }

    return Result;
}

internal b32
CheckObstaclesUpLeft(tile_map *TileMap, particle_emitter *Emitter,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                     b32 Continue, particle_obstacle_rules Rule)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
           !(!CheckObstaclesUp(TileMap, Emitter, PreviousTile, RightSideTile, false) &&
             !CheckObstaclesLeft(TileMap, Emitter, PreviousTile, DownSideTile, false)) &&
           !(!CheckObstaclesUp(TileMap, Emitter, DownSideTile, CurrentTile, false) &&
             !CheckObstaclesLeft(TileMap, Emitter, RightSideTile, CurrentTile, false)) &&
           !(!CheckObstaclesLeft(TileMap, Emitter, RightSideTile, CurrentTile, false) && 
             !CheckObstaclesLeft(TileMap, Emitter, PreviousTile, DownSideTile, false)) &&
           !(!CheckObstaclesUp(TileMap, Emitter, PreviousTile, RightSideTile, false) &&
             !CheckObstaclesUp(TileMap, Emitter, DownSideTile, CurrentTile, false)))
        {
            Result = true;
            
            if(Continue)
            {
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY += 1;
                        NextTile.AbsTileX -= 1;
                        
                        tile_map_position NextTileUp = CurrentTile;
                        NextTileUp.AbsTileY += 1;
                        
                        tile_map_position NextTileLeft = CurrentTile;
                        NextTileLeft.AbsTileX -= 1;
                        
                        CheckObstaclesUpLeft(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        CheckObstaclesUp(TileMap, Emitter, CurrentTile, NextTileUp, Continue, Rule);
                        CheckObstaclesLeft(TileMap, Emitter, CurrentTile, NextTileLeft, Continue, Rule);
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY += 1;
                        NextTile.AbsTileX -= 1;
                        
                        tile_map_position NextTileUp = CurrentTile;
                        NextTileUp.AbsTileY += 1;
                        
                        tile_map_position NextTileLeft = CurrentTile;
                        NextTileLeft.AbsTileX -= 1;
                        
                        CheckObstaclesUpLeft(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        
                        if(CheckObstaclesLeft(TileMap, Emitter, RightSideTile, CurrentTile, false))
                        {
                            CheckObstaclesUp(TileMap, Emitter, CurrentTile, NextTileUp, Continue, Rule);
                        }
                            
                        if(CheckObstaclesUp(TileMap, Emitter, DownSideTile, CurrentTile, false))
                        {
                            CheckObstaclesLeft(TileMap, Emitter, CurrentTile, NextTileLeft, Continue, Rule);
                        }
                        
                    } break;
                }
                
            }
        }
    }
    return Result;
}

internal b32
CheckObstaclesUpRight(tile_map *TileMap, particle_emitter *Emitter,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                      b32 Continue, particle_obstacle_rules Rule)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
           !(!CheckObstaclesUp(TileMap, Emitter, PreviousTile, LeftSideTile, false) &&
             !CheckObstaclesRight(TileMap, Emitter, PreviousTile, DownSideTile, false)) &&
           !(!CheckObstaclesUp(TileMap, Emitter, DownSideTile, CurrentTile, false) &&
             !CheckObstaclesRight(TileMap, Emitter, LeftSideTile, CurrentTile, false)) &&
           !(!CheckObstaclesRight(TileMap, Emitter, LeftSideTile, CurrentTile, false) && 
             !CheckObstaclesRight(TileMap, Emitter, PreviousTile, DownSideTile, false)) &&
           !(!CheckObstaclesUp(TileMap, Emitter, PreviousTile, LeftSideTile, false) &&
             !CheckObstaclesUp(TileMap, Emitter, DownSideTile, CurrentTile, false)))
        {
            Result = true;
            
            if(Continue)
            {
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY += 1;
                        NextTile.AbsTileX += 1;
                        
                        tile_map_position NextTileUp = CurrentTile;
                        NextTileUp.AbsTileY += 1;
                        
                        tile_map_position NextTileRight = CurrentTile;
                        NextTileRight.AbsTileX += 1;
                        
                        CheckObstaclesUpRight(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        CheckObstaclesUp(TileMap, Emitter, CurrentTile, NextTileUp, Continue, Rule);
                        CheckObstaclesRight(TileMap, Emitter, CurrentTile, NextTileRight, Continue, Rule);
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY += 1;
                        NextTile.AbsTileX += 1;
                        
                        tile_map_position NextTileUp = CurrentTile;
                        NextTileUp.AbsTileY += 1;
                        
                        tile_map_position NextTileRight = CurrentTile;
                        NextTileRight.AbsTileX += 1;
                        
                        CheckObstaclesUpRight(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        
                        if(CheckObstaclesRight(TileMap, Emitter, LeftSideTile, CurrentTile, false))
                        {
                            CheckObstaclesUp(TileMap, Emitter, CurrentTile, NextTileUp, Continue, Rule);
                        }
                        
                        
                        if(CheckObstaclesUp(TileMap, Emitter, DownSideTile, CurrentTile, false))
                        {
                            CheckObstaclesRight(TileMap, Emitter, CurrentTile, NextTileRight, Continue, Rule);
                        }
                        
                    } break;
                }
    
            }
        }
    }
    
    return Result;
}

internal b32
CheckObstaclesDownLeft(tile_map *TileMap, particle_emitter *Emitter,
                 tile_map_position PreviousTile, tile_map_position CurrentTile,
                       b32 Continue, particle_obstacle_rules Rule)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
           !(!CheckObstaclesDown(TileMap, Emitter, PreviousTile, RightSideTile, false) &&
             !CheckObstaclesLeft(TileMap, Emitter, PreviousTile, UpSideTile, false)) &&
           !(!CheckObstaclesDown(TileMap, Emitter, UpSideTile, CurrentTile, false) &&
             !CheckObstaclesLeft(TileMap, Emitter, RightSideTile, CurrentTile, false)) &&
           !(!CheckObstaclesLeft(TileMap, Emitter, RightSideTile, CurrentTile, false) && 
             !CheckObstaclesLeft(TileMap, Emitter, PreviousTile, UpSideTile, false)) &&
           !(!CheckObstaclesDown(TileMap, Emitter, PreviousTile, RightSideTile, false) &&
             !CheckObstaclesDown(TileMap, Emitter, UpSideTile, CurrentTile, false)))
        {
            Result = true;
            
            if(Continue)
            {
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY -= 1;
                        NextTile.AbsTileX -= 1;
                        
                        tile_map_position NextTileDown = CurrentTile;
                        NextTileDown.AbsTileY -= 1;
                        
                        tile_map_position NextTileLeft = CurrentTile;
                        NextTileLeft.AbsTileX -= 1;
                        
                        CheckObstaclesDownLeft(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        CheckObstaclesDown(TileMap, Emitter, CurrentTile, NextTileDown, Continue, Rule);
                        CheckObstaclesLeft(TileMap, Emitter, CurrentTile, NextTileLeft, Continue, Rule);
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY -= 1;
                        NextTile.AbsTileX -= 1;
                        
                        tile_map_position NextTileDown = CurrentTile;
                        NextTileDown.AbsTileY -= 1;
                        
                        tile_map_position NextTileLeft = CurrentTile;
                        NextTileLeft.AbsTileX -= 1;
                        
                        CheckObstaclesDownLeft(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                                                
                        if(CheckObstaclesLeft(TileMap, Emitter, RightSideTile, CurrentTile, false))
                        {
                            CheckObstaclesDown(TileMap, Emitter, CurrentTile, NextTileDown, Continue, Rule);
                        }
                        
                        if(CheckObstaclesDown(TileMap, Emitter, UpSideTile, CurrentTile, false))
                        {
                            CheckObstaclesLeft(TileMap, Emitter, CurrentTile, NextTileLeft, Continue, Rule);
                        }
                        
                    } break;
                }
                
            }
        }
    }
    
    return Result;
}

internal b32
CheckObstaclesDownRight(tile_map *TileMap, particle_emitter *Emitter,
                        tile_map_position PreviousTile, tile_map_position CurrentTile,
                        b32 Continue, particle_obstacle_rules Rule)
{
    b32 Result = false;
    
    s32 TileHalfCountX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileHalfCountY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    s32 DiffTileX = CurrentTile.AbsTileX - Emitter->TilePos.AbsTileX;
    s32 DiffTileY = CurrentTile.AbsTileY - Emitter->TilePos.AbsTileY;
    
    if(TileHalfCountX != (s32)AbsoluteValue((r32)DiffTileX) &&
       TileHalfCountY != (s32)AbsoluteValue((r32)DiffTileY))
    {
        particle_tile *CurrTile = &Emitter->SimTiles[(TileHalfCountY + DiffTileY)*Emitter->SimTileCountX + (TileHalfCountX + DiffTileX)];
        
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
           !(!CheckObstaclesDown(TileMap, Emitter, PreviousTile, UpSideTile, false) &&
             !CheckObstaclesRight(TileMap, Emitter, PreviousTile, LeftSideTile, false)) &&
           !(!CheckObstaclesDown(TileMap, Emitter, UpSideTile, CurrentTile, false) &&
             !CheckObstaclesRight(TileMap, Emitter, LeftSideTile, CurrentTile, false)) &&
           !(!CheckObstaclesRight(TileMap, Emitter, LeftSideTile, CurrentTile, false) && 
             !CheckObstaclesRight(TileMap, Emitter, PreviousTile, UpSideTile, false)) &&
           !(!CheckObstaclesDown(TileMap, Emitter, PreviousTile, LeftSideTile, false) &&
             !CheckObstaclesDown(TileMap, Emitter, UpSideTile, CurrentTile, false)))
        {
            Result = true;
            
            
            if(Continue)
            {
                CurrTile->Valid = true;
                
                switch(Rule)
                {
                    case ParticleRule_Envelop:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY -= 1;
                        NextTile.AbsTileX += 1;
                        
                        tile_map_position NextTileDown = CurrentTile;
                        NextTileDown.AbsTileY -= 1;
                        
                        tile_map_position NextTileRight = CurrentTile;
                        NextTileRight.AbsTileX += 1;
                        
                        CheckObstaclesDownRight(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        CheckObstaclesDown(TileMap, Emitter, CurrentTile, NextTileDown, Continue, Rule);
                        CheckObstaclesRight(TileMap, Emitter, CurrentTile, NextTileRight, Continue, Rule);
                    } break;
                    
                    case ParticleRule_Scatter:
                    {
                        tile_map_position NextTile = CurrentTile;
                        NextTile.AbsTileY -= 1;
                        NextTile.AbsTileX += 1;

                        tile_map_position NextTileDown = CurrentTile;
                        NextTileDown.AbsTileY -= 1;
                        
                        tile_map_position NextTileRight = CurrentTile;
                        NextTileRight.AbsTileX += 1;
                        
                        CheckObstaclesDownRight(TileMap, Emitter, CurrentTile, NextTile, Continue, Rule);
                        
                        if(CheckObstaclesRight(TileMap, Emitter, LeftSideTile, CurrentTile, false))
                        {
                            CheckObstaclesDown(TileMap, Emitter, CurrentTile, NextTileDown, Continue, Rule);
                        }
                        
                        if(CheckObstaclesDown(TileMap, Emitter, UpSideTile, CurrentTile, false))
                        {
                            CheckObstaclesRight(TileMap, Emitter, CurrentTile, NextTileRight, Continue, Rule);
                        }
                        
                    } break;
                }
            }
        }
    }

    return Result;
}

internal void
CheckParticleObstacles(tile_map *TileMap, particle_emitter *Emitter, 
                       particle_obstacle_rules Rule = ParticleRule_Envelop)
{
    Assert((Emitter->SimTileCountX % 2) == 1);
    Assert((Emitter->SimTileCountY % 2) == 1);
    
    s32 TileCenterX = FloorR32ToS32((r32)Emitter->SimTileCountX/2);
    s32 TileCenterY = FloorR32ToS32((r32)Emitter->SimTileCountY/2);
    
    Emitter->SimTiles[TileCenterY*Emitter->SimTileCountX + TileCenterX].Valid = true;
    
    tile_map_position TilePos = Emitter->TilePos;
    tile_map_position TilePosUp = Emitter->TilePos;
    TilePosUp.AbsTileY += 1;
    tile_map_position TilePosUpLeft = Emitter->TilePos;
    TilePosUpLeft.AbsTileY += 1;
    TilePosUpLeft.AbsTileX -= 1;
    tile_map_position TilePosUpRight = Emitter->TilePos;
    TilePosUpRight.AbsTileY += 1;
    TilePosUpRight.AbsTileX += 1;
    
    tile_map_position TilePosDown = Emitter->TilePos;
    TilePosDown.AbsTileY -= 1;
    tile_map_position TilePosDownLeft = Emitter->TilePos;
    TilePosDownLeft.AbsTileY -= 1;
    TilePosDownLeft.AbsTileX -= 1;
    tile_map_position TilePosDownRight = Emitter->TilePos;
    TilePosDownRight.AbsTileY -= 1;
    TilePosDownRight.AbsTileX += 1;
    
    tile_map_position TilePosLeft = Emitter->TilePos;
    TilePosLeft.AbsTileX -= 1;
    
    tile_map_position TilePosRight = Emitter->TilePos;
    TilePosRight.AbsTileX += 1;
    
    CheckObstaclesUp(TileMap, Emitter, TilePos, TilePosUp, true, Rule);
    CheckObstaclesUpLeft(TileMap, Emitter, TilePos, TilePosUpLeft, true, Rule);
    CheckObstaclesUpRight(TileMap, Emitter, TilePos, TilePosUpRight, true, Rule);
    CheckObstaclesDown(TileMap, Emitter, TilePos, TilePosDown, true, Rule);
    CheckObstaclesDownLeft(TileMap, Emitter, TilePos, TilePosDownLeft, true, Rule);
    CheckObstaclesDownRight(TileMap, Emitter, TilePos, TilePosDownRight, true, Rule);
    CheckObstaclesLeft(TileMap, Emitter, TilePos, TilePosLeft, true, Rule);
    CheckObstaclesRight(TileMap, Emitter, TilePos, TilePosRight, true, Rule);
}

internal particle_emitter
DefaultSmokeEmitter()
{
    particle_emitter Result;
    
    Result.ParticleCount = 7*7*16;
    Result.SimTileCountX = 7;
    Result.SimTileCountY = 7;
    Result.SimDim = V2(7, 7);
    
    Result.DensityCoefficient = 0.2f;
    Result.InvalidLifetime = 0.5f;
    Result.BoundsLifetime = 1.0f;
    Result.MaxVelocity = 0.25f;
    Result.Alpha = 0.2f;
    Result.DyingThreshold = 1.0f;
    Result.EntryThreshold = 2.0f;
    Result.EntryFadeTime = 2.0f;
    Result.SpawnRadius = 2.0f;
    Result.SpiralCount = 5.0f;
    Result.CenterOffset = 0.4f;
    Result.OffsetVariance = 0.55f;
    Result.VelocityVariance = 0.25f;
    Result.BaseLifetime = 2.0f;
    Result.LifetimeVariance = 1.0f;
    Result.ParticleSize = 1.5f;
    Result.SortBias = 0.0f;
    
    Result.Initialized = false;
    Result.Active = false;
    
    return Result;
}

internal void
InitializeSmokeEmitter(particle_emitter *Emitter, memory_arena *Arena, 
                       tile_map *TileMap, tile_map_position TilePos,
                       tile_map_position CameraTilePos, loaded_bitmap *Bitmap,
                       u32 TurnLifetime)
{
    v2 EmitterDim = V2(0, 0);
    EmitterDim.x = (r32)Emitter->SimTileCountX/2;
    EmitterDim.y = (r32)Emitter->SimTileCountY/2;
    
    if(!Emitter->Initialized)
    {
        Emitter->Particles = PushArray(Arena, Emitter->ParticleCount, particle);
        Emitter->SimTiles = PushArray(Arena, Emitter->SimTileCountX*Emitter->SimTileCountY, particle_tile);
    
        Emitter->Initialized = true;
        Emitter->Active = false;
    }
    
    if(!Emitter->Active)
    {
        Emitter->TilePos = TilePos;
        
        CheckParticleObstacles(TileMap, Emitter, ParticleRule_Envelop);
        
        tile_map_difference Diff = Subtract(TileMap, &Emitter->TilePos, &CameraTilePos);
        
        Emitter->P = Diff.dxy;
        
        r32 Angle = 0.0f;
        
        r32 LifetimeIncrement = 1.0f/Emitter->ParticleCount;
        r32 LifetimeAccumulation = 0.0f;
        
        for(u32 ParticleIndex = 0;
            ParticleIndex < Emitter->ParticleCount;
            ++ParticleIndex)
        {
            particle *Particle = Emitter->Particles + ParticleIndex;
            
            Particle->Bitmap = Bitmap;
            Particle->Alive = false;
            Particle->MaxLifetime = 4.0f*LifetimeAccumulation;
            Particle->Lifetime = Particle->MaxLifetime;
            
            LifetimeAccumulation += LifetimeIncrement;
        }
        
        Emitter->TurnLifetime = TurnLifetime;
        
        Emitter->Active = true;
    }
}

internal void
DeactivateEmitter(particle_emitter *Emitter)
{
    if(Emitter->Initialized != false)
    {
        for(u32 Y = 0;
            Y < Emitter->SimTileCountY;
            ++Y)
        {
            for(u32 X = 0;
                X < Emitter->SimTileCountX;
                ++X)
            {
                Emitter->SimTiles[Y*Emitter->SimTileCountX + X].Valid = false;
            }
        }
        
        Emitter->Active = false;
    }
}

internal void
SimulateParticles(tile_map *TileMap, render_group *RenderGroup, particle_emitter *Emitter, 
                  r32 dt, tile_map_position *CameraP, random_series *Series)
{
    
    u32 SimTileCountX = Emitter->SimTileCountX;
    u32 SimTileCountY = Emitter->SimTileCountY;
    
    if(Emitter->TurnLifetime >= 0)
    {
        for(u32 ParticleIndex = 0;
            ParticleIndex < SimTileCountY*SimTileCountX;
            ++ParticleIndex)
        {
            particle_tile *SimTile = Emitter->SimTiles + ParticleIndex;
            SimTile->Density = 0.0f;
        }
        
        for(u32 ParticleIndex = 0;
            ParticleIndex < Emitter->ParticleCount;
            ++ParticleIndex)
        {
            particle *Particle = Emitter->Particles + ParticleIndex;
            if(Particle->Alive)
            {
                u32 SimTileX = FloorR32ToU32((Particle->OffsetP.x + Emitter->SimDim.x/2 - 0.001f));
                u32 SimTileY = FloorR32ToU32((Particle->OffsetP.y + Emitter->SimDim.y/2 - 0.001f));
                particle_tile *SimTile = Emitter->SimTiles + SimTileY*SimTileCountX + SimTileX;
                SimTile->Density += Emitter->DensityCoefficient;
            }
        }
        
        for(u32 ParticleIndex = 0;
            ParticleIndex < Emitter->ParticleCount;
            ++ParticleIndex)
        {
            particle *Particle = Emitter->Particles + ParticleIndex;
            if(Particle->Alive)
            {
                u32 SimTileX = FloorR32ToU32((Particle->OffsetP.x + Emitter->SimDim.x/2 - 0.001f));
                u32 SimTileY = FloorR32ToU32((Particle->OffsetP.y + Emitter->SimDim.y/2 - 0.001f));
                
                particle_tile *CurrentSimTile = Emitter->SimTiles + SimTileY*SimTileCountX + SimTileX;
                
                if(!CurrentSimTile->Valid)
                {
                    Particle->Lifetime = Minimum(Emitter->InvalidLifetime, Particle->Lifetime);
                }
                
                v2 NewAcceleration = V2(0, 0);
                
                if(CurrentSimTile->Valid)
                {
                    
                    if(SimTileX < SimTileCountX - 1)
                    {
                        particle_tile *SimTileA = Emitter->SimTiles + SimTileY*SimTileCountX + (SimTileX+1);
                        
                        NewAcceleration += V2(Maximum(CurrentSimTile->Density - SimTileA->Density, 0.0f), 0.0f);
                        
                    }
                    else
                    {
                        Particle->Lifetime = Minimum(Emitter->BoundsLifetime, Particle->Lifetime);
                    }
                    
                    if(SimTileX > 0)
                    {
                        particle_tile *SimTileB = Emitter->SimTiles + SimTileY*SimTileCountX + (SimTileX-1);
                        
                        NewAcceleration += V2(Minimum(SimTileB->Density - CurrentSimTile->Density, 0.0f), 0.0f);
                    }
                    else
                    {
                        Particle->Lifetime = Minimum(Emitter->BoundsLifetime, Particle->Lifetime);
                    }
                    
                    if(SimTileY < SimTileCountY - 1)
                    {
                        particle_tile *SimTileC = Emitter->SimTiles + (SimTileY+1)*SimTileCountX + SimTileX;
                        
                        NewAcceleration += V2(0.0f, Maximum(CurrentSimTile->Density - SimTileC->Density, 0.0f));
                    }
                    else
                    {
                        Particle->Lifetime = Minimum(Emitter->BoundsLifetime, Particle->Lifetime);
                    }
                    
                    if(SimTileY > 0)
                    {
                        particle_tile *SimTileD = Emitter->SimTiles + (SimTileY-1)*SimTileCountX + SimTileX;
                        
                        NewAcceleration += V2(0.0f, Minimum(SimTileD->Density - CurrentSimTile->Density, 0.0f));
                    }
                    else
                    {
                        Particle->Lifetime = Minimum(Emitter->BoundsLifetime, Particle->Lifetime);
                    }
                    
                    u32 Direction = ParticleIndex % 16;
                    switch(Direction)
                    {
                        case 0:
                        {
                            NewAcceleration += V2(-1.0f, 0);
                        } break;
                        
                        case 1:
                        {
                            NewAcceleration += V2(-0.75f, 0.25f);
                        } break;
                        
                        case 2:
                        {
                            NewAcceleration += V2(-0.5f, 0.5f);
                        } break;
                        
                        case 3:
                        {
                            NewAcceleration += V2(-0.25f, 0.75f);
                        } break;
                        
                        case 4:
                        {
                            NewAcceleration += V2(0.0f, 1.0f);
                        } break;
                        
                        case 5:
                        {
                            NewAcceleration += V2(0.25f, 0.75f);
                        } break;
                        
                        case 6:
                        {
                            NewAcceleration += V2(0.5, 0.5f);
                        } break;
                        
                        case 7:
                        {
                            NewAcceleration += V2(0.75f, 0.25f);
                        } break;
                        
                        case 8:
                        {
                            NewAcceleration += V2(1.0f, 0);
                        } break;
                        
                        case 9:
                        {
                            NewAcceleration += V2(0.75f, -0.25f);
                        } break;
                        
                        case 10:
                        {
                            NewAcceleration += V2(0.5f, -0.5f);
                        } break;
                        
                        case 11:
                        {
                            NewAcceleration += V2(0.25f, -0.75f);
                        } break;
                        
                        case 12:
                        {
                            NewAcceleration += V2(0.0f, -1.0f);
                        } break;
                        
                        case 13:
                        {
                            NewAcceleration += V2(-0.25f, -0.75f);
                        } break;
                        
                        case 14:
                        {
                            NewAcceleration += V2(-0.5, -0.5f);
                        } break;
                        
                        case 15:
                        {
                            NewAcceleration += V2(-0.75f, -0.25f);
                        } break;
                    }
                    
                    Particle->Velocity = Particle->Velocity + dt*NewAcceleration;
                    Particle->Velocity.x = Clamp(-Emitter->MaxVelocity, Particle->Velocity.x, Emitter->MaxVelocity);
                    Particle->Velocity.y = Clamp(-Emitter->MaxVelocity, Particle->Velocity.y, Emitter->MaxVelocity);
                    Particle->OffsetP = Particle->OffsetP + dt*Particle->Velocity;
                    v2 HalfSimDim = 0.5f*Emitter->SimDim;
                    
                    if(Particle->OffsetP.x > HalfSimDim.x)
                    {
                        Particle->OffsetP.x = HalfSimDim.x;
                        Particle->Velocity.x = 0;
                    }
                    if(Particle->OffsetP.x < -HalfSimDim.x)
                    {
                        Particle->OffsetP.x = -HalfSimDim.x;
                        Particle->Velocity.x = 0;
                    }
                    if(Particle->OffsetP.y > HalfSimDim.y)
                    {
                        Particle->OffsetP.y = HalfSimDim.y;
                        Particle->Velocity.y = 0;
                    }
                    if(Particle->OffsetP.y < -HalfSimDim.y)
                    {
                        Particle->OffsetP.y = -HalfSimDim.y;
                        Particle->Velocity.y = 0;
                    }
                }
                if(Emitter->TurnLifetime == 0)
                {
                    Particle->Lifetime -= 8.0f*dt;
                }
                else
                {
                    Particle->Lifetime -= dt;
                }
                
                r32 Alpha = Emitter->Alpha;
                
                if(Particle->Lifetime > 0 &&
                   Particle->Lifetime <= Emitter->DyingThreshold)
                {
                    Alpha = (Particle->Lifetime/Emitter->DyingThreshold)*Emitter->Alpha;
                }
                else if(Particle->Lifetime > Emitter->EntryThreshold)
                {
                    Alpha = Maximum(0.0f, Emitter->Alpha - (((Particle->Lifetime-Emitter->EntryThreshold)/Emitter->EntryFadeTime)*Emitter->Alpha));
                    if(Alpha < 0)
                    {
                        Alpha = 0.0f;
                    }
                }
                else if(Particle->Lifetime <= 0)
                {
                    Particle->Alive = false;
                    Alpha = 0.0f;
                }
                if(!CurrentSimTile->Valid)
                {
                    Alpha = 0.0f;
                }
                
                tile_map_difference Diff = Subtract(TileMap, &Emitter->TilePos, CameraP);
                
                Emitter->P = Diff.dxy;
                r32 MaxAlpha = Minimum(1.0f, Alpha + 0.5f);
                if(Alpha)
                {
                    PushSmokeParticle(RenderGroup, Particle->Bitmap, Emitter->ParticleSize, V3(Particle->OffsetP + Emitter->P, 0.0f), Direction_Down + Emitter->SortBias, V4(MaxAlpha, MaxAlpha, MaxAlpha, Alpha));
                }
                
            }
            else if(Particle->Lifetime < 0)
            {
                
                if(Emitter->TurnLifetime > 0)
                {
                    r32 ParticleTileOffset = Emitter->SpawnRadius*(r32)ParticleIndex/(r32)Emitter->ParticleCount;
                    
                    v2 Velocity = V2(0, 0);
                    r32 SpawnCoefficientX = 0.0f;
                    r32 SpawnCoefficientY = 0.0f;
                    u32 Direction = ParticleIndex % 8;
                    switch(Direction)
                    {
                        case 0:
                        {
                            SpawnCoefficientX = -1.0f;
                            SpawnCoefficientY = 1.0f;
                            Velocity += V2(-0.5f, 0);
                        } break;
                        
                        case 1:
                        {
                            SpawnCoefficientX = -1.0f;
                            SpawnCoefficientY = -1.0f;
                            Velocity += V2(-0.35f, 0.35f);
                        } break;
                        
                        case 2:
                        {
                            SpawnCoefficientX = 1.0f;
                            SpawnCoefficientY = -1.0f;
                            Velocity += V2(0, 0.5f);
                        } break;
                        
                        case 3:
                        {
                            SpawnCoefficientX = 1.0f;
                            SpawnCoefficientY = 1.0f;
                            Velocity += V2(0.35f, 0.35f);
                        } break;
                        
                        case 4:
                        {
                            SpawnCoefficientX = -1.0f;
                            SpawnCoefficientY = 1.0f;
                            Velocity += V2(0.5, 0.0f);
                        } break;
                        
                        case 5:
                        {
                            SpawnCoefficientX = -1.0f;
                            SpawnCoefficientY = -1.0f;
                            Velocity += V2(0.35f, -0.35f);
                        } break;
                        
                        case 6:
                        {
                            SpawnCoefficientX = 1.0f;
                            SpawnCoefficientY = -1.0f;
                            Velocity += V2(0, -0.5f);
                        } break;
                        
                        case 7:
                        {
                            SpawnCoefficientX = 1.0f;
                            SpawnCoefficientY = 1.0f;
                            Velocity += V2(-0.35f, -0.35f);
                        } break;
                    }
                    
                    r32 Angle = ParticleIndex*Emitter->SpiralCount*2.0f*Pi32/(r32)Emitter->ParticleCount;
                    r32 TileX = SpawnCoefficientX*(ParticleTileOffset + Emitter->CenterOffset)*Cos(Angle);
                    r32 TileY = SpawnCoefficientY*(ParticleTileOffset + Emitter->CenterOffset)*Sin(Angle);
                    Particle->OffsetP = V2((TileX) + Emitter->OffsetVariance*RandomBilateral(Series), (TileY) + Emitter->OffsetVariance*RandomBilateral(Series));
                    
                    Particle->Velocity = V2(Velocity.x + RandomBilateral(Series)*Emitter->VelocityVariance, Velocity.y + RandomBilateral(Series)*Emitter->VelocityVariance);
                    Particle->Alive = true;
                    Particle->MaxLifetime = Emitter->BaseLifetime + Emitter->LifetimeVariance*RandomUnilateral(Series);
                    Particle->Lifetime = Particle->MaxLifetime;
                }
            }
            else
            {
                Particle->Lifetime -= dt;
            }
        }
    }
}