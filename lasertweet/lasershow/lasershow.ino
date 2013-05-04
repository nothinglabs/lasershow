// Arduino Laser Show with Full XY Control (using audio speakers for galvos)

// THIS VERSION MODIFIED FOR USE WITH TWITTER LASER SHOW

// Copyright Rich Olson - rich@nothinglabs.com 
// Project homepage: http://www.nothinglabs.com/ (see for additional documentation)

// Some rights reserved:
// Attribution-Noncommercial-Share Alike 3.0 Unported
// Details at: http://creativecommons.org/licenses/by-nc-sa/3.0/
// - You are free to use / modify this code for non-commercial projects
// - But you have to give credit to the original author 
// - Contact the author if you're interested in commercial use.
// - There are almost certainly some bugs / errors in documentation
// - No support is implied (but - if you email the author - you might get a helpful response)


void sendto (float x_target, float y_target);
void do_pwm();
void hold(long length);
void laser_on();
void laser_off();
void draw_letter(byte letter);

float x_level;
float y_level;
float move_scale = 2.4;

int laser_pin = 12;
int x_speaker_pin = 5;
int y_speaker_pin = 6;


float exp_move = 2.2;  //main value to adjust to change laser speed
                         //controls exponential laser movement
                        //lower values = faster drawing
                        //drawing too fast will decrease quality
                        //try setting it around 2.0 to 6
                        //this is the easiest variable to adjust to tweak quality vs. speed

float min_move = 0.5;  //if the laser gets this close to its target location - (on both axis)
                        //it's considered close enough 
                        //larger values increase drawing speed (at expense of quality)
                        //try setting it to about 0.5

float max_move = 30;     //maximum allowed movement - helps keep things from moving too fast
                        //when drawing long lines
                        
float laser_toggle_delay = 20;   //extra delay in units of 20 us before toggling laser on / off - try 0 to 100
float line_delay = 20;   //extra delay in units of 20 us at end of each line - try 0 to 100                      

float x_min_value = 25; //these set the minimum value for X and Y channels (on a scale of 0 to 255)
float y_min_value = 5; //this helps compensate for non-linear response some speakers have right off "center"
                        //these values must be below the corresponding max values
                        //increasing these values decrease image size
                        //try values of 0-30 - increase if you are having quality issues


float x_max_value = 210; //these set the minimum voltage for X and Y channels (on a scale of 0 to 255)
float y_max_value = 190; //this helps prevent the speaker from going outside its linear response range / distorting
                         //these values must be about the corresponding min values
                         //decreasing these values decrease image size
                         //try values of about 190 - decrease if you are having quality issues
                         //these values may also be manipulated to adjust the "aspect ratio"

boolean fast_marquee = true;  //if true - marquee scrolls at 2x normal speed
                                                
String inputString = "";
boolean stringComplete = false;


void setup()   {               

  inputString.reserve(200); 
  Serial.begin(9600);

  pinMode(laser_pin, OUTPUT);
  pinMode(x_speaker_pin, OUTPUT);
  pinMode(y_speaker_pin, OUTPUT);

  //resets the default arduino PWM stuff to much higher speed
  setPwmFrequency(x_speaker_pin, 1);
  setPwmFrequency(y_speaker_pin, 1);
  
}

  //basic routines
 
  //sendto (X,Y) - sends laser to X and Y coordinates - scale is always 0 to 255 (although actual resolution may be lower than this depending on above config values)
  //move (X,Y) - moves laser to X and Y coordinates -relative- to current position
  //move_scale = X - sets scaling - only used by "move" and "draw_letter" functions
                     //scaling of 2.8 allows for 5 characters across
  //draw_letter("X") - draws single letter at single position (uppercase only - see routine for other supported characters)
  //draw_string("STRING", X, Y, COUNT) - draws a string at X,Y for COUNT times
  //marquee("HELLO WORLD", CHARACTERS) - draws a scrolling marquee CHARACTERS long - text is autoscaled to fit (4 to 7 characters long works best)
  //laser_off() - turns laser off
  //laser_on() - turns laser on
  //hold(time) - tells laser to hold current position for roughly 20 microseconds (useful if you need to force laser to "catch up" after moving a large distance)

