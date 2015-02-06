#define CTRL 8
#define chanel_number 8  //set the number of chanels
#define default_servo_value 1500  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 0  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 10  //set PPM signal output pin on the arduino

int controlBuffer[CTRL] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup()
{
Serial.begin(9600);
Serial.println("Ready");

  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

}

void loop()
{
  char inByte = ' ';
  if(Serial.available())
  {
    delay(1);
   char inByte = Serial.read();
   if(inByte == '<')
   {
     for(int i = 0; i < CTRL; i++)
     {
       delay(1);
       inByte = Serial.read();
       controlBuffer[i] = map((int) inByte, 0, 128, 1000, 2000);
       if(i==3)
       Serial.println(controlBuffer[i]);
       
     }
     inByte = Serial.read();
   }
  }
  delay(1);
}

ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if(state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PPM_PulseLen * 2;
    state = false;
  }
  else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= chanel_number){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PPM_PulseLen;// 
      OCR1A = (PPM_FrLen - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (controlBuffer[cur_chan_numb] - PPM_PulseLen) * 2;
      calc_rest = calc_rest + controlBuffer[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
