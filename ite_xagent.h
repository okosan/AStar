#pragma once

/*


*/

enum VOLATILE_FLAG
{
    freeDispatch = 0,
    waitInsideActionGroup,
    waitGlobally, // wait until this message will process
    lockUntilPerformed
}

enum PRIORITY_LEVEL
{
    PRIORITY_LOW,
    NORMAL,
    HIGH,
    CRITICAL
}


class XAgent
{
public:
    XAgent();
    ~XAgent();

    int init();

    // 
    int retreiveStereoImage();
    int setCameraPos(float tilt_,float yaw_,float roll_);

    float getCurrentCameraTilt();
    float getCurrentCameraYaw();

    int addTask(); // add active task to task list dispatcher. returns task manipulator - Handle

    int moveTo(void* posClass123);

    int setEventHandler(void * handler); // receive messages about completed tasks

    int rotateWheel(int index,float deg,int priority,bool isVolatile); // volatile - wait until this message will pass

    int setMoveParameters(int speed,int direction_vector,int parameters_power);

    int getMoveSpeed();
    int getMoveDirection();

    int readAccelerometerData(float &ax,float &ay,float &az);
    int readGyroData(float &rx,float &ry,float &rz);

    int sleepFor(float time_ms);


private:


}


// real agent will communicate with controller,rotate camera,move wheels,get wheel speed,etc
// virtual agent will create 3d map,render to texture and give it as camera,move by map with physics emulation