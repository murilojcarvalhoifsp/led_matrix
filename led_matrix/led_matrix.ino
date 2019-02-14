/*
 *    MatrixLED.cpp - An example for movimenting text on a Matrix LED (8x8) with a MAX7219/MAX7221, 
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */


#include "./LedControl.h"  //Library for SPI-Hardware (20x faster tha SPI-Software) https://github.com/jacken/LedControl_HW_SPI
 
LedControl lc = LedControl(10,8);

unsigned char desenhacaracter(unsigned char carac); 
void buildPhrase(char *texto);
void deslocaCasa(void);


#define qtd_max_carac   80    //Limite de caracters

#define   normal      0
#define   invertido   1

unsigned char phrase[qtd_max_carac][8] = {0};
unsigned char speedRunning = 0;
unsigned char fillout;
 
byte alphabet[][8]= {
  { //pos0
  B00000000,    //A
  B00111100,
  B01100110,
  B01100110,
  B01111110,
  B01100110,
  B01100110,
  B01100110
},{ //pos1
  B00000000,    //B
  B01111100,
  B01100110,
  B01100110,
  B01111100,
  B01100110,
  B01100110,
  B01111100
},{ //pos2
  B00000000,    //C
  B00111100,
  B01100110,
  B01100000,
  B01100000,
  B01100000,
  B01100110,
  B00111100
},{ //pos3
  B00000000,    //D
  B01111100,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01111100
},{ //pos4
  B00000000,    //E
  B01111110,
  B01100000,
  B01100000,
  B01111100,
  B01100000,
  B01100000,
  B01111110
},{ //pos5
  B00000000,    //F
  B01111110,
  B01100000,
  B01100000,
  B01111100,
  B01100000,
  B01100000,
  B01100000
},{ //pos6
  B00000000,    //G
  B00111100,
  B01100110,
  B01100000,
  B01100000,
  B01101110,
  B01100110,
  B00111100
},{ //pos7
  B00000000,    //H
  B01100110,
  B01100110,
  B01100110,
  B01111110,
  B01100110,
  B01100110,
  B01100110
},{ //pos8
  B00000000,    //I
  B00111100,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00111100
},{ //pos9
  B00000000,    //J
  B00011110,
  B00001100,
  B00001100,
  B00001100,
  B01101100,
  B01101100,
  B00111000
},{ //pos10
  B00000000,    //K
  B01100110,
  B01101100,
  B01111000,
  B01110000,
  B01111000,
  B01101100,
  B01100110
},{ //pos11
  B00000000,    //L
  B01100000,  
  B01100000,
  B01100000,
  B01100000,
  B01100000,
  B01100000,
  B01111110
},{ //pos12
  B00000000,    //M
  B01100011,
  B01110111,
  B01111111,
  B01101011,
  B01100011,
  B01100011,
  B01100011
},{ //pos13
  B00000000,    //N
  B01100011,
  B01110011,
  B01111011,
  B01101111,
  B01100111,
  B01100011,
  B01100011
},{ //pos14
  B00000000,    //O
  B00111100,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00111100
},{ //pos15
  B00000000,    //P
  B01111100,
  B01100110,
  B01100110,
  B01100110,
  B01111100,
  B01100000,
  B01100000
},{ //pos16
  B00000000,    //Q
  B00111100,
  B01100110,
  B01100110,
  B01100110,
  B01101110,
  B00111100,
  B00000110
},{ //pos17
  B00000000,    //R
  B01111100,
  B01100110,
  B01100110,
  B01111100,
  B01111000,
  B01101100,
  B01100110
},{ //pos18
  B00000000,    //S
  B00111100,
  B01100110,
  B01100000,
  B00111100,
  B00000110,
  B01100110,
  B00111100
},{ //pos19
  B00000000,    //T
  B01111110,
  B01011010,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000
},{ //pos20
  B00000000,    //U
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00111110
},{ //pos21
  B00000000,    //V
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00111100,
  B00011000
},{ //pos22
  B00000000,    //W
  B01100011,
  B01100011,
  B01100011,
  B01101011,
  B01111111,
  B01110111,
  B01100011
},{ //pos23
  B00000000,    //X
  B01100011,
  B01100011,
  B00110110,
  B00011100,
  B00110110,
  B01100011,
  B01100011
},{ //pos24
  B00000000,    //Y
  B01100110,
  B01100110,
  B01100110,
  B00111100,
  B00011000,
  B00011000,
  B00011000
},{ //pos25
  B00000000,    //Z
  B01111110,
  B00000110,
  B00001100,
  B00011000,
  B00110000,
  B01100000,
  B01111110
},{ //pos26
  B00000000,  //blank
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},{ //pos27
  B00000000,    //a
  B00000000,
  B00000000,
  B00111100,
  B00000110,
  B00111110,
  B00100110,
  B00111110
},{ //pos28
  B00000000,    //b
  B01100000,
  B01100000,
  B01100000,
  B01111100,
  B01100110,
  B01100110,
  B01111100
},{ //pos29
  B00000000,    //c
  B00000000,
  B00000000,
  B00111100,
  B01100110,
  B01100000,
  B01100110,
  B00111100
},{ //pos30
  B00000000,    //d
  B00000110,
  B00000110,
  B00000110,
  B00111110,
  B01100110,
  B01100110,
  B00111110
},{ //pos31
  B00000000,    //e
  B00000000,
  B00000000,
  B00111100,
  B01100110,
  B01111110,
  B01100000,
  B00111100
},{ //pos32
  B00000000,    //f
  B00011100,
  B00110110,
  B00110000,
  B00110000,
  B01111100,
  B00110000,
  B00110000
},{ //pos33
  B00000000,    //g
  B00000000,
  B00111110,
  B01100110,
  B01100110,
  B00111110,
  B00000110,
  B00111100
},{ //pos34
  B00000000,    //h
  B01100000,
  B01100000,
  B01100000,
  B01111100,
  B01100110,
  B01100110,
  B01100110
},{ //pos35
  B00000000,    //i
  B00000000,
  B00011000,
  B00000000,
  B00011000,
  B00011000,
  B00011000,
  B00111100
},{ //pos36
  B00000000,    //j
  B00001100,
  B00000000,
  B00001100,
  B00001100,
  B01101100,
  B01101100,
  B00111000
},{ //pos37
  B00000000,    //k
  B01100000,
  B01100000,
  B01100110,
  B01101100,
  B01111000,
  B01101100,
  B01100110
},{ //pos38
  B00000000,    //l
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000
},{ //pos39
  B00000000,    //m
  B00000000,
  B00000000,
  B01100011,
  B01110111,
  B01111111,
  B01101011,
  B01101011
},{ //pos40
  B00000000,    //n
  B00000000,
  B00000000,
  B01111100,
  B01111110,
  B01100110,
  B01100110,
  B01100110
},{ //pos41
  B00000000,    //o
  B00000000,
  B00000000,
  B00111100,
  B01100110,
  B01100110,
  B01100110,
  B00111100
},{ //pos42
  B00000000,    //p
  B00000000,
  B01111100,
  B01100110,
  B01100110,
  B01111100,
  B01100000,
  B01100000
},{ //pos43
  B00000000,    //q
  B00000000,  
  B00111100,
  B01101100,
  B01101100,
  B00111100,
  B00001101,
  B00001111
},{ //pos44
  B00000000,    //r
  B00000000,
  B00000000,
  B01111100,
  B01100110,
  B01100110,
  B01100000,
  B01100000
},{ //pos45
  B00000000,    //s
  B00000000,
  B00000000,
  B00111110,
  B01000000,
  B00111100,
  B00000010,
  B01111100
},{ //pos46
  B00000000,    //t
  B00000000,
  B00011000,
  B00011000,
  B01111110,
  B00011000,
  B00011000,
  B00011000
},{ //pos47
  B00000000,    //u
  B00000000,
  B00000000,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00111110
},{ //pos48
  B00000000,    //v
  B00000000,
  B00000000,
  B00000000,
  B01100110,
  B01100110,
  B00111100,
  B00011000
},{ //pos49
  B00000000,    //w
  B00000000,
  B00000000,
  B01100011,
  B01101011,
  B01101011,
  B01101011,
  B00111110
},{  //pos50
  B00000000,    //x
  B00000000,
  B00000000,
  B01100110,
  B00111100,
  B00011000,
  B00111100,
  B01100110
},{ //pos51
  B00000000,    //y
  B00000000,
  B00000000,
  B01100110,
  B01100110,
  B00111110,
  B00000110,
  B00111100
},{ //pos52
  B00000000,    //z
  B00000000,
  B00000000,
  B00111100,
  B00001100,
  B00011000,
  B00110000,
  B00111100
},{ //pos53
  B00000000,    //0
  B00111100,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00111100
},{ //pos54
  B00000000,    //1
  B00011000,
  B00011000,
  B00111000,
  B00011000,
  B00011000,
  B00011000,
  B01111110
},{ //pos55
  B00000000,    //2
  B00111100,
  B01100110,
  B00000110,
  B00001100,
  B00110000,
  B01100000,
  B01111110
},{ //pos56
  B00000000,    //3
  B00111100,
  B01100110,
  B00000110,
  B00011100,
  B00000110,
  B01100110,
  B00111100
},{ //pos57
  B00000000,    //4
  B00001100,
  B00011100,
  B00101100,
  B01001100,
  B01111110,
  B00001100,
  B00001100
},{ //pos58
  B00000000,    //5
  B01111110,
  B01100000,
  B01111100,
  B00000110,
  B00000110,
  B01100110,
  B00111100
},{ //pos59
  B00000000,    //6
  B00111100,
  B01100110,
  B01100000,
  B01111100,
  B01100110,
  B01100110,
  B00111100
},{ //pos60
  B00000000,    //7
  B01111110,
  B01100110,
  B00001100,
  B00001100,
  B00011000,
  B00011000,
  B00011000
},{ //pos61
  B00000000,    //8
  B00111100,
  B01100110,
  B01100110,
  B00111100,
  B01100110,
  B01100110,
  B00111100
},{ //pos62
  B00000000,    //9
  B00111100,
  B01100110,
  B01100110,
  B00111110,
  B00000110,
  B01100110,
  B00111100
},{ //pos63
  B00000000,    //.
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00110000,
  B00110000
},{ //pos64
  B00000000,    //,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00110000,
  B00110000,
  B01100000
},{ //pos65
  B00000000,    //:
  B00000000,
  B00011000,
  B00011000,
  B00000000,
  B00011000,
  B00011000,
  B00000000
},{ //pos66
  B00000000,  //-
  B00000000,
  B00000000,
  B01111110,
  B01111110,
  B00000000,
  B00000000,
  B00000000
},{ //pos67
  B00000000,  //(
  B00000010,
  B00000100,
  B00001100,
  B00001100,
  B00001100,
  B00000100,
  B00000010
},{ //pos68
  B00000000,  //)
  B01000000,
  B00100000,
  B00110000,
  B00110000,
  B00110000,
  B00100000,
  B01000000
}
};


