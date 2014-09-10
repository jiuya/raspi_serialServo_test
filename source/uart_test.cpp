#include <stdio.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include <termios.h>

int main(){
	/* シリアルポートオープン */
	int fd = serialOpen("/dev/ttyAMA0",115200);
	if(fd<0){
		printf("can not open serialport");
	}
	struct termios options ;

	tcgetattr (fd, &options) ;   // Read current options
	options.c_cflag &= ~CSIZE ;  // Mask out size
	options.c_cflag |= CS8 ;
	options.c_cflag |= PARENB ;  // Enable Parity - even by default
	tcsetattr (fd,TCSANOW, &options) ;   // Set new options
	
	delay(1000);
	long servo = 3497;
	int rol = 1;
	while(1){
		/* 受信処理 */
		while(serialDataAvail(fd)){
			printf("Receive:0x%x\n" , serialGetchar(fd) );
			fflush(stdout);
		}

		/* 送信処理 */
		//serialPuts(fd,"hello world\n");
		if(rol == 1)
		{
			if(servo < 11000)
			{
				servo += 500;
			}
			else
			{
				rol = 0;
				servo = 11000;
			}
		}
		else
		{
			if(servo > 3497)
			{
				servo -= 500;
			}
			else
			{
				rol = 1;
				servo = 3497;
			}
		}
		serialPutchar(fd,0x81);
		serialPutchar(fd,(servo >> 7) & 0x7f);
		serialPutchar(fd,servo & 0x7f);
		/*
		serialPutchar(fd,0xff);
		serialPutchar(fd,0);
		serialPutchar(fd,0);
		serialPutchar(fd,0);
		*/
		delay(1000);
	}

	return 0;
}

