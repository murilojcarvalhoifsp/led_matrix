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
#include "led_matrix_alphabet.h"
 
LedControl lc = LedControl(10,8);

unsigned char desenhacaracter(unsigned char carac); 
void buildPhrase(char *texto_sem_espaco);
void deslocaCasa(void);


#define qtd_max_carac   88    //Limite de caracters

#define   normal      0
#define   invertido   1

unsigned char phrase[qtd_max_carac][8] = {0};
unsigned char speedRunning = 0;
unsigned char fillout;


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

  //Write the message here. Max 80 characters per message. 
  speedRunning = 20;
  buildPhrase("Instituto Federal de Educacao, Ciencia e Tecnologia de Sao Paulo");
  delay(500);
  speedRunning = 35;
  buildPhrase("IFSP - Braganca Paulista");
  delay(500);
  speedRunning = 30;
  buildPhrase("bra.ifsp.edu.br   (11)4034-7800");
  delay(1000);

  fillout = !fillout;
}







void buildPhrase(char *texto_sem_espaco){
//  <<---------<<---------<<---------<<---------<<---------<<---------<<---------<<---------
//      disp7      disp6      disp5      disp4      disp3      disp2      disp1      disp0
  
  unsigned char tamanhoVetor=0;
  unsigned int i=0, i2=0, disp=0, linha=0;
  unsigned int deslocamento=0;
  unsigned char posi_vetor_carac=0;
  char texto[qtd_max_carac+8] = "        ";

  strcat(texto, texto_sem_espaco);
  tamanhoVetor = strlen(texto);

  for(i2=0; i2<tamanhoVetor; i2++){
    posi_vetor_carac = desenhacaracter(texto[i2]);
    for (i=0; i<8; i++){
      phrase[i2][i] = alphabet[posi_vetor_carac][i];
      if(fillout){  //Inverte o estado dos LEDs
        phrase[i2][i] ^= 0b11111111;
      }
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
  
}
