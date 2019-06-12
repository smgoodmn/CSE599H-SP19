#include <Arduino.h>
#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9

char serialData;

unsigned char Display_Buffer[2];
/*const unsigned char  Word1[1][32] = 
{
0xFF,0xFF,0xFF,0xE1,0xC0,0x80,0x80,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,
0xFF,0xFF,0xFF,0x87,0x03,0x01,0x01,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,0xFF,/*"C:\Users\Lin\Desktop\心.BMP",0

};*/

// listening
const unsigned char  listening[1][32] = 
{ 
0xfd,0xfb,0xf7,0xee,0xdd,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0xdd,0xee,0xf7,0xfb,0xfd,0xff,0xff,0x7f,0xff,0xdf,0xbf,0x77,0x6f,0x6f,0x77,0xbf,0xdf,0xff,0x7f,0xff,0xff
};

//recordning 
const unsigned char  recording[1][32] = 
{ 
0xff,0xff,0xff,0xf0,0xef,0xef,0xee,0xed,0xed,0xee,0xef,0xef,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0xf7,0xf7,0x77,0xb7,0xb7,0x77,0xf7,0xf7,0x0f,0xff,0xff,0xff
};

//question mark 
const unsigned char unknown[1][32] =
{0xff,0xff,0xff,0xf9,0xf7,0xef,0xef,0xef,0xef,0xef,0xef,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe5,0xdf,0xbf,0x7f,0xff,0xff,0xff,0xff};

// garbage disposal
const unsigned char  g[1][32] = 
{
0xff,0xff,0xff,0xff,0xff,0xff,0xf8,0xf7,0xf7,0xfb,0xff,0xfe,0xfd,0xfb,0xff,0xff,0xff,0xff,0xff,0x2f,0x27,0xc3,0x03,0xc3,0x27,0x2f,0xff,0xef,0xdf,0xbf,0xff,0xff,
};

// microwave close
const unsigned char  m[1][32] = 
{
0xab,0x83,0xff,0xa3,0x8b,0xff,0x83,0xbb,0x83,0xff,0xfb,0x83,0xff,0xbb,0xbb,0x83,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x81,0xbd,0xbd,0xa5,0xa5,0xa5,0xbd,0x81,0xff/*"Unnamed file",0*/
};

//microwave open
const unsigned char  w[1][32] = 
{
0x83,0xef,0xdf,0x83,0xff,0xab,0xab,0x83,0xff,0x8f,0xaf,0x83,0xff,0x83,0xbb,0x83,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x81,0xbd,0xbd,0xa5,0xa5,0xa5,0xbd,0x81,0xff,/*"Unnamed file",0*/
};

// microwave ending
const unsigned char  e[1][32] = 
{
0xff,0xff,0xc5,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x81,0xbd,0xbd,0xa5,0xa5,0xa5,0xbd,0x81,0xff
};

//toaster
const unsigned char  t[1][32] = 
{
0xff,0xe3,0xdd,0xbe,0xbf,0xbf,0xbf,0xbf,0xbf,0xbf,0xbf,0xbf,0xbe,0xdd,0xe3,0xff,0xff,0xff,0xff,0xff,0x03,0xfb,0xfb,0xfb,0xfb,0xfb,0xfb,0x03,0xff,0xff,0xff,0xff,/*"Unnamed file",0*/
};

//fridge open
const unsigned char  f[1][32] = 
{
0x83,0xef,0xdf,0x83,0xff,0xbb,0xab,0x83,0xff,0x8f,0xaf,0x82,0xfe,0x82,0xba,0x83,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x01,0x91,0xdd,0xdd,0x01,0xff
};

//coffee grinder
const unsigned char  c[1][32] = 
{
0xff,0xff,0xff,0xff,0xfc,0xfb,0xbb,0x5b,0xeb,0xfb,0xfb,0xfc,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0xf7,0xfb,0xfb,0xfb,0xfb,0xf7,0x0f,0xdf,0x1f,0xff,0xff
};

//fridge close
const unsigned char  r[1][32] = 
{
0xab,0x83,0xff,0xa3,0x8b,0xff,0x83,0xbb,0x83,0xff,0xfb,0x82,0xfe,0xba,0xba,0x83,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x01,0x91,0xdd,0xdd,0x01,0xff/*"Unnamed file",0*/
};


