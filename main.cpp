#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const std::string poker[] =
{
    "poker_pic/Ac.png", "poker_pic/2c.png", "poker_pic/3c.png", "poker_pic/4c.png", "poker_pic/5c.png",
    "poker_pic/6c.png", "poker_pic/7c.png", "poker_pic/8c.png", "poker_pic/9c.png",
    "poker_pic/10c.png", "poker_pic/Jc.png", "poker_pic/Qc.png", "poker_pic/Kc.png",
    "poker_pic/Ad.png", "poker_pic/2d.png", "poker_pic/3d.png", "poker_pic/4d.png", "poker_pic/5d.png",
    "poker_pic/6d.png", "poker_pic/7d.png", "poker_pic/8d.png", "poker_pic/9d.png",
    "poker_pic/10d.png", "poker_pic/Jd.png", "poker_pic/Qd.png", "poker_pic/Kd.png",
    "poker_pic/Ah.png", "poker_pic/2h.png", "poker_pic/3h.png", "poker_pic/4h.png", "poker_pic/5h.png",
    "poker_pic/6h.png", "poker_pic/7h.png", "poker_pic/8h.png", "poker_pic/9h.png",
    "poker_pic/10h.png", "poker_pic/Jh.png", "poker_pic/Qh.png", "poker_pic/Kh.png",
    "poker_pic/As.png", "poker_pic/2s.png", "poker_pic/3s.png", "poker_pic/4s.png", "poker_pic/5s.png",
    "poker_pic/6s.png", "poker_pic/7s.png", "poker_pic/8s.png", "poker_pic/9s.png",
    "poker_pic/10s.png", "poker_pic/Js.png", "poker_pic/Qs.png", "poker_pic/Ks.png"
};
//参与游戏人数
const int PERSON_NUM = 4;

//每个人所得牌数
const int EACH_NUM = 13;

//总的牌数
const int TOTAL_NUM = 52;

//窗口的宽，高
const int SCREEN_WIDTH = 1040;
const int SCREEN_HEIGHT = 800;

//渲染图片的类
class LTexture
{
public:
    //Initializes variables
    LTexture();
    
    //Deallocates memory
    ~LTexture();
    
    //Loads image at specified path
    bool loadFromFile( std::string path );
    
    //Deallocates texture
    void free();
    
    //Renders texture at given point
    void render( int x, int y );
    
    //Gets image dimensions
    int getWidth();
    int getHeight();
    void setWidth(int w);
    void setHeight(int h);
    
private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    
    //Image dimensions
    int mWidth;
    int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//渲染一手牌
bool renderEachPoker(LTexture eachPoker[]);

//渲染整副牌
void renderAllPoker(LTexture allPoker[]);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//渲染扑克正面的对象
LTexture pokerTexture[TOTAL_NUM];

//扑克的背面
LTexture pokerBack[TOTAL_NUM];

//背景
LTexture backTexture;
//发牌
bool dealPoker();

int GroupPoker[PERSON_NUM][EACH_NUM];

bool dealPoker()
{
    bool inhand[TOTAL_NUM] = {false};
    int i = 0;
    int j = 0;
    int k = 0;
    
    while(i < TOTAL_NUM)
        {
        for( ; j < PERSON_NUM; j++)
            {
            k = 0;
            while(k < EACH_NUM)
                {
                int temp_num = rand() % TOTAL_NUM;
                
                if(!inhand[temp_num])
                    {
                    inhand[temp_num] = true;
                    GroupPoker[j][k] = temp_num;
                    i++;
                    k++;
                    }
                }
            }
        
        }
    
    return true;
}

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
        {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        }
    else
        {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
            {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
        else
            {
            //Get image dimensions
            mWidth = loadedSurface->w / 2;
            mHeight = loadedSurface->h / 2;
            }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
        }
    
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
        {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
        }
}

void LTexture::render( int x, int y )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

void LTexture::setWidth(int w)
{
    mWidth = w;
}
void LTexture::setHeight(int h)
{
    mHeight = h;
}
bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
        }
    else
        {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            {
            printf( "Warning: Linear texture filtering not enabled!" );
            }
        
        //Create window
        gWindow = SDL_CreateWindow( "Poke Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
            {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
            }
        else
            {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
                {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
                }
            else
                {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                    {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                    }
                }
            }
        }
    
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //加载背景图片
    if(!backTexture.loadFromFile("render_pic/background.png"))
        {
        printf("Fail to load background image");
        success = false;
        }
    
    //加载扑克的背面图
    for(int i = 0; i < TOTAL_NUM; i++)
        {
        if(!pokerBack[i].loadFromFile("poker_back/poke_back.jpeg"))
            {
            printf("Fail to load the back of poker");
            success = false;
            }
        }
    
    int total = 0;
    for(int i = 0; i < PERSON_NUM; i++)
        {
        for(int j = 0; j < EACH_NUM; j++)
            {
            if(!pokerTexture[total++].loadFromFile(poker[GroupPoker[i][j]]))
                {
                printf("Fail to load picture");
                success = false;
                }
            }
        }
    return success;
}

