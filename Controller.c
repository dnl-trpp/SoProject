#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

int serialport_init(const char* serialport, int baud);
int serialport_writebyte(int fd, uint8_t b);
int serialport_write(int fd, const char* str);
int serialport_read_until(int fd, char* buf, char until);
int serialport_readbyte(int fd, uint8_t* b);


int main(int argc, char *argv[]) 
{ 
    int fd = 0;
    int baudrate = B19200;
    char buf[100], dat[20], command[20];
    int rc,n;

    printf("Connecting to /dev/ttyACM0...");
    fflush(stdout);
    fd = serialport_init("/dev/ttyACM0", baudrate);
    if(fd==-1) return -1;

    sleep(1);
    printf("[SUCCESS]\n");

    while(1) {

        printf("> ");
        fgets(command,20,stdin);
        if(strncmp(command,"help",4)==0){
            printf("Usage:\n");
            printf("-- set [addr] [value]\n");
            printf("-- get [addr]\n");
            printf("-- apply\n");
            printf("-- sample\n");

        }

        //Handle SET
        else if(strncmp(command,"set",3)==0){
            int addr=0;
            int value=-1;
            sscanf(command,"set %d %i" ,&addr, &value);
            if(addr<=0 || addr>127 || value==-1 || value> 255){
                printf("Invalid format. \nUsage:\n");
                printf("set [addr] [value]\n");
                
            }
            else{
 
                if(serialport_writebyte(fd,'s')==-1) {
                    perror("Can't write byte to uart\n");
                    return -1;
                }
                if(serialport_writebyte(fd,addr)==-1) {
                    perror("Can't write byte to uart\n");
                    return -1;
                }
                if(serialport_writebyte(fd,value)==-1) {
                    perror("Can't write byte to uart\n");
                    return -1;
                }
                uint8_t r;
                if(serialport_readbyte(fd,&r)==-1){
                    perror("Can't read byte from uart\n");
                    return -1;
                }
                if(r==0xff){
                    printf("Command sent sucesfully\n");
                }
                else{
                    printf("Master had troubles sending this command\n");
                }
            }
        }

        //Handle SAMPLE
        else if(strncmp(command,"sample",6)==0){

            if(serialport_writebyte(fd,'m')==-1) {
                perror("Can't write byte to uart\n");
                return -1;
            }
            uint8_t r;
            if(serialport_readbyte(fd,&r)==-1){
                perror("Can't read byte from uart\n");
                return -1;
            }
            if(r==0xff){
                printf("Command sent sucesfully\n");
            }
            else{
                printf("Master had troubles sending this command\n");
            }

        }

        //Handle GET
        else if(strncmp(command,"get",3)==0){
            int addr=0;
            sscanf(command,"get %d" ,&addr);
            if(addr<=0 || addr>127){
                printf("Invalid format. \nUsage:\n");
                printf("get [addr]\n");
            }
            else{
 
                if(serialport_writebyte(fd,'g')==-1) {
                    perror("Can't write byte to uart\n");
                    return -1;
                }
                if(serialport_writebyte(fd,addr)==-1) {
                    perror("Can't write byte to uart\n");
                    return -1;
                }
                uint8_t r;
                if(serialport_readbyte(fd,&r)==-1){
                    perror("Can't read byte from uart\n");
                    return -1;
                }
                if(r==0xff){
                    printf("Command sent sucesfully\n");
                    if(serialport_readbyte(fd,&r)==-1){
                        perror("Can't read byte from uart\n");
                        return -1;
                    }
                    printf("Master read %x\n",r);
                }
                else{
                    printf("Master had troubles sending this command\n");
                }
            }
        }

        //Handle APPLY
        else if(strncmp(command,"apply",5)==0){
            if(serialport_writebyte(fd,'a')==-1) {
                perror("Can't write byte to uart\n");
                return -1;
            }
            uint8_t r;
            if(serialport_readbyte(fd,&r)==-1){
                perror("Can't read byte from uart\n");
                return -1;
            };
            if(r==0xff){
                printf("Command sent sucesfully\n");
            }
            else{
                printf("Master had troubles sending this command\n");
            }

        }
        else{
            printf("Unknown command, type help for usage\n");
        }

       /* strcpy(dat, "0\0");
        if(command[0] == 's') {
            dat[0]='s';
        }
        rc = serialport_write(fd, dat);
        if(rc==-1) return -1;
        //printf("Waiting until UART buffer clears: %d\n", tcdrain(fd));
        n = serialport_read_until(fd, buf, '\n');
        printf("wrote %d bytes, read %d bytes: %s\n", rc, n, buf);*/
    }


    close(fd);
    exit(EXIT_SUCCESS);    
} 

    

int serialport_writebyte( int fd, uint8_t b)

{
    int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;
}

int serialport_readbyte(int fd, uint8_t* b)

{
    int n=-1;
    do { 
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        
    } while( n>0 );
    return 1;
}


int serialport_write(int fd, const char* str)
{
    int len = strlen(str);
    int n = write(fd, str, len);
    if( n!=len ) 
        return -1;
    return n;
}



int serialport_read_until(int fd, char* buf, char until)
{
    char b[1];
    int i=0;
    do { 
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0]; i++;
    } while( b[0] != until );

    buf[i] = 0;  // null terminate the string
    return i;
}

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);
    //fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    fd = open(serialport, O_RDWR | O_NOCTTY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
// if you want these speeds, uncomment these and set #defines if Linux
//#ifndef OSNAME_LINUX
//    case 14400:  brate=B14400;  break;
//#endif
    case 19200:  brate=B19200;  break;
//#ifndef OSNAME_LINUX
//    case 28800:  brate=B28800;  break;
//#endif
    //case 28800:  brate=B28800;  break;
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
    // 8N1
    toptions.c_cflag &= ~PARENB; //No parity
    toptions.c_cflag &= ~CSTOPB; //One Stop
    toptions.c_cflag &= ~CSIZE; //Disable Charsize mask
    toptions.c_cflag |= CS8; //8 data bits
    // no flow control
    toptions.c_cflag &= ~CRTSCTS; //Disable hardawre flow control
    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off software flow ctrl
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }
    return fd;

}