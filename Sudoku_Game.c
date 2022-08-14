void TileInfoSet(TILEINFO *TileInfo);
void TileUpdate(TILEINFO *TileInfo, MOUSE MouseDisplay, int Color1, int Color2);
void ChooseNumber(int x0, int y0, int Color1, int Color2, int Num);
void MouseDetect(MOUSE *MouseDisplay, int OffSet);
void LockTile(MOUSE MouseDisplay, TILEINFO *TileInfo);
int PossibleNum(int x, int y, TILEINFO *TileInfo, int Num);
int Solve(TILEINFO *TileInfo, int i, int j);
void Draw(TILEINFO *TileInfo, int OffSet, MOUSE MouseDisplay);

void TileInfoSet(TILEINFO *TileInfo){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            TileInfo[i * n + j].X = j;
            TileInfo[i * n + j].Y = i;
            TileInfo[i * n + j].Selected = 0;
            TileInfo[i * n + j].Number = 0;
        }
    }
}

void TileUpdate(TILEINFO *TileInfo, MOUSE MouseDisplay, int Color1, int Color2){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int k = i * n + j;
            if(TileInfo[k].Selected == 1) TileInfo[k].Color = RED;
            if(TileInfo[k].Selected == 0){
                if(i >= 3 && i <= 5 && j < 3) TileInfo[k].Color = Color2;
                else if(i >= 3 && i <= 5 && j > 5) TileInfo[k].Color = Color2;
                else if(j >= 3 && j <= 5 && i < 3) TileInfo[k].Color = Color2;
                else if(j >= 3 && j <= 5 && i > 5) TileInfo[k].Color = Color2;
                else TileInfo[k].Color = Color1;
                TileInfo[MouseDisplay.TileNum].Color = GREEN;
            }
        }
    }
    
    if(PRESS(BACKSPACE))TileInfo[MouseDisplay.TileNum].Number = 0;
    if(PRESS(ONE))TileInfo[MouseDisplay.TileNum].Number = 1;
    if(PRESS(TWO))TileInfo[MouseDisplay.TileNum].Number = 2;
    if(PRESS(THREE))TileInfo[MouseDisplay.TileNum].Number = 3;
    if(PRESS(FOUR))TileInfo[MouseDisplay.TileNum].Number = 4;
    if(PRESS(FIVE))TileInfo[MouseDisplay.TileNum].Number = 5;
    if(PRESS(SIX))TileInfo[MouseDisplay.TileNum].Number = 6;
    if(PRESS(SEVEN))TileInfo[MouseDisplay.TileNum].Number = 7;
    if(PRESS(EIGHT))TileInfo[MouseDisplay.TileNum].Number = 8;
    if(PRESS(NINE))TileInfo[MouseDisplay.TileNum].Number = n;
}



void ChooseNumber(int x0, int y0, int Color1, int Color2, int Num){
    switch(Num){
        case 0:
        DrawNumber(x0, y0, Color1, Color2, Num0);
        break;
        case 1:
        DrawNumber(x0, y0, Color1, Color2, Num1);
        break;
        case 2:
        DrawNumber(x0, y0, Color1, Color2, Num2);
        break;
        case 3:
        DrawNumber(x0, y0, Color1, Color2, Num3);
        break;
        case 4:
        DrawNumber(x0, y0, Color1, Color2, Num4);
        break;
        case 5:
        DrawNumber(x0, y0, Color1, Color2, Num5);
        break;
        case 6:
        DrawNumber(x0, y0, Color1, Color2, Num6);
        break;
        case 7:
        DrawNumber(x0, y0, Color1, Color2, Num7);
        break;
        case 8:
        DrawNumber(x0, y0, Color1, Color2, Num8);
        break;
        case 9:
        DrawNumber(x0, y0, Color1, Color2, Num9);
        break;
    }
}

void MouseDetect(MOUSE *MouseDisplay, int OffSet){
    input.Mouse.X = Clamp(input.Mouse.X, 0, RenderBuffer.Width - tile);
    input.Mouse.Y = Clamp(input.Mouse.Y, 0, RenderBuffer.Height - tile);
    
    MouseDisplay->X = input.Mouse.X/tile;
    MouseDisplay->Y = input.Mouse.Y/tile;
    MouseDisplay->TileNum = MouseDisplay->Y * n + MouseDisplay->X;
    
}


void LockTile(MOUSE MouseDisplay, TILEINFO *TileInfo){
    if(PRESS(SPACE)) 
        TileInfo[MouseDisplay.TileNum].Selected = 1;
    if(PRESS(SPACE) && PRESS(ALT))
        TileInfo[MouseDisplay.TileNum].Selected = 0;
}


int PossibleNum(int x, int y, TILEINFO *TileInfo, int Num){
    int x0 = (x/3) * 3;
    int y0 = (y/3) * 3;
    for(int i = 0; i < n; i++){
        if(TileInfo[i * n + x].Number == Num) return 0;
        if(TileInfo[y * n + i].Number == Num) return 0;
    }
    
    for(int i = y0; i < y0 + 3; i++){
        for(int j = x0; j < x0 + 3; j++){
            if(TileInfo[i * n + j].Number == Num) return 0;
        }
    }
    return 1;
}

int Solve(TILEINFO *TileInfo,int i, int j){
    if(i == n - 1 && j == n) return 1;
    
    if(j == n){
        i++;
        j = 0;
    }
    
    if(TileInfo[i * n + j].Number != 0) return Solve(TileInfo, i, j + 1);
    for(int num = 1; num < 10; num ++){
        if(PossibleNum(j, i, TileInfo, num) == 1){
            TileInfo[i * n + j].Number = num;
            if(Solve(TileInfo, i, j + 1) == 1) return 1;
            TileInfo[i * n + j].Number = 0;
        }
    }
    
    return 0;
}


void Draw(TILEINFO *TileInfo, int OffSet, MOUSE MouseDisplay){
    
    for(int i = 0; i < 81; i++){
        DrawRectangle(TileInfo[i].X * tile + OffSet, TileInfo[i].Y * tile + OffSet, tile - OffSet, tile - OffSet, TileInfo[i].Color);
        if(TileInfo[i].Number != 0)
            ChooseNumber(TileInfo[i].X * tile + OffSet + tile/4, TileInfo[i].Y * tile + OffSet + tile/4, TileInfo[i].Color, BLACK, TileInfo[i].Number);
    }
}
