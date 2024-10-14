#ifndef RESERVED_H
#define RESERVED_H

#include <cstdint>

/** CAN Message IDs that are reserved */
enum class ReservedIDs : uint32_t
{
    // Custom Sensor Messages

    StartSwitch, Throttle1Position, Throttle2Position, BrakePressure, RVC, TireRPM, TireTemperature, BMSPercentage,
    BMSTemperature,
    // Motor Messages

    Temperatures1=0x0A0, Temperatures2, Temperatures3, AnalogInputVoltages, DigitalInputStatus, MotorPositionInfo,
    CurrentInfo, VoltageInfo, FluxInfo, InternalVoltages, InternalStates, FaultCodes, TorqueAndTimerInfo,
    ModulationIndex, FirmwareInformation, DiagnosticData, HighSpeed, TorqueCapability,
    // Motor Commands/Response Messages

    ControlCommand=0x0C0, ParameterCommand, ParameterResponse,
    // Health Check Commands/Response Messages

    HealthCheck=200, DCF, DCR, DCT,
    // Other Commands/Response Messages
    Fault, DriveState, DriveMode, ThrottleMin, ThrottleMax,

    // ID for default initializations.
    INVALID=0xFFFFFFFF
};

enum class FaultSources : uint8_t
{
    ThrottleMismatch, Throttle1Zero, Throttle2Zero, BrakeZero, StartFault,
};

enum class DriveModes : uint8_t
{
    Full,
    Endurance,
    SkidPad,
    Party,
    Limp,
};

enum class RVCSubIDs : uint8_t
{
    X_Accel,
    Y_Accel,
    Z_Accel,
    Roll,
    Pitch,
    Yaw,
};

enum class TireSubIDs : uint8_t
{
    FrontLeft,
    FrontRight,
    RearLeft,
    RearRight,
};

#endif //RESERVED_H
