#include <Arduino.h>
#include "./BufferPacker.ino"

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println("Default Packing Example: ");
    defaultPackingExample();
    Serial.println();
    Serial.println("Custom Packing Example: ");
    customPackingExample();
    Serial.println();
    Serial.println("Custom Plain-Old-Data Example: ");
    customPlainOldDataExample();
    Serial.println();
    Serial.println("Heap Ownership Example: ");
    heapOwnershipExample();
    Serial.println();
    Serial.println("Reset Packer with new Values Example: ");
    resetValuesExample();
    Serial.println();
    Serial.println("Reset Packer with new Buffer Example: ");
    resetBufferExample();
    Serial.println();
    Serial.println("Buffer Protection Example: ");
    bufferProtectionExample();
    Serial.println();
    delay(10000);
}
