#include <Arduino.h>
#include "BufferPacker.h"

template <typename T1, typename T2>
void printComparison(T1 PackValue, T2 UnpackValue)
{
    Serial.print("Pack Value: ");
    Serial.print(PackValue);
    Serial.print(PackValue == UnpackValue ? " == " : " != ");
    Serial.print("Unpack Value: ");
    Serial.print(UnpackValue);
    Serial.print(" ");
    Serial.println(PackValue == UnpackValue ? "PASSED" : "FAILED");
}

void defaultPackingExample()
{
    // Values to pack
    constexpr uint16_t packValue1 = 15;
    constexpr float packValue2 = 16.0f;
    constexpr int16_t packValue3 = -5;

    // Size of data to store
    constexpr size_t BUFFER_SIZE = sizeof(uint16_t) + sizeof(float) + sizeof(int16_t); // 7 on most architectures

    // Buffer to copy results into
    uint8_t packBuffer[BUFFER_SIZE] {};

    // Packer with default initialization (PACK mode, size 8)
    auto packer = BufferPacker();

    // Packing operations
    packer.pack(packValue1);
    packer.pack(packValue2);
    packer.pack(packValue3);

    // Copy operation
    packer.deepCopyTo(packBuffer);

    // Unpacker with default initialization (UNPACK mode, size 8)
    auto unpacker = BufferPacker(packBuffer);

    // Unpacking operations
    const auto unpackValue1 = unpacker.unpack<uint16_t>();
    const auto unpackValue2 = unpacker.unpack<float>();
    const auto unpackValue3 = unpacker.unpack<int16_t>();

    // Print results
    printComparison(packValue1, unpackValue1);
    printComparison(packValue2, unpackValue2);
    printComparison(packValue3, unpackValue3);
}

void customPackingExample()
{
    // Values to pack
    constexpr int64_t packValue1 = 1512340971234;
    constexpr float packValue2 = 16.21345f;
    constexpr int8_t packValue3 = -5;

    // Size of data to store
    constexpr size_t BUFFER_SIZE = sizeof(int64_t) + sizeof(float) + sizeof(double); // 17 on most architectures

    // Buffer to copy results into
    uint8_t packBuffer[BUFFER_SIZE] {};

    // Packer with non-default initialization (PACK mode, size 17)
    auto packer = BufferPacker<BUFFER_SIZE>();

    // Packing operations
    packer.pack(packValue1);
    packer.pack(packValue2);
    packer.pack(packValue3);

    // Copy operation
    packer.deepCopyTo(packBuffer);

    // Unpacker with default initialization (UNPACK mode, size 17)
    auto unpacker = BufferPacker<BUFFER_SIZE>(packBuffer);

    // Unpacking operations
    const auto unpackValue1 = unpacker.unpack<int64_t>();
    const auto unpackValue2 = unpacker.unpack<float>();
    const auto unpackValue3 = unpacker.unpack<double>();

    // Print results
    printComparison(packValue1, unpackValue1);
    printComparison(packValue2, unpackValue2);
    printComparison(packValue3, unpackValue3);
}

void customPlainOldDataExample()
{
    // Custom type to pack - shouldn't have pointers or custom copy/move constructors
    struct POD
    {
        float x;
        float y;
        float z;
    };

    // Value to pack
    constexpr POD packValue1 = { 1.3f, -10.5555f, 0.0234f};

    // Size of data to store
    constexpr size_t BUFFER_SIZE = sizeof(POD); // 12 on most architectures

    // Buffer to copy results into
    uint8_t packBuffer[BUFFER_SIZE] {};

    // Packer with non-default initialization (PACK mode, size 12)
    auto packer = BufferPacker<BUFFER_SIZE>();

    // Packing operation
    packer.pack(packValue1);

    // Copy operation
    packer.deepCopyTo(packBuffer);

    // Unpacker with default initialization (UNPACK mode, size 8)
    auto unpacker = BufferPacker<BUFFER_SIZE>(packBuffer);

    // Unpacking operation
    const auto unpackValue1 = unpacker.unpack<POD>();

    // Print results
    printComparison(packValue1.x, unpackValue1.x);
    printComparison(packValue1.y, unpackValue1.y);
    printComparison(packValue1.z, unpackValue1.z);
}

void heapOwnershipExample()
{
    // Values to pack
    constexpr uint8_t packValue1 = 15;
    constexpr uint8_t packValue2 = 10;
    constexpr uint8_t packValue3 = 5;
    constexpr uint8_t packValue4 = 0;

    constexpr uint8_t packValueCheck[4] = {packValue1, packValue2, packValue3, packValue4};

    // Packer with default initialization (PACK mode, size 8)
    auto packer = BufferPacker();

    // Packing operations
    packer.pack(packValue1);
    packer.pack(packValue2);
    packer.pack(packValue3);
    packer.pack(packValue4);

    // Heap-allocated buffer extraction - heapBuffer is the owner of this pointer
    const uint8_t* heapBuffer = packer.getOwnedHeapBuffer();

    // Getter for bufferSize
    const size_t bufferSize = packer.getBufferSize();

    // Example for loop
    for (size_t i = 0; i < bufferSize; i++) {}

    // Printing results
    printComparison(packValueCheck[0], heapBuffer[0]);
    printComparison(packValueCheck[1], heapBuffer[1]);
    printComparison(packValueCheck[2], heapBuffer[2]);
    printComparison(packValueCheck[3], heapBuffer[3]);
}

