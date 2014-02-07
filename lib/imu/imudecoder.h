#ifndef IMU_DECODER
#define IMU_DECODER

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <stdio.h>
#include "nmeaParse.h"
#include <iostream>
#include <fstream>
using namespace std;

typedef struct Vector3D
{
    // In meters
    double coordX;
    double coordY;
    double coordZ;
    // In seconds
    double timestamp;
} Vector3D;

typedef struct ImuCalcData
{
    // In degrees.
    double yaw;
    double pitch;
    double roll;

    // In meters/second^2.
    Vector3D accel;
    Vector3D spaceAccel;

    // In meters/second. An approximation calculated from acceleration.
    Vector3D vel;
    Vector3D spaceVel;

    // In meters. An approximation calculated from acceleration.
    Vector3D pos;
    Vector3D spacePos;

    // ??? comes from built-in gyro
    Vector3D angVel;
} ImuCalcData;

// Decodes bytes obtained from the Inertial Measurement Unit (IMU) so that relevant details may be accessed.
// Made to work with the VectorNav VN-100.
class IMUDecoder
{
public:
    // Initialize a new IMU decoder.
    IMUDecoder();

    // Everything
    ImuCalcData getCurrentData() const;

    // Passes the IMUDecoder an additional byte from the the IMU's output stream to decode.
    // Returns true if the IMUDecoder has updated its parameters.
    bool decodeByte(int8_t newByte);

    // Print Details
    void printIMUDetails();

private:
    // Parses strings to actual doubles, etc...
    void convertToCalcData();

    // Does integration with current and lastCalcData
    void calculateIntegratedData();

    // performs rotation transformations between absolute (based on gyro readings) and relative (to device) coordinates.
    static void rotateCoordinatesBodytoSpace(Vector3D* vectorIn, Vector3D* vectorOut, ImuCalcData* imuCalcData);
    static void rotateCoordinatesSpacetoBody(Vector3D* vectorIn, Vector3D* vectorOut, ImuCalcData* imuCalcData);
    static void integrateWithTime(Vector3D* vectorNew, Vector3D* vectorOld, Vector3D* vectorIntegrated);

    // Calculation type data
    ImuCalcData currentCalcData;
    ImuCalcData lastCalcData;

    // A NMEA parser for each tag type we want to interpret
    NmeaData nmeaData;
    int nmeaIndices[12];
    char* nmeaDatums[12];

    // Directly parsed data
    char yawString[10];
    char pitchString[10];
    char rollString[10];
    char magnetXString[10];
    char magnetYString[10];
    char magnetZString[10];
    char accelXString[10];
    char accelYString[10];
    char accelZString[10];
    char gyroXString[10];
    char gyroYString[10];
    char gyroZString[10];
};

#endif
