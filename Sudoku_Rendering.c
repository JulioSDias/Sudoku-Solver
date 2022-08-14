void ColorScreen(int Color);
void DrawRectangle(int x0, int y0, int width, int height, int Color);
void DrawNumber(int x0, int y0, int color1, int color2, int TileMap[8][8]);

void ColorScreen(int Color){
    int *pixel = RenderBuffer.Pixels;
    for(int i = 0; i < RenderBuffer.Height; i++){
        for(int j = 0; j < RenderBuffer.Width; j++){
            *pixel = Color;
            pixel++;
        }
    }
}

void DrawRectangle(int x0, int y0, int width, int height, int Color){
    x0 = Clamp(x0, 0, RenderBuffer.Width);
    y0 = Clamp(y0, 0, RenderBuffer.Height);
    
    for(int i = y0; i < y0 + height; i++){
        int *pixel = RenderBuffer.Pixels + x0 + RenderBuffer.Width * i;
        for(int j = x0; j < x0 + width; j++){
            *pixel = Color;
            pixel++;
        }
    }
}

void DrawNumber(int x0, int y0, int color1, int color2, int TileMap[8][8]){
    
    int size = 4;
    
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(TileMap[i][j] == 0)
                DrawRectangle(x0 + j * size, y0 + i * size, size, size, color1);
            if(TileMap[i][j] == 1)
                DrawRectangle(x0 + j * size, y0 + i * size, size, size, color2);
        }
    }
    
}

