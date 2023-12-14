//importação das bibliotecas
#include <Arduino.h>
#include "NimBLEDevice.h"

//definição dos uuid's
#define SERVICE_UUID "****"
#define CHARACTERISTIC_UUID "****"

//declaração da característica bluetooth
NimBLECharacteristic *pCharacteristic;

const int electrodePin = 35;

void setup()
{
  Serial.begin(9600);

  //início do servidor bluetooth
  NimBLEDevice::init("Bruxismo");

  NimBLEServer *pServer = NimBLEDevice::createServer();
  NimBLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

  pService->start();
  pCharacteristic->setValue("0.0");

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("Anunciando... Aguardando conexão!");
}

void loop()
{
  // Lendo o sinal
  double sensorValue = analogRead(electrodePin);
  //Serial.print(sensorValue);

  //Serial.print("\t|");

  double outputValue = sensorValue * (3.3 / 4095.0) * 1000;
  //Serial.print(outputValue);

  //Serial.print("mV\n");

  // Transmitindo o sinal
  String val = String(outputValue, 1);
  pCharacteristic->setValue((uint8_t *)val.c_str(), val.length());
  pCharacteristic->notify(true);

  // Taxa de amostragem de 20Hz
  delay(50);
}