int currentImageID;
unsigned char currentImage[1][32]; 

void setup()
{
  pinMode(LEDARRAY_D, OUTPUT); 
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);
  currentImageID=1;
  memcpy(currentImage,listening,sizeof(currentImage));
  Serial.begin(9600);
}

void loop(){
  //if (currentImageID == 1) {
   // Display(g);
  //}
  //else if (currentImageID == 2) {
  //  Display(Word2);
  //}
  //Display(w);
  Display(currentImage);
  
  if (Serial.available() > 0){
    serialData = Serial.read();
    Serial.print(serialData);
    int count = 500;
    //while(count){
    //  Display(currentImage);
    //  count--;
    //}
    Serial.print("display\n");
    if (serialData == 'l'){
      memcpy(currentImage,listening,sizeof(currentImage));
    } else if (serialData == 'r'){
      memcpy(currentImage,recording,sizeof(currentImage));
    } else if (serialData == 'g'){
      memcpy(currentImage,g,sizeof(currentImage));
    } else if (serialData == 'c'){
      Serial.print("displaying c");
      memcpy(currentImage,c,sizeof(currentImage));
    } else if (serialData == 't'){
      memcpy(currentImage,t, sizeof(currentImage));
    } else if (serialData == 'm'){
      memcpy(currentImage,m, sizeof(currentImage));
    } else if (serialData == 'g'){
      memcpy(currentImage,g, sizeof(currentImage));
    } else if (serialData == 'w'){
      memcpy(currentImage,w, sizeof(currentImage));
    } else if (serialData == 'e'){
      memcpy(currentImage,e, sizeof(currentImage));
    } else if (serialData == 'f'){
      memcpy(currentImage,f, sizeof(currentImage));
    } else if (serialData == 'r'){
      memcpy(currentImage,r, sizeof(currentImage));
    } 
 
  }
}

/*void loop()
{
  int a=300, i=300;
  while(i)
  {
  Display(Word1);
  i--;
  }
  delay(100);
    while(a)
  {
  Display(Word2);
  a--;
  }
  delay(100);
}*/



//************************************************************
//num means the number of word  dat[][32] is the name of font
//*************************************************************
void Display(const unsigned char dat[][32])         
{
  unsigned char i;

  for( i = 0 ; i < 16 ; i++ )
  {
    digitalWrite(LEDARRAY_G, HIGH);   //When updating the data,please close the display. After updating the data, open the 138 display line. Avoiding ghosting.

    Display_Buffer[0] = dat[0][i];    
    Display_Buffer[1] = dat[0][i+16];

    Send(Display_Buffer[1]);
    Send(Display_Buffer[0]);

    digitalWrite(LEDARRAY_LAT, HIGH);         //storage data  
    delayMicroseconds(1);

    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    Scan_Line(i);             //Choose the line i

    digitalWrite(LEDARRAY_G, LOW);

    delayMicroseconds(100);;      // Delay for a period of time to let the LED light up.          
  } 
}

//****************************************************
//Scanning line
//****************************************************
void Scan_Line( unsigned char m)
{ 
  switch(m)
  {
    case 0:     
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);          
      break;
    case 1:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 2:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 3:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 4:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 5:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 6:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 7:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 8:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 9:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;  
    case 10:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 11:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 12:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 13:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 14:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 15:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    default : break;  
  }
}

//****************************************************
//Send the data      
//****************************************************
void Send( unsigned char dat)
{
  unsigned char i;
  digitalWrite(LEDARRAY_CLK, LOW);
  delayMicroseconds(1);;  
  digitalWrite(LEDARRAY_LAT, LOW);
  delayMicroseconds(1);;

  for( i = 0 ; i < 8 ; i++ )
  {
    if( dat&0x01 )
    {
      digitalWrite(LEDARRAY_DI, HIGH);  
    }
    else
    {
      digitalWrite(LEDARRAY_DI, LOW);
      }

    delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, HIGH);       // Rising edge sends the data 
      delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, LOW);
      delayMicroseconds(1);   
    dat >>= 1;

  }     
}
