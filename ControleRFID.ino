#include <SPI.h>
#include <MFRC522.h>

#define verde 6
#define vermelho 7
#define buzzer 8
#define sda 10
#define rst 9

String tagID = "";
bool liberado = false; 
//Vetor de ID's cadastrados
String tagsPermitidas[] = {"3d7f7d63","75f8375f"};
MFRC522 RFID(sda, rst);

void setup() {
        Serial.begin(9600); // Comunicação Serial
        SPI.begin(); // Comunicacao SPI 
        RFID.PCD_Init(); // Leitor RFID
        pinMode(verde, OUTPUT);
        pinMode(vermelho, OUTPUT);
        pinMode(buzzer, OUTPUT);
}
 
void loop() {  
    // Verificação de existência de tag
    if ( !RFID.PICC_IsNewCardPresent() || !RFID.PICC_ReadCardSerial() ) {
        delay(50);
        return;
    }
    
    tagID = "";       
    // Estrutura de repetição padrão para coleta dos valores das tags pela função uid
    for (byte i = 0; i < RFID.uid.size; i++) {        
        tagID.concat(String(RFID.uid.uidByte[i], HEX));
    }        
    
    // Estrutura de repetição para comparação com as tags que permitimos no vetor
    for (int i = 0; i < (sizeof(tagsPermitidas)/sizeof(String)); i++) {
      if(  tagID.equalsIgnoreCase(tagsPermitidas[i])  ){
          liberado = true; //Libera tag
      }
    }       
    if(liberado == true){
      acessoLiberado();
      Serial.println("Tag Cadastrada: " + tagID); //Exibe a mensagem "Tag Cadastrada" e o ID da tag não cadastrada
    } else{
      acessoNegado();
      Serial.println("Tag Não Cadastrada: " + tagID); //Exibe a mensagem "Tag Cadastrada" e o ID da tag não cadastrada
    }
    delay(1000); //1 seg até a próxima leitura
}
 
void acessoLiberado(){  
  int bips = 1; //quantidade de bips
  for(int j=0; j<bips; j++){
    tone(buzzer,1500); //Liga buzzer
    digitalWrite(verde, HIGH);   
    delay(100);   
    noTone(buzzer); //Desliga buzzer
    digitalWrite(verde, LOW);
    delay(500);
  }  
  liberado = false;
}
 
void acessoNegado(){  
  int bips = 2;  //quantidade de bips
  for(int j=0; j<bips; j++){   
    tone(buzzer,500); //Liga buzzer
    digitalWrite(vermelho, HIGH);   
    delay(100); 
    noTone(buzzer); //Desliga buzzer
    digitalWrite(vermelho, LOW);
    delay(100);
  }  
}