//将整副牌一次性发完
void renderAllPoker(LTexture allPoker[])
{
    int x0 = 400, y0 = 10;
    int x1 = 700, y1 = 130;
    int x2 = 400, y2 = 580;
    int x3 = 280, y3 = 130;
    
    for(int i = 0; i < TOTAL_NUM; i++)
        {
        switch(i / 13)
            {
                case 0:
                allPoker[i].render(x0, y0);
                x0 += 15;
                break;
                case 1:
                allPoker[i].render(x1, y1);
                y1 += 30;
                break;
                case 2:
                allPoker[i].render(x2, y2);
                x2 += 15;
                break;
                case 3:
                allPoker[i].render(x3, y3);
                y3 += 30;
                break;
            }
        
        }
}

//根据需要定向发牌
bool renderEachPoker(LTexture eachPoker[], int begin)
{
    int x = 0,y = 0;
    bool success = true;
    
    if(begin == 0)
        {
        x = 400;
        y = 580;
        }
    else if(begin == 13)
        {
        x = 280;
        y = 130;
        }
    else if(begin == 26)
        {
        x = 400;
        y = 10;
        }
    else if(begin == 39)
        {
        x = 700;
        y = 130;
        }
    
    for(int i = begin; i < begin + 13; i ++)
        {
        eachPoker[i].render(x, y);
        if(begin == 0)
            x += 15;
        else if(begin == 13)
            y += 30;
        else if(begin == 26)
            x += 15;
        else if(begin == 39)
            y += 30;
        }
    
    return success;
}

void close()
{
    //free the background picture
    backTexture.free();
    
    //Free the poker images
    for(int i = 0; i < TOTAL_NUM; i++)
        {
         pokerTexture[i].free();
        }
    
    //free the back of poker
    for(int i = 0; i < TOTAL_NUM; i++)
        {
        pokerBack[i].free();
        }
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    
    srand((unsigned) time(NULL));
    
    //发牌
    dealPoker();
    
    //Start up SDL and create window
    if( !init() )
        {
        printf( "Failed to initialize!\n" );
        }
    else
        {
        //Load media
        if( !loadMedia() )
            {
            printf( "Failed to load media!\n" );
            }
        else
            {
            //Main loop flag
            bool quit = false;
            
            //Event handler
            SDL_Event e;
            
           //选择位置分别为上下左右
            int role = 0;
            
            //While application is running
            while( !quit )
                {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                    {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                        {
                        quit = true;
                        }
                    else if(e.type == SDL_KEYDOWN)
                        {
                        switch(e.key.keysym.sym)
                            {
                            case SDLK_DOWN:
                                role = 0;
                                break;
                            case SDLK_LEFT:
                                role = 13;
                                break;
                            case SDLK_UP:
                                role = 26;
                                break;
                            case SDLK_RIGHT:
                                role = 39;
                                break;
                            }
                        }
                    }
                
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x00 );
                SDL_RenderClear( gRenderer );

                //渲染背景
                backTexture.setHeight(SCREEN_HEIGHT * 2);
                backTexture.setWidth(SCREEN_WIDTH * 2);
                backTexture.render(0, 0);
                
                //渲染整副牌
                renderAllPoker(pokerBack);
                
                //渲染一手牌
                renderEachPoker(pokerTexture, role);

                //Update screen
                SDL_RenderPresent( gRenderer );
                
                }
            }
        }
    
    //Free resources and close SDL
    close();
    
    return 0;
}
