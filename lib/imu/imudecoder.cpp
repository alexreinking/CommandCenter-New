#include "imudecoder.h"
#include <cmath>
#include <cstdlib>
#include <sys/time.h>

#define PI 3.1415926535897932384
#define IMU_TAG "VNYMR,"

IMUDecoder::IMUDecoder()
{
    for(int i = 0; i < 12; i++)
        nmeaIndices[i] = i;
    nmeaDatums[0] = yawString;
    nmeaDatums[1] = pitchString;
    nmeaDatums[2] = rollString;
    nmeaDatums[3] = magnetXString;
    nmeaDatums[4] = magnetYString;
    nmeaDatums[5] = magnetZString;
    nmeaDatums[6] = accelXString;
    nmeaDatums[7] = accelYString;
    nmeaDatums[8] = accelZString;
    nmeaDatums[9] = gyroXString;
    nmeaDatums[10] = gyroYString;
    nmeaDatums[11] = gyroZString;

    initNmea(&nmeaData, IMU_TAG, sizeof(nmeaDatums) / sizeof(sizeof(*nmeaDatums)), nmeaIndices, nmeaDatums);
}

bool IMUDecoder::decodeByte(int8_t newByte)
{
    if(newByte == -1)
        return false;

    bool ret;
    if (ret = parseNmea(&nmeaData, newByte))
    {
        // It parsed! Something happened!
        lastCalcData = currentCalcData;
        convertToCalcData();

        // Time stamp our just converted time!
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        currentCalcData.accel.timestamp = (double)currentTime.tv_sec + (double)currentTime.tv_usec / 1000000;

        calculateIntegratedData();
    }
    return ret;
}

void IMUDecoder::convertToCalcData()
{
    currentCalcData.yaw = strtod(yawString, NULL);
    currentCalcData.pitch = strtod(pitchString, NULL);
    currentCalcData.roll = strtod(rollString, NULL);
    currentCalcData.accel.coordX = strtod(accelXString, NULL);
    currentCalcData.accel.coordY = strtod(accelYString, NULL);
    currentCalcData.accel.coordZ = strtod(accelZString, NULL);
    currentCalcData.angVel.coordX = strtod(gyroXString, NULL);
    currentCalcData.angVel.coordY = strtod(gyroYString, NULL);
    currentCalcData.angVel.coordZ = strtod(gyroZString, NULL);
}

void IMUDecoder::calculateIntegratedData()
{
    rotateCoordinatesBodytoSpace(&currentCalcData.accel, &currentCalcData.spaceAccel, &currentCalcData);
    integrateWithTime(&currentCalcData.spaceAccel, &lastCalcData.spaceAccel, &currentCalcData.spaceVel);
    rotateCoordinatesSpacetoBody(&currentCalcData.spaceVel, &currentCalcData.vel, &currentCalcData);
    integrateWithTime(&currentCalcData.spaceVel, &lastCalcData.spaceVel, &currentCalcData.spacePos);
    rotateCoordinatesSpacetoBody(&currentCalcData.spacePos, &currentCalcData.pos, &currentCalcData);
}

void IMUDecoder::rotateCoordinatesBodytoSpace(Vector3D* vectorIn, Vector3D* vectorOut, ImuCalcData* imuCalcData)
{
    double cosYaw = cos(imuCalcData->yaw);
    double sinYaw = sin(imuCalcData->yaw);
    double cosPitch = cos(imuCalcData->pitch);
    double sinPitch = sin(imuCalcData->pitch);
    double cosRoll = cos(imuCalcData->roll);
    double sinRoll = sin(imuCalcData->roll);
    vectorOut->coordX = cosYaw*(cosPitch*vectorIn->coordX + sinPitch*(sinRoll*vectorIn->coordY + cosRoll*vectorIn->coordZ)) - sinYaw*(cosRoll*vectorIn->coordY - sinRoll*vectorIn->coordZ);
    vectorOut->coordY = sinYaw*(cosPitch*vectorIn->coordX + sinPitch*(sinRoll*vectorIn->coordY + cosRoll*vectorIn->coordZ)) + cosYaw*(cosRoll*vectorIn->coordY - sinRoll*vectorIn->coordZ);
    vectorOut->coordZ = -sinPitch*vectorIn->coordX + cosPitch*(sinRoll*vectorIn->coordY + cosRoll*vectorIn->coordZ);
    vectorOut->timestamp = vectorIn->timestamp;
}

void IMUDecoder::rotateCoordinatesSpacetoBody(Vector3D* vectorIn, Vector3D* vectorOut, ImuCalcData* imuCalcData)
{
    double cosYaw   = cos(imuCalcData->yaw);
    double sinYaw   = sin(imuCalcData->yaw);

    double cosPitch = cos(imuCalcData->pitch);
    double sinPitch = sin(imuCalcData->pitch);

    double cosRoll  = cos(imuCalcData->roll);
    double sinRoll  = sin(imuCalcData->roll);

    vectorOut->coordX =  cosPitch*(cosYaw*vectorIn->coordX + sinYaw*vectorIn->coordY) - sinPitch*vectorIn->coordZ;
    vectorOut->coordY =  cosRoll*(-sinYaw*vectorIn->coordX + cosYaw*vectorIn->coordY) + sinRoll*(sinPitch*(cosYaw*vectorIn->coordX + sinYaw*vectorIn->coordY) + cosPitch*vectorIn->coordZ);
    vectorOut->coordZ = -sinRoll*(-sinYaw*vectorIn->coordX + cosYaw*vectorIn->coordY) + cosRoll*(sinPitch*(cosYaw*vectorIn->coordX + sinYaw*vectorIn->coordY) + cosPitch*vectorIn->coordZ);
    vectorOut->timestamp = vectorIn->timestamp;
}

void IMUDecoder::integrateWithTime(Vector3D* vectorNew, Vector3D* vectorOld, Vector3D* vectorIntegrated)
{
    //trapazoid integration scheme
    vectorIntegrated->coordX = (vectorNew->timestamp - vectorOld->timestamp)
            * (vectorNew->coordX + vectorOld->coordX)/2;

    vectorIntegrated->coordY = (vectorNew->timestamp - vectorOld->timestamp)
            * (vectorNew->coordY + vectorOld->coordY)/2;

    vectorIntegrated->coordZ = (vectorNew->timestamp - vectorOld->timestamp)
            * (vectorNew->coordZ + vectorOld->coordZ)/2;

    vectorIntegrated->timestamp = vectorNew->timestamp;
}

ImuCalcData IMUDecoder::getCurrentData() const
{
    return currentCalcData;
}

void IMUDecoder::printIMUDetails()
{
    printf("Acceleration: X: %f, Y: %f, Z: %f\n",
           currentCalcData.accel.coordX,
           currentCalcData.accel.coordY,
           currentCalcData.accel.coordZ);

    printf("    Velocity: X: %f, Y: %f, Z: %f\n",
           currentCalcData.vel.coordX,
           currentCalcData.vel.coordY,
           currentCalcData.vel.coordZ);

    printf("    Position: X: %f, Y: %f, Z: %f\n",
           currentCalcData.pos.coordX,
           currentCalcData.pos.coordY,
           currentCalcData.pos.coordZ);

    printf("     Heading: X: %f, Y: %f, Z: %f\n",
           currentCalcData.yaw,
           currentCalcData.pitch,
           currentCalcData.roll);
}

