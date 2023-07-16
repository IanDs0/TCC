#include <Wire.h>
#include <ESP32Servo.h>
#include <SparkFunLSM6DS3.h>

// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>

#include "BluetoothSerial.h"
#include "esp32-hal-bt.h"

#include <ArduinoJson.h>
#include <cstring>

#define PI 3.1415926535897932384626433832795 // Definindo o valor de pi

//Bluetooth
BluetoothSerial BT;
bool led = false;//status Bluetooth

//Classe para os motores
class Motor {
  private:
    Servo motor;
    int pin;
    int estado;

  public:
    void teste(int pin, int estado){
      pinMode(pin, OUTPUT);
      
      this->estado = estado;
      this->pin = pin;
      this->motor.attach(pin);
    }

    void setEstado (int estado){
      motor.write(estado);
      // motor.write(map(estado, 0, 1023, 0, 179));
    }
};

// Cria um objeto para interagir com o sensor
LSM6DS3 myIMU( I2C_MODE, 0x6B ); // endereço padrão do LSM6DS3 é 0x6A
// Dados do sensor giroscopio
// float roll = 0.0;

//ESC
int pinEsc =5;//D5
Motor ESC;
// Servo frontal
int pinServoF = 4;//D4
Motor ServoF;
// Servo traseiro
int pinServoT = 16;//D16
Motor ServoT;

//speed motor
int speed = 20;
//poss sevo motores
int pos = 0;





// const char key[6] = "_Zay_";

// Tamanho do buffer para armazenar o JSON
const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 120;

// Cria um objeto DynamicJsonDocument
DynamicJsonDocument doc(bufferSize);



class Item {
  private:

    const char* type;
    int output;
    const char* pin;

  public:

    bool setType(const char* charItem) {
      if (charItem != NULL) {
        this->type = charItem;
        return true;
      }
      return false;
    }
    bool setOutput(int charItem) {
      this->output = charItem;
      return true;
    }
    bool setPin(const char* charItem) {
      if (charItem != NULL) {
        this->pin = charItem;
        return true;
      }
      return false;
    }
};

class Input{
  private:

    bool authorization;

    Item item [4];
    /*
    0 temperature;
    1 giroscopy;
    2 speedometer;
    3 battery;
    */
  
  public:

  bool setAuthorization (JsonObject chatAuthorization){
    // char key[6] = "_Zay_";
    if (chatAuthorization.containsKey("autorization"))
      if (strcmp(chatAuthorization["autorization"].as<const char*>(), "_Zay_") == 0){
        authorization = true;
        return true;
      }
    return false;
  }

  bool setItem (JsonObject json, int usize){
    if(usize>=0 && usize<=3)
      if(this->item[usize].setType(json["type"].as<const char*>()) && json.containsKey("type"))
        if(this->item[usize].setOutput(json["output"].as<int>()) && json.containsKey("output"))
          if(this->item[usize].setPin(json["pin"].as<const char*>()) && json.containsKey("pin"))
            return true;
    return false;
  }

  bool setAllItems (DynamicJsonDocument json){
    if(setAuthorization(json["autentication"])){
      JsonObject sensors = json["sensors"];
      if(setItem (sensors["temperature"] , 0))
        if(setItem (sensors["gyroscopy"] , 1))
          if(setItem (sensors["speedometer"] , 2))
            if(setItem (sensors["battery"] , 3))
              return true;
    }
    return false;
  }
};





void setup() {
  Serial.begin(115200);
  while (!Serial); // Aguarda a conexão com o monitor serial

  // Inicializa o sensor
  myIMU.begin();

  //Servo Frontal
  ServoF.teste(pinServoF, pos);
  //Servo Traseiro
  ServoT.teste(pinServoT, pos);
  //Esc
  ESC.teste(pinEsc, pos);

  //Bluetooth
  BT.begin("ESP-32");
  pinMode(LED_BUILTIN, OUTPUT);

  // Obtém o endereço MAC Bluetooth do dispositivo
  uint8_t bt_addr[6];
  esp_read_mac(bt_addr, ESP_MAC_BT);
  char addr_str[18];
  sprintf(addr_str, "%02X:%02X:%02X:%02X:%02X:%02X", bt_addr[0], bt_addr[1], bt_addr[2], bt_addr[3], bt_addr[4], bt_addr[5]);
  Serial.println(addr_str);

}

float leitura() {

  // Lê os dados do acelerometro
  float ax = myIMU.readFloatAccelX()/ 16384.0;
  float ay = myIMU.readFloatAccelY()/ 16384.0;
  float az = myIMU.readFloatAccelZ()/ 16384.0;

  // Lê os dados do giroscópio
  float gx = myIMU.readFloatGyroX()* 0.0175;
  float gy = myIMU.readFloatGyroY()* 0.0175;
  float gz = myIMU.readFloatGyroZ()* 0.0175;
  
  //Get all parameters
  // Serial.print("\nAccelerometer:\n");
  // Serial.print(" X = ");
  // Serial.println(myIMU.readFloatAccelX(), 4);
  // Serial.print(" Y = ");
  // Serial.println(myIMU.readFloatAccelY(), 4);
  // Serial.print(" Z = ");
  // Serial.println(myIMU.readFloatAccelZ(), 4);

  // // Exibe os dados no monitor serial
  // Serial.print("\nGyroscope:\n");
  // Serial.print("X = ");
  // Serial.println(gx, 4);
  // Serial.print(" | Y = ");
  // Serial.println(gy, 4);
  // Serial.print(" | Z = ");
  // Serial.println(gz, 4);

  // Serial.print("\nThermometer:\n");
  // Serial.print(" Degrees C = ");
  // Serial.println(myIMU.readTempC(), 4);
  // Serial.print(" Degrees F = ");
  // Serial.println(myIMU.readTempF(), 4);


  float Roll = atan2(ay, az) * 180 / PI; // Calculando a inclinação no eixo X (roll)
  // float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI; // Calculando a inclinação no eixo Y (pitch)
  // float yaw = gz; // Utilizando a leitura do giroscópio para o ângulo de rotação em torno do eixo Z (yaw)

  //deacordo com o https://akjournals.com/view/journals/606/18/1/article-p167.xml
  //angulo de 15 para erro
  //erro menor de de 5%
  //165 -> 179
  //-165 -> -179
  //ou
  //15 -> 0
  //-15 -> 0

  return Roll;
}

void powerLed(){
  if(!led){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {

  powerLed();

  if(BT.connected()){
    led = true;
    // BT.disconnect();
  }else{
    led = false;
  }
  
  // Serial.print(" Pitch: ");
  // Serial.print(pitch);
  // Serial.print(" Yaw: ");
  // Serial.println(yaw);


  // Serial.print("Roll: ");
  // Serial.print(leitura());
  // Serial.print("\n");
  
  ESC.setEstado(speed);

  pos = 0;
  ServoF.setEstado(pos);
  ServoT.setEstado(pos);
  delay(150);

  pos = 180;
  ServoF.setEstado(pos);
  ServoT.setEstado(pos);
  delay(150);

}
