#include <iostream>
#include <vector>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/kd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // gives malloc
#include <math.h>

#include "ChaCha20.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <unistd.h>

unsigned int sample_rate = 88100;
int blocksize = 88100 / 2;

ALCdevice *openal_output_device;
ALCcontext *openal_output_context;

ALuint internal_buffer;
ALuint streaming_source[1];

int al_check_error(const char *given_label)
{

    ALenum al_error;
    al_error = alGetError();

    if (AL_NO_ERROR != al_error)
    {

        printf("ERROR - %s  (%s)\n", alGetString(al_error), given_label);
        return al_error;
    }
    return 0;
}

void MM_init_al()
{

    const char *defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    openal_output_device = alcOpenDevice(defname);
    openal_output_context = alcCreateContext(openal_output_device, NULL);
    alcMakeContextCurrent(openal_output_context);

    // setup buffer and source

    alGenBuffers(1, &internal_buffer);
    al_check_error("failed call to alGenBuffers");
}

void MM_exit_al()
{

    ALenum errorCode = 0;

    // Stop the sources
    alSourceStopv(1, &streaming_source[0]); //      streaming_source
    int ii;
    for (ii = 0; ii < 1; ++ii)
    {
        alSourcei(streaming_source[ii], AL_BUFFER, 0);
    }
    // Clean-up
    alDeleteSources(1, &streaming_source[0]);
    alDeleteBuffers(16, &streaming_source[0]);
    errorCode = alGetError();
    alcMakeContextCurrent(NULL);
    errorCode = alGetError();
    alcDestroyContext(openal_output_context);
    alcCloseDevice(openal_output_device);
}

void play_buffer(short *buff, int n)
{
    // reset, allow buffer changes
    alSourcei(streaming_source[0], AL_BUFFER, NULL);

    alBufferData(internal_buffer, AL_FORMAT_MONO16, buff, n * sizeof(short), sample_rate);
    al_check_error("populating alBufferData");

    /* Set-up sound source and play buffer */
    // ALuint src = 0;
    // alGenSources(1, &src);
    // alSourcei(src, AL_BUFFER, internal_buffer);
    alGenSources(1, &streaming_source[0]);
    alSourcei(streaming_source[0], AL_BUFFER, internal_buffer);
    // alSourcePlay(src);
    alSourcePlay(streaming_source[0]);

    ALenum current_playing_state;
    alGetSourcei(streaming_source[0], AL_SOURCE_STATE, &current_playing_state);
    al_check_error("alGetSourcei AL_SOURCE_STATE");

    while (AL_PLAYING == current_playing_state)
    {

        // printf("still playing ... so sleep\n");

        // sleep(1);   // should use a thread sleep NOT sleep() for a more responsive finish

        alGetSourcei(streaming_source[0], AL_SOURCE_STATE, &current_playing_state);
        al_check_error("alGetSourcei AL_SOURCE_STATE");
    }
}