void setup() {
  Serial.begin(9600);
  
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.shutdown(2,false);
  lc.shutdown(3,false);
  lc.shutdown(4,false);
  lc.shutdown(5,false);
  lc.shutdown(6,false);
  lc.shutdown(7,false);
 
  // seta o brilho para um valor mediano:
  //-------------------------------------
  lc.setIntensity(0,8);
  lc.setIntensity(1,8);
  lc.setIntensity(2,8);
  lc.setIntensity(3,8);
  lc.setIntensity(4,8);
  lc.setIntensity(5,8);
  lc.setIntensity(6,8);
  lc.setIntensity(7,8);
 
  // limpa o display:
  //-----------------
  lc.clearDisplay(0);  
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
  lc.clearDisplay(4);
  lc.clearDisplay(5);
  lc.clearDisplay(6);
  lc.clearDisplay(7);
}
 

 
void loop(){

  
  //Write the message here. Max 80 characters. 
  //Must use at least 8 spaces at the begin of the message, in order to text start in the first display.
  speedRunning = 20;
  buildPhrase("        Instituto Federal de Educacao Ciencia e Tecnologia de Sao Paulo");
  delay(500);
  speedRunning = 35;
  buildPhrase("        IFSP - Braganca Paulista");
  delay(500);
  speedRunning = 30;
  buildPhrase("        bra.ifsp.edu.br   (11)4034-7800");
  delay(1000);

  fillout = !fillout;
}



