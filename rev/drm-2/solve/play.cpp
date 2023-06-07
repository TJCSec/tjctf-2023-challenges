#include <iostream>
#include <vector>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/kd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>    // gives malloc
#include <math.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <unistd.h>

unsigned int sample_rate = 88100;
int blocksize = 88100/2;

ALCdevice  * openal_output_device;
ALCcontext * openal_output_context;

ALuint internal_buffer;
ALuint streaming_source[1];

int al_check_error(const char * given_label) {

    ALenum al_error;
    al_error = alGetError();

    if(AL_NO_ERROR != al_error) {

        printf("ERROR - %s  (%s)\n", alGetString(al_error), given_label);
        return al_error;
    }
    return 0;
}

void MM_init_al() {

    const char * defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    openal_output_device  = alcOpenDevice(defname);
    openal_output_context = alcCreateContext(openal_output_device, NULL);
    alcMakeContextCurrent(openal_output_context);

    // setup buffer and source

    alGenBuffers(1, & internal_buffer);
    al_check_error("failed call to alGenBuffers");
}

void MM_exit_al() {

    ALenum errorCode = 0;

    // Stop the sources
    alSourceStopv(1, & streaming_source[0]);        //      streaming_source
    int ii;
    for (ii = 0; ii < 1; ++ii) {
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

void play_buffer(short* buff, int n)
{
	//reset, allow buffer changes
    alSourcei(streaming_source[0], AL_BUFFER, NULL);
    
    alBufferData( internal_buffer, AL_FORMAT_MONO16, buff, n*sizeof(short), sample_rate);
    al_check_error("populating alBufferData");    

    /* Set-up sound source and play buffer */
    // ALuint src = 0;
    // alGenSources(1, &src);
    // alSourcei(src, AL_BUFFER, internal_buffer);
    alGenSources(1, & streaming_source[0]);
    alSourcei(streaming_source[0], AL_BUFFER, internal_buffer);
    // alSourcePlay(src);
    alSourcePlay(streaming_source[0]);
    
    ALenum current_playing_state;
    alGetSourcei(streaming_source[0], AL_SOURCE_STATE, & current_playing_state);
    al_check_error("alGetSourcei AL_SOURCE_STATE");
	
    while (AL_PLAYING == current_playing_state) {

        //printf("still playing ... so sleep\n");

        //sleep(1);   // should use a thread sleep NOT sleep() for a more responsive finish

        alGetSourcei(streaming_source[0], AL_SOURCE_STATE, & current_playing_state);
        al_check_error("alGetSourcei AL_SOURCE_STATE");
    }
    
    
}

void MM_render_one_buffer() {

    /* Fill buffer with Sine-Wave */
    // float freq = 440.f;
    float freq = 500.f;
    float incr_freq = 0.5f;

    int seconds = 7;
    // unsigned sample_rate = 22050;

    double my_pi = 3.14159;
    size_t buf_size = seconds * sample_rate;

   
    
    std::ifstream file("song.dat", std::ios::binary | std::ios::ate);
    
    if (!file.is_open())
    {
    	std::cout<<"Cannot open song.dat\n";
    	return;
    }
    std::streamsize size = file.tellg();
     // allocate PCM audio buffer        
    std::vector<short> samples = std::vector<short>(size/2); 
    file.seekg(0, std::ios::beg);
    file.read((char*)&samples[0], size);
    

    /* upload buffer to OpenAL */
    for (int i = 0; i < samples.size(); i+=blocksize)
    {
    	//decrypt
    	play_buffer((short*)&samples[i],blocksize);
    }
        	
    
    //play_buffer(&samples[0],samples.size());




    printf("end of playing\n");

    /* Dealloc OpenAL */
    MM_exit_al();

}

int main()
{
	std::cout<<"Hello world!!\n";
	MM_init_al();
    	MM_render_one_buffer();
    	//test chacha20
    	
    	
    	
    	return 0;
}
