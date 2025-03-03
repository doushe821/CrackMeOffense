// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// C++ 
#include <thread>
#include <vector>
#include <array>
#include <string>
#include <chrono>


const int NUMBER_OF_FRAMES = 8;
const int FRAME_NUMBER_CHARACHTER = 18;
const int spriteX = 320;
const int spriteY = 320;
const int windowX = 800;
const int windowY = 800;

enum modes
{
    mainInc = 0,
    mainDec = 1,
    sideBot = 2,
    sideTop = 3,
};

const size_t JE_IP = 0x3A;

enum errors
{
    CANNOT_OPEN_FILE = 1,
    CANNOT_CREATE_FILE = 2
};

char* FileToString(FILE* out);
size_t GetFileSize(FILE* fp);
void* Crack(void* arg);
void* Visualizer(void* arg);

int main()
{
    pthread_t threads[2];

    pthread_create(&threads[0], NULL, Crack, NULL);
    pthread_create(&threads[1], NULL, Visualizer, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;

}

void* Crack(void* arg)
{
    FILE* fpToCrack = fopen("PASSWORD.COM", "r+b");
    if(fpToCrack == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }

    size_t initialSize = GetFileSize(fpToCrack);
    char* fileBuffer = FileToString(fpToCrack); // need to replace on 3a
    fclose(fpToCrack);

    fileBuffer[JE_IP] = 0x75;

    FILE* fpCracked = fopen("PASSWORD_CRACKED.COM", "w+b");
    if(fpCracked == NULL)
    {
        fprintf(stderr, "Failed to create new file\n");
        return NULL;
    }

    fprintf(stderr, "File length = %zu\n", initialSize);
    fwrite(fileBuffer, 1, initialSize, fpCracked);
    fclose(fpCracked);
    free(fileBuffer);

    return NULL;
}

char* FileToString(FILE* out)
{
    if(out == NULL)
    {
        return NULL;
    }

    size_t size = GetFileSize(out);
    char* string = (char*)calloc(size + 1, sizeof(char));
    if(string == NULL)
    {
        return NULL;
    }
    string[size] = '\0';
    if(fread(string, sizeof(char), size, out) != size)
    {
        return NULL;
    }
    return string;
}

size_t GetFileSize(FILE* fp)
{
    
    fseek(fp, 0L, SEEK_END);
    
    size_t size = (size_t)ftell(fp);
    
    rewind(fp);
    return size;
}


void* Visualizer(void* arg)
{
    int MS_BETWEEN_FRAMES = 200;
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "You got KeyGen'ed!!");

    sf::Music audio;
    if(!audio.openFromFile("Music/NightOfNights8bit.mp3"))
    {
        return NULL;
    }
    audio.play();
    system("pause");

    std::array<sf::Texture, 8> textures {};
    std::array<sf::Sprite, 8> sprites {};
    std::string FrameFileName = "Animation/hinaspin1.png"; 
    for(int i = 0; i < NUMBER_OF_FRAMES; i++)
    {
        textures[i].loadFromFile(FrameFileName);
        sprites[i].setTexture(textures[i]);
        FrameFileName[FRAME_NUMBER_CHARACHTER]++;
    }

    int introCounter = 0;
    int animationCounter = 0;
    int currentX = 100;
    int currentY = 0;
    int velocity = 13;
    int mode = 0; 
    int mainTangent = windowX / windowY;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(introCounter == 40)
        {
            MS_BETWEEN_FRAMES /= 5;
        }
        if(introCounter == 32)
        {
            MS_BETWEEN_FRAMES *= 2;
        }

        window.clear();
        sprites[animationCounter % 8].setPosition(currentX, currentY);
        window.draw(sprites[animationCounter % 8]);
        animationCounter++;
        introCounter+=1;
        std::this_thread::sleep_for(std::chrono::milliseconds(MS_BETWEEN_FRAMES));

        if(windowX - currentX <= spriteX)
        {
            mode = mainDec;
        }
        else if(currentX <= 0)
        {
            mode = mainInc;
        }
        else if(windowY - currentY <= spriteY)
        {
            mode = sideBot;
        }
        else if(currentY <= 0)
        {
            mode = sideTop;
        }

        switch(mode)
        {
            case mainInc:
            {
                currentX += velocity / mainTangent;
                currentY += velocity;
                break;
            }
            case mainDec:
            {
                currentX -= velocity / mainTangent;
                currentY -= velocity;
                break;
            }
            case sideBot:
            {
                currentX += velocity / mainTangent;
                currentY -= velocity;   
                break;  
            }
            case sideTop:
            {
                currentX -= velocity / mainTangent;
                currentY += velocity;
                break;
            }
            default:
            {
                fprintf(stderr, "An error appeared, try relaunching program!\n");
                abort();
            }
        } 


        window.display();
    }

    return 0;
}