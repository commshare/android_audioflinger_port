#include <stdio.h>

#include <utils/Log.h>

#include <signal.h>
#include <execinfo.h>
#include <media/SoundPool.h>
#include <system/audio.h>
#include <media/AudioSystem.h>

using namespace android;


static void signal_handler(int sig)
{
    int status = 0;
    bool do_exit = false;
    printf("signal %d received\n", sig);
    switch(sig) {
        case SIGTERM:
        case SIGINT:
        case SIGKILL:
            status = 1;
            do_exit = true;
        case SIGABRT:
        case SIGBUS:
        case SIGFPE:
        case SIGILL:
        case SIGSEGV:{
            void *array[10];
            int i = 0, size = 0;
            char **messages = (char **)NULL;

            size = backtrace(array, 10);
            printf("stack trace:\n");
            messages = backtrace_symbols(array, size);
            for (i=0; i<size; i++)
            	printf("%s\n", messages[i]);

            status = 1;
            do_exit = true;
            break;
        }
        case SIGHUP:
        default:
        	printf("signal not supported.\n");
            break;
    }

    if (do_exit != 0){
        exit(status);
    }
}

void test_asm(int32_t increment, volatile int32_t *ptr)
{
    __asm__ __volatile__ ("lock; xaddl %0, %1"
                          : "+r" (increment), "+m" (*ptr)
                          : : "memory");
}

int main (int argc, char **argv)
{
	printf("start to test binder!!!\n");

	if (argc<2) {
		printf("test demo need parameter file name\n");
	}
	std::string file_name = std::string(argv[1]);
	printf("input file name %s\n", file_name.c_str());
    signal(SIGHUP, signal_handler); /* catch hangup signal */
    signal(SIGTERM, signal_handler); /* catch kill signal */
    signal(SIGKILL, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGBUS, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGILL, signal_handler);
    signal(SIGSEGV, signal_handler);

    SoundPool* pool = new SoundPool(1, AUDIO_STREAM_MUSIC, 100);
    if (pool == NULL) {
        printf("sound pool is null.\n");
        return 1;
    }
       
    int id = pool->load(file_name.c_str(), 1);
    sleep(1);
    pool->play(id, 0,0,1,0,44100);//int sampleID, float leftVolume, float rightVolume, int priority, int loop, float rate
    while(1) {
    	sleep(3);
    }

	return 0;
}
