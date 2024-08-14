/*
 *CmdMain.c : Defines the entry point for the console application.
 */


#include <signal.h>
#include <pthread.h>

#include "CmdTypes.h"
#include "CmdTask.h"


#ifndef WIN32
    pthread_t stPThreadSerial;
#endif // WIN32

struct termios g_stTermios;

void err_quit(int fd, char *msg, struct termios *flags)
{
    fprintf(stderr, "%s\n", msg);
    tcsetattr(fd, TCSANOW, flags);
    exit(EXIT_FAILURE);
}

void SetAttr(int fd)
{
    struct termios new_flags, old_flags;
    char *msg = "Failed to change attributes!";

    tcgetattr(fd, &old_flags);

    /*save console attr*/
    g_stTermios = old_flags;
    new_flags = old_flags;
    memset(&new_flags.c_cc, 0x00, sizeof(new_flags.c_cc));
    new_flags.c_lflag &= ~(ECHO | ECHOE | ICANON | ISIG);
    new_flags.c_iflag &= ~(BRKINT | ICRNL);
    new_flags.c_cc[VTIME] = 0;
    new_flags.c_cc[VMIN] = 1;
     
    if(tcsetattr(fd, TCSAFLUSH, &new_flags) < 0)
    {
        err_quit(fd, msg, &old_flags);
    }
}

int main(int argc, char *argv[])
{
    int iFd;
    ULONG timetick;
        
    timetick = 1000;

    CMD_Init(CLI_MAX_TELNET + 1);

    #if 1
    iFd = 1;
    fcntl(iFd, F_SETFL, 0);
    SetAttr(iFd);
    #endif

#ifndef WIN32
    pthread_create(&stPThreadSerial, NULL, (void*)CMD_TimerEntry, (void *)&timetick);
#endif
    
    
    CMD_Entry(CLI_CONSOLE_ID, iFd, iFd);

    return 0;
}