void buildPhrase(char *texto){
//  <<---------<<---------<<---------<<---------<<---------<<---------<<---------<<---------
//      disp7      disp6      disp5      disp4      disp3      disp2      disp1      disp0
  
  unsigned char tamanhoVetor=0;
  unsigned int i=0, i2=0, disp=0, linha=0;
  unsigned int deslocamento=0;
  unsigned char posi_vetor_carac=0;

  tamanhoVetor = strlen(texto);
  //Serial.print("texto: ");
  //Serial.println(texto);
  //Serial.print("tamanhoVetor: ");
  //Serial.println(tamanhoVetor);

  for(i2=0; i2<tamanhoVetor; i2++){
    posi_vetor_carac = desenhacaracter(texto[i2]);
    //Serial.print("carac: ");
    //Serial.println(texto[i2]);
    //Serial.print("posi vetor carac: ");
    //Serial.println(posi_vetor_carac);
    for (i=0; i<8; i++){
      phrase[i2][i] = alphabet[posi_vetor_carac][i];
      if(fillout){  //Inverte o estado dos LEDs
        phrase[i2][i] ^= 0b11111111;
      }
      //Serial.print("phrase: ");
      //Serial.println(phrase[i2][i], BIN);
      //Serial.print("alphabet: ");
      //Serial.println(alphabet[posi_vetor_carac][i], BIN);
    }
  }

  for(deslocamento=0; deslocamento<((8*tamanhoVetor)+1); deslocamento++){
    for (disp=0; disp<8; disp++){
      for(linha=0; linha<8; linha ++){
        lc.setRow(disp,linha,phrase[7-disp][linha]);
      }
    }
    deslocaCasa();
    delay(speedRunning);
  }

  
}