//put your laser control code here!

void loop()
{

    //run the default laser show demo
//    demo();
  
    //if we're just waiting - keep everything cool
    analogWrite(y_speaker_pin, 0);
    analogWrite(x_speaker_pin, 0);

    move_scale = 1.8;  
  
   int stringLength = 0;
   stringComplete = false;

   while (Serial.available() && stringLength < 200) {
    stringLength ++;
    char inChar = (char)Serial.read(); 
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
 
   if (stringComplete == true)
   { 
       marquee(inputString, 5);
       inputString = "";
   }

  Serial.println("ready");
}

void demo()
{

    move_scale = 1.8;
        
    marquee("THIS ARDUINO LASER SHOW USES AUDIO SPEAKERS FOR GALVOS!", 5);

    //star zoom in and out
    for (float loop = 0; loop < 25; loop ++)
    {
       move_scale = loop / 5; 
       laser_off();
       sendto (1,1);
       move (17,5);
       laser_on();
       move (13,24);
       move (-30,-18);
       move (34,0);
       move (-30,18);
       move (13,-24);
    }

    for (float loop = 25; loop > 0; loop --)
    {
       move_scale = loop / 5; 
       laser_off();
       sendto (1,1);
       move (17,5);
       laser_on();
       move (13,24);
       move (-30,-18);
       move (34,0);
       move (-30,18);
       move (13,-24);
    }
   
   //"speed test"
   move_scale = 1.8;  
   float exp_move_holder;
   exp_move_holder = exp_move;
   for (exp_move = 8; exp_move >= 1; exp_move = exp_move - exp_move / 20)
   {
     draw_string("SPEED", 1, 1, 1);
   }
   
   for (exp_move = 1; exp_move <= 8; exp_move = exp_move + exp_move / 20)
   {
     draw_string("TEST", 1, 1, 1);
   }
   exp_move = exp_move_holder;


   //"pacman" demo - this code is very hacky...
    move_scale = 2.3;
  
    for (int x = 0; x < 10; x++)
    {
      draw_string("prrr", 1, 1, 2);
      draw_string("qsss", 1, 1, 2);
    }

      draw_string("prrg", 1, 1, 2);
      draw_string("qsg", 1, 1, 3);
      draw_string("pg", 1, 1 , 3);

      draw_string("q", 1, 1, 1);
      draw_string("g", move_scale * 16 / 1.3, 1, 1);

      draw_string("q", 1, 1, 1);
      draw_string("g", move_scale * 16 / 1.4, 1, 1);

      draw_string("ph", 1, 1, 3);

    for (int x = 0; x < 6; x++)
    {
      draw_string("p", 1, 1, 5);
      draw_string("q", 1, 1, 5);
    }

   marquee("LINK TO INSTRUCTABLE AT WWW.NOTHINGLABS.COM", 5);

}


void move (float x_movedir, float y_movedir)
{
    
  x_movedir = x_movedir * move_scale;
  y_movedir = y_movedir * move_scale;
  
  sendto (x_level + x_movedir, y_level + y_movedir);
  
}

void sendto (float x_target, float y_target)
{

  
    float x_move;
    float y_move;
           
    float temp_exp_move = exp_move;

   while (fabs(x_target - x_level) > min_move || fabs(y_target - y_level) > min_move)
   {
  
       x_move = (x_target - x_level) / temp_exp_move;
       y_move = (y_target - y_level) / temp_exp_move;
                       
       while (fabs(x_move) > max_move || fabs(y_move) > max_move)
       {
          x_move = x_move *.5;
          y_move = y_move *.5;
          temp_exp_move = temp_exp_move * 1.5;
       }
 
        x_level = x_level + x_move;
        y_level = y_level + y_move;

        do_pwm();

    }

  x_level = x_target;
  y_level = y_target;
  do_pwm();
  hold(line_delay);

}

void laser_on()
{
  hold(laser_toggle_delay);
  digitalWrite(laser_pin, HIGH);
}

void laser_off()
{
  hold(laser_toggle_delay);
  digitalWrite(laser_pin, LOW);
}

void do_pwm()
{
    //actual PWM code
   
    float x_adjust;
    float y_adjust;
   
    //adds any offset (but still maintains 0-255 range) / handles scaling   
    x_adjust = x_min_value + (x_level * ((x_max_value - x_min_value) / 255)); 
    y_adjust = y_min_value + ((y_level) * ((y_max_value - y_min_value) / 255));
   
    if (x_adjust < 1) x_adjust = 1;
    if (y_adjust < 1) y_adjust = 1;
    if (x_adjust > x_max_value) x_adjust = x_max_value;
    if (y_adjust > y_max_value) y_adjust = y_max_value;
   
    analogWrite(y_speaker_pin, y_adjust);
    analogWrite(x_speaker_pin, x_adjust);

}


void hold(long length)
{
    long loop2 = 0;
        length = length * 2;  
       
        while (loop2 < length)
        {
            delay (1); //we'v messed with timers - so this is a bunch smaller than 1ms delay
            loop2 ++;
        }
}

//void draw_string(char text[], int x, int y, int count)
void draw_string(String text, int x, int y, int count)
{

  for (int loop = 0; loop < count; loop ++)
  {
    sendto (x,y);
    int i = 0;
    while (text.charAt(i) != '\0')
    {
       draw_letter(text.charAt(i)); 
       i ++;
    }
  }
  
}



void marquee(String text, int characters)
{

    float move_scale_holder;
    int i = 0;
    int loop = 0;

    move_scale_holder = move_scale;

    move_scale = 256 / (characters * 18);
    
    for (int x=0; x < characters + 2; x++)
    {
      text = String(" " + text + " ");
    }
         
    for (loop = 0; loop < (text.length() - 5); loop ++)
    {
      
      if (!fast_marquee) draw_string(text.substring(loop,loop + characters),move_scale * 9,1,1);
      draw_string(text.substring(loop,loop + characters),1,1,1);

    }

      move_scale = move_scale_holder;

}

void draw_letter(byte letter)
{
  
  float temp_x_level = x_level;
  float temp_y_level = y_level;
  
  //all characters 16x16 (leave -plenty- of space on right side to prevent overlap with next character)
  //characters are drawn starting from upper left corner
  //"cursor" is automatically returned to top right corner
  
  laser_off();
  
  switch (letter)
  {

    case 'A':        
        move (0,16);
        laser_on();
        move (7,-16);
        move (7,16);
        laser_off();
        move (-10,-7);
        laser_on();
        move (9,0);
        
        break;

 
    case 'B': 
        laser_on();
        move (0,16);
        move (11,-3);
        move (-7,-5);
        move (7,-5);
        move (-11,-3);
        break;

    case 'C': 
        move (11,16);
        laser_on();
        move (-11,-2);
        move (0,-12);
        move (11,-2);
        break;

    case 'D': 
        laser_on();
        move (0,16);
        move (9,-4);
        move (0,-8);
        move (-9,-4);
        break;


    case 'E': 

        move (10,0);
        laser_on();  
        move (-10,0);
        move (0,14);
        move (10,2);
        laser_off();
        move (-6,-10);
        laser_on();
        move (-6,0);
  
        break;


    case 'F':         
        move (10,0);
        laser_on();  
        move (-10,0);
        move (0,16);
        laser_off();
        move (0,-8);
        laser_on();
        move (5,0);
        break;



    case 'G': 

        move (9,8);
        laser_on();
        move (4,0);
        move (-1,8);
        move (-12,-2);
        move (0,-12);
        move (9,-2);

       break;


    case 'H': 
        move (2,0);
        laser_on();
        move (0,16);
        laser_off();
        move (0,-8);
        laser_on();
        move (9,0);
        laser_off();
        move (0,8);
        laser_on();
        move (0,-16);
        break;

    case 'I':    
        move (5,0);
        laser_on();
        move (0,16);
        break;

    case 'J':    
        move (0,8);
        laser_on();
        move (2,8);
        move (9,0);
        move (0,-16);
        break;

    case 'K':    
        laser_on();
        move (0,16);
        laser_off();
        move (11,0);
        laser_on();
        move (-9,-8);
        move (9,-8);
        break;

    case 'L':    
        laser_on();
        move (0,16);
        move (11,0);
        break;

    case 'M':    
        move (0,16);
        laser_on();
        move (0,-16);
        move (7,8);
        move (7,-8);
        move (0,16);
        break;

   case 'N':    
        move (0,16);
        laser_on();
        move (0,-16);
        move (11,16);
        move (0,-16);
        break;
 
    case 'O':      
        laser_on();
        move (11,0);
        move (0,16);
        move (-11,0);
        move (0,-16);
        break;


    case 'P':      
        move (0,16);
        laser_on();
        move (0,-16);
        move (11,4);
        move (-11,4);
        break;

    case 'Q':      

        move (6,0);
        laser_on();
        move (-6,8);
        move (6,8);
        move (6,-8);
        move (-6,-8);
        laser_off();
        move (0,7);
        laser_on();
        move (7,7);
        
        break;

   case 'R':      
        move (0,16);
        laser_on();
        move (0,-16);
        move (11,3);
        move (-11,5);
        move (11,10);
        break;

  case 'S':      

        move (11,0);
        laser_on();
        move (-11,4);
        move (11,6);
        move (-11,6);

        
        break;


 case 'T':      
       laser_on();
       move (13,0);
       laser_off();
       move (-8,0);
       laser_on();
       move (0,16);
       
       break;

  case 'U':      
        laser_on();
        move (2,16);
        move (9,0);
        move (2,-16);
        break;

  case 'V':      
        laser_on();
        move (5,16);
        move (5,-16);
        break;

  case 'W':      
        laser_on();
        move (3,16);
        move (4,-8);
        move (4,8);
        move (3,-16);
        break;


 case 'X':      
        laser_on();
        move (12,16);
        laser_off();
        move (-12,0);
        laser_on();
        move (12,-16);
        break;

 case 'Y':      
        laser_on();
        move (6,7);
        move (0,9);
        laser_off();
        move (0,-9);
        laser_on();
        move (6,-7);
        break;


 case 'Z':      
        laser_on();
        move (12,0);
        move (-12,16);
        move (12,0);
        break;


    case ' ':    
        break;

    case '$':      

        move (11,0);
        laser_on();
        move (-11,4);
        move (11,5);
        move (-11,5);
        laser_off();
        move (6,1);
        laser_on();
        move (0,-15);
        break;
   
    case '-':    
        move (2,8); 
        laser_on();
        move (10,0);
        break;

   case '+':    
        move (2,8); 
        laser_on();
        move (12,0);
        laser_off();
        move (-6,-8);
        laser_on();
        move (0,16);
        break;

  //lazy - really doing + for &
   case '&':    
        move (2,8); 
        laser_on();
        move (12,0);
        laser_off();
        move (-6,-8);
        laser_on();
        move (0,16);
       break;


    case '\'':    
        move (8,0); 
        laser_on();
        move (0,4);
        break;



    case '!':    
        move (5,0);  //tweaked to make sure doesn't look like '?'
        laser_on();
        move (-1,3);
        move (1,6);
        laser_off();
        move (0,5);
        laser_on();
        move (0,2);
        break;
        
    case '?':
        laser_on();
        move (12,1);
        move (-6,6);
        move (0,2);
        laser_off();
        move (0,5);
        laser_on();
        move (0,2);
        break;

    case '.':    
        move (5,14);
        laser_on();
        move (0,2);
        move (0,-2);
        break;

    case '*':    
        move (5,8);
        laser_on();
        move (0,2);
        move (0,-2);
        break;


    case ':':
        move (5,2);
        laser_on();
        move (0,3);
        laser_off();
        move (0,5);
        laser_on();
        move (0,3);
        break;

    case '/':    
        move (0,16);
        laser_on();
        move (12,-16);
        break;

    case '(': 
        move (11,0);
        laser_on();
        move (-4,8);
        move (4,8);
        break;
        
    case ')': 
        move (2,0);
        laser_on();
        move (4,8);
        move (-4,8);
        break;


    case '@': 
        move (6,9);
        laser_on();
        move (5,0);
        move (0,-5);
        move (-5,0);
        move (0,5);
        move (6,-7);
        move (-12,-0);
        move (0,12);
        move (10,0);

        break;

   case '#': 
        move (7,0);
        laser_on();
        move (0,16);
        laser_off();
        move (4,0);
        laser_on();
        move (0,-16);
        laser_off();
        move (-11,4);
        laser_on();
        move (16,0);
       laser_off();
        move (0,5);
        laser_on();
        move (-16,0);
   
        break;


    case '0':      
        move (0,1);
        laser_on();
        move (10,0);
        move (0,13);
        move (-10,0);
        move (0,-13);
        laser_off();
        move (12,-1);
        laser_on();
        move (-12,16);
        break;
       
        
    case '1':    
        move (5,0);
        laser_on();
        move (0,16);
        break;

    case '2':    
        laser_on();
        move (12,2);
        move (-12,14);
        move (12,0);
        break;

    case '3':    
        laser_on();
        move (12,3);
        move (-7,6);
        move (7,6);
        move (-12,3);
        break;

    case '4':    
        laser_on();
        move (0,7);
        move (12,0);
        laser_off();
        move (0,-7);
        laser_on();
        move (0,16);
        break;

    case '5':    
        move (0,16);
        laser_on();
        move (12,-5);
        move (0,-5);
        move (-12,0);
        move (0,-6);
        move (12,0);
        break;
        

    case '6': 
        move (0,6);
        laser_on();
        move (12,3);
        move (0,6);
        move (-12,1);
        move (0,-16);
        move (10,0);
       break;


    case '7': 
        laser_on();
        move (12,0);
        move (-10,16);
       break;

    case '8': 
        move (0,2);
        laser_on();
        move (12,12);
        move (-6, 2);
        move (-6,-2);
        move (12,-12);
        move (-6, -2);
        move (-6, 2);
       break;

    case '9': 
        move (12,6);
        laser_on();
        move (-12,-3);
        move (0,-3);
        move (12,4);
        move (0,12);
       break;


//following characters are just for pacman demo

//pacman open
    case 'p': 
        move (6,0);
        laser_on();
        move (6,2);
        move (-6,6);
        move (6,6);
        move (-6,2);
        move (-6,-8);
        move (6,-8);
       break;


//pacman closed
    case 'q': 
        move (6,0);
        laser_on();
        move (6,7);
        move (-7,1);
        move (7,1);
        move (-6,7);
        move (-6,-8);
        move (6,-8);
       break;
       

        //pellet 1 
        case 'r':    
        move (0,8);
        laser_on();
        move (0,2);
        move (0,-2);
        break;


        //pellet 2 
        case 's':    
        move (9,8);
        laser_on();
        move (0,2);
        move (0,-2);
        break;

//ghost
    case 'g': 
        move (8,0);
        laser_on();
        move (8,3);
        move (0,13);
        move (-4,-5);
        move (-4,5);
        move (-4,-5);
        move (-4,5);
        move (0,-13);
        move (8,-3);
       break;


//ghost gets eaten
    case 'h': 
        move (-7,8);
        laser_on();
        move (3,3);
        move (-6,-6);
        move (3,3);
        move (3,-3);
        move (-6,6);
       break;

        
  }

  //return to bottom right corner of character "cell" - adds 2 spaces
  laser_off();
  sendto (temp_x_level + (18 * move_scale) , temp_y_level);
  
}


/**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired.
 *   - Pins 9 and 10 are paired.
 *   - Pins 3 and 11 are paired.
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