void MM_render_one_buffer()
{

    /* Fill buffer with Sine-Wave */
    // float freq = 440.f;
    float freq = 500.f;
    float incr_freq = 0.5f;

    int seconds = 7;
    // unsigned sample_rate = 22050;

    double my_pi = 3.14159;
    size_t buf_size = seconds * sample_rate;

    std::ifstream file("enc_song.dat", std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cout << "Cannot open enc_song.dat\n";
        return;
    }
    std::streamsize size = file.tellg();
    // allocate PCM audio buffer
    std::vector<short> samples = std::vector<short>(size / 2);
    file.seekg(0, std::ios::beg);
    file.read((char *)&samples[0], size);
    std::vector<int> order = {2422750, 4052600, 5726500, 9162400, 6827750, 7576600, 5330050, 3656150, 8633800, 8325450, 7048000, 7312300, 5902700, 572650, 881000, 11673250, 8413550, 8061150, 792900, 9294550, 4933600, 13347150, 10880350, 1013150, 8589750, 13126900, 3479950, 6783700, 1189350, 12686400, 9250500, 6122950, 2026300, 2863250, 1497700, 396450, 7224200, 1629850, 7180150, 9646950, 9823150, 10043400, 11761350, 9118350, 2334650, 3127550, 925050, 8721900, 9779100, 2158450, 9691000, 2246550, 6211050, 352400, 3083500, 8986200, 8677850, 11012500, 5550300, 6739650, 1541750, 5594350, 9470750, 2643000, 2819200, 5682450, 3700200, 2951350, 8810000, 13743600, 5065750, 1982250, 6915850, 9074300, 10660100, 9558850, 10527950, 13170950, 1277450, 8237350, 12598300, 1321500, 9911250, 4801450, 5109800, 8369500, 12730450, 10836300, 7532550, 2598950, 12818550, 6343200, 8281400, 10924400, 3568050, 1233400, 9955300, 4713350, 5638400, 13919800, 176200, 12334000, 4845500, 2554900, 616700, 11100600, 0, 484550, 4316900, 3391850, 8854050, 4889550, 6607500, 7796850, 2687050, 10792250, 1057200, 5153850, 5197900, 5241950, 5946750, 8193300, 3876400, 528600, 1365550, 5506250, 3039450, 11276800, 5418150, 11232750, 4140700, 5770550, 9867200, 11408950, 7929000, 5286000, 5990800, 2775150, 12201850, 12510200, 10968450, 2731100, 10087450, 6299150, 11629200, 10307700, 6431300, 11805400, 13479300, 8017100, 13567400, 3964500, 7136100, 11144650, 13391200, 2995400, 13303100, 3171600, 9514800, 10439850, 1894150, 660750, 88100, 3303750, 2510850, 6034850, 8457600, 3435900, 1717950, 2466800, 1453650, 9426700, 2070350, 7400400, 7664700, 1101250, 7752800, 12113750, 7620650, 12642350, 308350, 7973050, 6475350, 2114400, 8105200, 4757400, 5021700, 6871800, 1585800, 13875750, 12862600, 8545700, 12774500, 8501650, 13787650, 2202500, 4360950, 12422100, 3524000, 8765950, 11497050, 4669300, 13655500, 4096650, 9030250, 9382650, 4449050, 11893500, 6651550, 9206450, 11585150, 836950, 6695600, 5374100, 704800, 10748200, 969100, 11981600, 7268250, 5462200, 11320850, 2907300, 6563450, 11056550, 5814600, 4537150, 13611450, 9338600, 1806050, 4405000, 1673900, 440500, 10219600, 7488500, 12466150, 10351750, 10395800, 9999350, 10483900, 12069700, 10572000, 7708750, 3259700, 10704150, 3920450, 12994750, 3832350, 10616050, 13435250, 7356350, 12157800, 5858650, 12025650, 1938200, 11188700, 9735050, 1850100, 8942150, 13831700, 13259050, 11453000, 4184750, 7884950, 7444450, 11541100, 1409600, 11717300, 2290600, 3347800, 11849450, 3744250, 4625250, 11364900, 4581200, 6387250, 13699550, 44050, 132150, 4493100, 1145300, 10263650, 7003950, 12378050, 6078900, 748850, 4977650, 11937550, 12554250, 220250, 10131500, 4008550, 13082850, 264300, 12906650, 6255100, 3788300, 13038800, 6167000, 7840900, 9602900, 13215000, 8149250, 4272850, 12289950, 1762000, 6959900, 2378700, 13523350, 3612100, 4228800, 8898100, 12245900, 6519400, 7092050, 10175550, 12950700, 3215650};

    // printf("\nhere is freq %f\n", freq);
    // int i=0;
    // for(; i<buf_size; i++) {
    //     samples.push_back(32760 * sin( (2.f * my_pi * freq)/sample_rate * i ));
    // }

    // init encryption objects
    uint8_t key[32];

    std::ifstream keyfile("drm.key", std::ios::binary | std::ios::ate);
    if (!keyfile.is_open())
    {
        std::cout << "Could not open drm.key\n";
        return;
    }
    // allocate PCM audio buffer
    keyfile.seekg(0, std::ios::beg);
    keyfile.read((char *)&key[0], 32);

    // Really does not matter what this is, except that it is only used once.
    uint8_t nonce[8];

    std::ifstream noncefile("drm.nonce", std::ios::binary | std::ios::ate);
    if (!noncefile.is_open())
    {
        std::cout << "Could not open drm.nonce\n";
        return;
    }
    // allocate PCM audio buffer
    noncefile.seekg(0, std::ios::beg);
    noncefile.read((char *)&nonce[0], 8);

    Chacha20 chacha = Chacha20(key, nonce);

    /* upload buffer to OpenAL */
    uint8_t buffer[blocksize * 2];
    for (int i = 0; i < order.size(); i += 1)
    {
        // decrypt
        memcpy(buffer, &samples[order[i]], blocksize * 2);

        chacha.crypt(buffer, 2 * blocksize);

        if (buffer[0] != 0)
            play_buffer((short *)buffer, blocksize);
    }

    // play_buffer(&samples[0],samples.size());

    printf("end of playing\n");

    /* Dealloc OpenAL */
    MM_exit_al();
}

int main()
{
    MM_init_al();
    MM_render_one_buffer();
    // test chacha20

    return 0;
}