void resetValuesExample()
{
    // Values to pack
    uint16_t packValue1 = 15;
    float packValue2 = 18.0f;
    int16_t packValue3 = -5;

    // Size of data to store
    constexpr size_t BUFFER_SIZE = sizeof(uint16_t) + sizeof(float) + sizeof(int16_t); // 7 on most architectures

    // Buffer to copy results into
    uint8_t packBuffer[BUFFER_SIZE] {};

    // Packer with default initialization (PACK mode, size 8)
    auto packer = BufferPacker();

    // Packing operations
    packer.pack(packValue1);
    packer.pack(packValue2);
    packer.pack(packValue3);

    // Change in values which would require a reset()
    packValue1 = 14;
    packValue2 = 17.0f;
    packValue3 = -6;

    // Resetting the packers internal buffer for 'PACK' mode - using the clearBuffer flag will 0 out buffer
    packer.reset(true);

    // Packing operations
    packer.pack(packValue1);
    packer.pack(packValue2);
    packer.pack(packValue3);

    // Copy operation
    packer.deepCopyTo(packBuffer);

    // Unpacker with default initialization (UNPACK mode, size 8)
    auto unpacker = BufferPacker(packBuffer);

    // Unpacking operations
    const auto unpackValue1 = unpacker.unpack<uint16_t>();
    const auto unpackValue2 = unpacker.unpack<float>();
    const auto unpackValue3 = unpacker.unpack<int16_t>();

    // Print results
    printComparison(packValue1, unpackValue1);
    printComparison(packValue2, unpackValue2);
    printComparison(packValue3, unpackValue3);
}

void resetBufferExample()
{
    // Buffer of bytes
    uint8_t resetBuffer[3] = {10, 5, 10};

    // unpacker with default initialization
    auto unpacker = BufferPacker(resetBuffer);

    constexpr uint8_t newValue1 = 9;
    constexpr uint8_t newValue2 = 4;
    constexpr uint8_t newValue3 = 9;

    // Change the values of the buffer
    resetBuffer[0] = newValue1;
    resetBuffer[1] = newValue2;
    resetBuffer[2] = newValue3;

    // Reset operation with the new buffer
    unpacker.reset(resetBuffer);

    // Unpack operations with newly reset buffer
    const auto unpackValue1 = unpacker.unpack<uint8_t>();
    const auto unpackValue2 = unpacker.unpack<uint8_t>();
    const auto unpackValue3 = unpacker.unpack<uint8_t>();

    // Print results
    printComparison(newValue1, unpackValue1);
    printComparison(newValue2, unpackValue2);
    printComparison(newValue3, unpackValue3);
}

void bufferProtectionExample()
{
    // These are all BAD examples - don't do these and you'll be fine!
    // Use if(BufferPacker) or static_cast<bool>(BufferPacker) to check if there was a failure on an operation

    uint8_t largeBuffer[50] = {};
    auto tooSmallPacker = BufferPacker<1>(largeBuffer);
    if (tooSmallPacker)
    {
        Serial.println("Buffer Packer Should've Failed on Construction Overflow - FAILED");
    } else
    {
        Serial.println("Buffer Packer Failed Safely - No Construction Overflow - PASSED");
    }

    // Reset for easy demonstration
    tooSmallPacker.reset(true);

    constexpr int64_t bigData = 50;
    tooSmallPacker.pack(bigData);
    if (tooSmallPacker)
    {
        Serial.println("Buffer Packer Should've Failed on pack() Overflow - FAILED");
    } else
    {
        Serial.println("Buffer Packer Failed Safely - No pack() Overflow - PASSED");
    }

    uint8_t tooSmallBuffer[1] = {};
    auto bigPacker = BufferPacker<50>();
    bigPacker.deepCopyTo(tooSmallBuffer);
    if (bigPacker)
    {
        Serial.println("Buffer Packer Should've Failed on deepCopyTo() Overflow - FAILED");
    } else
    {
        Serial.println("Buffer Packer Failed Safely - No deepCopyTo() Overflow - PASSED");
    }

    uint8_t smallBuffer[1] = {1};
    auto smallPacker = BufferPacker<1>(smallBuffer);
    smallPacker.unpack<int64_t>();
    if (smallPacker)
    {
        Serial.println("Buffer Packer Should've Failed on unpack() Overread - FAILED");
    } else
    {
        Serial.println("Buffer Packer Failed Safely - No unpack() Overread - PASSED");
    }

    // Reset for easy demonstration
    smallPacker.reset(smallBuffer);
    smallPacker.skip<int64_t>();
    if (smallPacker)
    {
        Serial.println("Buffer Packer Should've Failed on skip() Overread - FAILED");
    } else
    {
        Serial.println("Buffer Packer Failed Safely - No skip() Overread - PASSED");
    }
}