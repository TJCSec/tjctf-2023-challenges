#include <emscripten.h>
#include <SDL.h>
#include <iostream>

#define MAX_NAME_LENGTH 8
#define NUM_LAYERS 3

using namespace std;

const int width = 32;
const int height = 32;
const int channels = 4;

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Surface *surface;

struct PixelData
{
    Uint8 pixels[width * height * channels];
    char name[MAX_NAME_LENGTH];
    Uint8 layers[NUM_LAYERS][width * height * channels];
    uint16_t n = width * height * channels;
};

PixelData pixelData;

int layer;
Uint8 color[3];
bool leftMouseButtonDown = false;

extern "C" char *EMSCRIPTEN_KEEPALIVE getName()
{
    return pixelData.name;
}

// return layers
extern "C" Uint8 *EMSCRIPTEN_KEEPALIVE getLayer(int l)
{
    return pixelData.layers[l];
}

extern "C" void EMSCRIPTEN_KEEPALIVE setName(char *newName, int n)
{
    n = n >= MAX_NAME_LENGTH ? MAX_NAME_LENGTH - 1 : n;
    for (int i = 0; i < n; i++)
    {
        pixelData.name[i] = newName[i];
    }

    pixelData.name[n] = '\0';

    free(newName);
}

extern "C" void EMSCRIPTEN_KEEPALIVE copyCanvas(Uint8 *data, int n)
{
    memcpy(pixelData.layers, data, n);
    free(data);
}

extern "C" void EMSCRIPTEN_KEEPALIVE setColor(int r, int g, int b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

extern "C" void EMSCRIPTEN_KEEPALIVE setLayer(int l)
{
    layer = l;
}

extern "C" void EMSCRIPTEN_KEEPALIVE toggleLeftMouseButton(int state)
{
    leftMouseButtonDown = state;
}

extern "C" void EMSCRIPTEN_KEEPALIVE draw(int mouseX, int mouseY)
{
    if (mouseX < 0 || mouseX >= width || mouseY < 0 || mouseY >= height)
        return;

    if (leftMouseButtonDown)
    {
        pixelData.layers[layer][4 * (mouseY * width + mouseX)] = color[2];
        pixelData.layers[layer][4 * (mouseY * width + mouseX) + 1] = color[1];
        pixelData.layers[layer][4 * (mouseY * width + mouseX) + 2] = color[0];
        pixelData.layers[layer][4 * (mouseY * width + mouseX) + 3] = 0;
    }
}

extern "C" void EMSCRIPTEN_KEEPALIVE clearCanvas()
{
    memset(pixelData.pixels, 255, width * height * channels);
    memset(pixelData.layers, 255, width * height * channels * NUM_LAYERS);
}

extern "C" void EMSCRIPTEN_KEEPALIVE loop()
{
    if (SDL_MUSTLOCK(surface))
        SDL_LockSurface(surface);

    Uint8 *sPixels = (Uint8 *)surface->pixels;

    for (int i = 0; i < pixelData.n; i += 4)
    {
        int topLayer = 0;

        for (int j = 0; j < NUM_LAYERS; j++)
        {
            if (pixelData.layers[j][i + 3] == 0)
            {
                topLayer = j;
                break;
            }
        }

        pixelData.pixels[i] = pixelData.layers[topLayer][i];
        pixelData.pixels[i + 1] = pixelData.layers[topLayer][i + 1];
        pixelData.pixels[i + 2] = pixelData.layers[topLayer][i + 2];
        pixelData.pixels[i + 3] = 255 - pixelData.layers[topLayer][i + 3];
    }

    emscripten_run_script("setName()");

    memcpy(sPixels, pixelData.pixels, width * height * channels);

    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    emscripten_set_main_loop(loop, 0, true);
}
