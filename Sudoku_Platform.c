#include "Sudoku_Utils.c"
#include "Sudoku_Rendering.c"
#include "Sudoku_Game.c"

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int ShowCode);
void MainLoop(HWND Window, HDC DeviceContext, MSG Message);
LRESULT CALLBACK WindowMessageHandle(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);
void InputMessageHandling(MSG Message, HWND Window);

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int ShowCode){
    
    MSG Message;
    WINDOWSIZE WindowSize = {622, 645};
    /*Window Class*/
    WNDCLASSA WindowClass = {0};
    
    WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowMessageHandle;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "SudokuWindowClass";
    
    RegisterClass(&WindowClass);
    
    /*Create Window*/
    
    HWND Window = CreateWindowEx(0, WindowClass.lpszClassName, "Sudoku", WS_VISIBLE|WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, WindowSize.Width, WindowSize.Height, 0, 0,  Instance, 0);
    
    HDC DeviceContext = GetDC(Window);
    
    MainLoop(Window, DeviceContext, Message);
    
    return 0;
}

void MainLoop(HWND Window, HDC DeviceContext, MSG Message){
    
    TILEINFO TileInfo[81];
    int Color1 = WHITE;
    int Color2 = GREY;
    MOUSE MouseDisplay;
    int OffSet = 4;
    
    TileInfoSet(TileInfo);
    while(Running != 0){
        /*Input*/
        InputMessageHandling(Message, Window);
        
        /*Simulate Game*/
        ColorScreen(BLACK);
        MouseDetect(&MouseDisplay, OffSet);
        LockTile(MouseDisplay, TileInfo);
        TileUpdate(TileInfo, MouseDisplay, Color1, Color2);
        if(PRESS(ENTER))Solve(TileInfo, 0, 0);
        Draw(TileInfo, OffSet, MouseDisplay);
        
        /*Render*/
        StretchDIBits(DeviceContext, 0, 0, RenderBuffer.Width, RenderBuffer.Height, 0, 0, RenderBuffer.Width, RenderBuffer.Height, RenderBuffer.Pixels, &RenderBuffer.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
        Sleep(16);
    }
}


LRESULT CALLBACK WindowMessageHandle(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam){
    
    LRESULT Result = 0;
    
    switch(Message){
        case WM_SIZE:{
            /*Buffer Width and Height*/
            RECT ClientRect = {0};
            GetClientRect(Window, &ClientRect);
            
            RenderBuffer.Width = ClientRect.right - ClientRect.left;
            RenderBuffer.Height = ClientRect.bottom - ClientRect.top;
            
            /*Allocate the Buffer*/
            if(RenderBuffer.Pixels != 0){
                /*Free*/
                VirtualFree(RenderBuffer.Pixels, 0, MEM_RELEASE);
            }
            RenderBuffer.Pixels = VirtualAlloc(0, sizeof(int) * RenderBuffer.Width * RenderBuffer.Height, MEM_COMMIT, PAGE_READWRITE);
            
            /*Fill BitMap info*/
            
            RenderBuffer.BitMapInfo.bmiHeader.biSize = sizeof(RenderBuffer.BitMapInfo.bmiHeader);
            RenderBuffer.BitMapInfo.bmiHeader.biWidth = RenderBuffer.Width;
            RenderBuffer.BitMapInfo.bmiHeader.biHeight = -RenderBuffer.Height;
            RenderBuffer.BitMapInfo.bmiHeader.biPlanes = 1;
            RenderBuffer.BitMapInfo.bmiHeader.biBitCount = 32;
            RenderBuffer.BitMapInfo.bmiHeader.biCompression = BI_RGB;
        }break;
        
        case WM_CLOSE:{
            Running = 0;
        }break;
        
        case WM_DESTROY:{
            Running = 0;
        }break;
        
        default:{
            Result = DefWindowProc(Window, Message, wParam, lParam);
        }break;
    }
    
    return Result;
}


void InputMessageHandling(MSG Message, HWND Window){
    
    for(int i = 0; i < BUTTON_COUNT; i++){
        input.button[i].Changed = 0;
    }
    
    while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE)){
        switch(Message.message){
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:{
                int VKCode = (int)Message.wParam;
                int IsDown = ((Message.lParam & (1<<31)) == 0);
#define ProcessButton(vk, b) if(VKCode == vk) input.button[b].IsDown = IsDown; input.button[b].Changed = 1; 
                
                ProcessButton(0x30, ZERO);
                ProcessButton(0x31, ONE);
                ProcessButton(0x32, TWO);
                ProcessButton(0x33, THREE);
                ProcessButton(0x34, FOUR);
                ProcessButton(0x35, FIVE);
                ProcessButton(0x36, SIX);
                ProcessButton(0x37, SEVEN);
                ProcessButton(0x38, EIGHT);
                ProcessButton(0x39, NINE);
                
                ProcessButton(VK_MENU, ALT);
                ProcessButton(VK_SPACE, SPACE);
                ProcessButton(VK_RETURN, ENTER);
                ProcessButton(VK_BACK, BACKSPACE);
                ProcessButton(VK_DOWN, DOWN);
                ProcessButton(VK_UP, UP);
                ProcessButton(VK_LEFT, LEFT);
                ProcessButton(VK_RIGHT, RIGHT);
                ProcessButton(VK_RBUTTON, RBUTTON);
                ProcessButton(VK_LBUTTON, LBUTTON);
            }break;
            
            default:{
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            POINT MouseCoord;
            GetCursorPos(&MouseCoord);
            ScreenToClient(Window, &MouseCoord);
            input.Mouse.X = (int)MouseCoord.x;
            input.Mouse.Y = (int)MouseCoord.y;
        }
    }
}