void deslocaCasa(void){
  unsigned char i=0, i2=0;

  for(i=0; i<(qtd_max_carac-1); i++){
    for(i2=0; i2<8; i2++){
          phrase[i][i2] = (phrase[i][i2]<<1) | ((phrase[i+1][i2] >>7) & 0b00000001);
    }
  }
}



unsigned char desenhacaracter(unsigned char carac){

  unsigned char posi_vetor_carac;
  unsigned int i=0;
  
  if(carac>=0x41 && carac<=0x5A){   //upper case
    for (i=0; i<26; i++){
      if(carac == (0x41+i)){   //0x41 == 'A' na tabela ASCII
        posi_vetor_carac = i;
      }
    }
  }
  else if(carac>=0x61 && carac<=0x7A){   //lower case
    for (i=0; i<26; i++){
      if(carac == (0x61+i)){   //0x61 == 'a' na tabela ASCII
        posi_vetor_carac = i+27;
      }
    }
  }
 else if(carac>=0x30 && carac<=0x39){   //numbers
    for (i=0; i<10; i++){
      if(carac == (0x30+i)){   //0x30 == '0' na tabela ASCII
        posi_vetor_carac = i+53;
      }
    }
  }
  else if(carac==0x20){   //space
        posi_vetor_carac = 26;
  }
  else if(carac==0x2E){   //.
        posi_vetor_carac = 63;
  }
  else if(carac==0x2C){   //,
        posi_vetor_carac = 64;
  }
  else if(carac==0x3A){   //:
        posi_vetor_carac = 65;
  }
  else if(carac==0x2D){   //-
        posi_vetor_carac = 66;
  }
  else if(carac==0x28){   //(
        posi_vetor_carac = 67;
  }
  else if(carac==0x29){   //)
        posi_vetor_carac = 68;
  }

  return(posi_vetor_carac);
  
   
//  for (i=0; i<8; i++){
    //lc.setRow(disp,i,alphabet[posi_vetor_carac][i]);
//  }
  
}
