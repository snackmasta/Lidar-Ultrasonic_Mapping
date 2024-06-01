//   mp3_play ();		//start play
//   mp3_play (5);	//play "mp3/0005.mp3"
//   mp3_next ();		//play next 
//   mp3_prev ();		//play previous
//   mp3_set_volume (uint16_t volume);	//0~30
//   mp3_set_EQ ();	//0~5
//   mp3_pause ();
//   mp3_stop ();
//  void mp3_get_state (); 	//send get state command
//   void mp3_get_volume (); 
//   void mp3_get_u_sum (); 
//  void mp3_get_tf_sum (); 
//   void mp3_get_flash_sum (); 
//   void mp3_get_tf_current (); 
//   void mp3_get_u_current (); 
//  void mp3_get_flash_current (); 
//   void mp3_single_loop (boolean state);	//set single loop 
//   void mp3_DAC (boolean state); 
//   void mp3_random_play (); 

#include <SoftwareSerial.h>    //memasukan library Software Serial
#include <DFPlayer_Mini_Mp3.h> //memasukan library DFPlayermini

SoftwareSerial mySerial(2, 3); //pin RX dan TX

void setup () { 
	Serial.begin (9600); //baud komunikasi pada 9600
	mp3_set_serial (Serial); 
	delay(5); 
	mp3_set_volume (15);
}

void loop () {   
mp3_play (1); //memainkan lagu 1 pada folder mp3 yang sudah direname dengan nama 0001.mp3
delay (10000); //jeda 10 detik

mp3_play (2); //memainkan lagu 2 dengan mode next
delay (10000);

mp3_play (3); //memainkan lagu 3
delay (10000);
}   