#include "dashboard_driver.hpp"
#include "ur_rtde/dashboard_client.h"

#include <asynOctetSyncIO.h>
#include <epicsExport.h>
#include <epicsString.h>
#include <epicsThread.h>
#include <iocsh.h>
#include <iostream>

static constexpr int BUFF_SIZE = 100;

static void main_loop_thread_C(void *pPvt) {
    URRobotDashboard *pURRobotDashboard = (URRobotDashboard *)pPvt;
    pURRobotDashboard->main_loop();
}

URRobotDashboard::URRobotDashboard(const char *asyn_port_name,
                                   const char *robot_ip)
    : asynPortDriver(asyn_port_name, MAX_CONTROLLERS,
                     asynInt32Mask | asynFloat64Mask | asynDrvUserMask |
                         asynOctetMask | asynInt32ArrayMask,
                     asynInt32Mask | asynFloat64Mask | asynOctetMask |
                         asynInt32ArrayMask,
                     ASYN_MULTIDEVICE | ASYN_CANBLOCK,
                     1, /* ASYN_CANBLOCK=0, ASYN_MULTIDEVICE=1, autoConnect=1 */
                     0, 0),
      ur_dashboard_(std::make_unique<ur_rtde::DashboardClient>(robot_ip)),
      poll_time_(DEFAULT_POLL_TIME) {

    // create asyn parameters
    createParam(IS_CONNECTED_STRING, asynParamInt32, &isConnectedIndex_);
    createParam(LOAD_URP_STRING, asynParamOctet, &loadURPIndex_);
    createParam(PLAY_STRING, asynParamInt32, &playIndex_);
    createParam(STOP_STRING, asynParamInt32, &stopIndex_);
    createParam(PAUSE_STRING, asynParamInt32, &pauseIndex_);
    createParam(QUIT_STRING, asynParamInt32, &quitIndex_);
    createParam(SHUTDOWN_STRING, asynParamInt32, &shutdownIndex_);
    createParam(IS_RUNNING_STRING, asynParamInt32, &isRunningIndex_);
    createParam(CLOSE_POPUP_STRING, asynParamInt32, &closePopupIndex_);
    createParam(POPUP_STRING, asynParamOctet, &popupIndex_);

    // connect to the UR dashboard server
    ur_dashboard_->connect();
    if (ur_dashboard_->isConnected()) {
        std::cout << "Connected to dashboard server" << std::endl;
        setIntegerParam(isConnectedIndex_, 1);
    } else {
        setIntegerParam(isConnectedIndex_, 0);
        throw std::runtime_error("Failed to connect to UR dashboard server");
    }

    std::cout << "starting main loop..." << std::endl;
    epicsThreadCreate("UrRobotMainLoop", epicsThreadPriorityLow,
                      epicsThreadGetStackSize(epicsThreadStackMedium),
                      (EPICSTHREADFUNC)main_loop_thread_C, this);
}

void URRobotDashboard::main_loop() {
    char buffer[BUFF_SIZE];
    int trigger = 0;
    while (true) {
        lock();

        if (ur_dashboard_->isConnected()) {
            setIntegerParam(isConnectedIndex_, 1);
            
            // check if program is running
            if (ur_dashboard_->running()) {
                setIntegerParam(isRunningIndex_, 1);
            }
            else {
                setIntegerParam(isRunningIndex_, 0);
            }


            // Load program
            getStringParam(loadURPIndex_, BUFF_SIZE, buffer);
            if (strlen(buffer) > 0) {
                std::cout << "loading program " << buffer << ".urp"
                          << std::endl;
                setStringParam(loadURPIndex_, "");
                // ur_dashboard_->loadURP(buffer);
            }

            // Play loaded program
            getIntegerParam(playIndex_, &trigger);
            if (trigger != 0) {
                std::cout << "Playing loaded program" << std::endl;
                setIntegerParam(playIndex_, 0);
                // ur_dashboard_->play();
            }

            // Stop program
            getIntegerParam(stopIndex_, &trigger);
            if (trigger != 0) {
                std::cout << "Stopping program" << std::endl;
                setIntegerParam(stopIndex_, 0);
                // ur_dashboard_->stop();
            }
            
            // Pause program
            getIntegerParam(pauseIndex_, &trigger);
            if (trigger != 0) {
                std::cout << "Pausing program" << std::endl;
                setIntegerParam(pauseIndex_, 0);
                // ur_dashboard_->pause();
            }
            
            // Quit
            getIntegerParam(quitIndex_, &trigger);
            if (trigger != 0) {
                std::cout << "Closing connection to dashboard server" << std::endl;
                setIntegerParam(quitIndex_, 0);
                // ur_dashboard_->quit();
            }
            
            // Shutdown
            getIntegerParam(shutdownIndex_, &trigger);
            if (trigger != 0) {
                std::cout << "Shutting down robot and controller" << std::endl;
                setIntegerParam(shutdownIndex_, 0);
                // ur_dashboard_->shutdown();
            }

            // Close popup
            getIntegerParam(closePopupIndex_, &trigger);
            if (trigger != 0) {
                std::cout << "Closing popup" << std::endl;
                setIntegerParam(closePopupIndex_, 0);
                ur_dashboard_->closePopup();
            }

            // Popup message
            getStringParam(popupIndex_, BUFF_SIZE, buffer);
            if (strlen(buffer) > 0) {
                std::cout << "Popup text = " << buffer << std::endl;
                setStringParam(popupIndex_, "");
                ur_dashboard_->popup(buffer);
            }


        } else {
            setIntegerParam(isConnectedIndex_, 0);
        }

        callParamCallbacks();
        unlock();
        epicsThreadSleep(poll_time_);
    }
}

// register function for iocsh
extern "C" int URRobotDashboardConfig(const char *asyn_port_name,
                                      const char *robot_ip) {
    URRobotDashboard *pURRobotDashboard =
        new URRobotDashboard(asyn_port_name, robot_ip);
    pURRobotDashboard = NULL;
    return (asynSuccess);
}

static const iocshArg urRobotArg0 = {"Asyn port name", iocshArgString};
static const iocshArg urRobotArg1 = {"Robot IP address", iocshArgString};
static const iocshArg *const urRobotArgs[2] = {&urRobotArg0, &urRobotArg1};
static const iocshFuncDef urRobotFuncDef = {"URRobotDashboardConfig", 2,
                                            urRobotArgs};

static void urRobotCallFunc(const iocshArgBuf *args) {
    URRobotDashboardConfig(args[0].sval, args[1].sval);
}

void URRobotDashboardRegister(void) {
    iocshRegister(&urRobotFuncDef, urRobotCallFunc);
}

extern "C" {
epicsExportRegistrar(URRobotDashboardRegister);
}
