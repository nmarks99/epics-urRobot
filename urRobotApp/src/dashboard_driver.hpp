#ifndef _DASHBOARD_DRIVER_HPP_
#define _DASHBOARD_DRIVER_HPP_

#include "ur_rtde/dashboard_client.h"
#include <asynPortDriver.h>

// Dashboard interface
static constexpr char IS_CONNECTED_STRING[] = "IS_CONNECTED";
static constexpr char CLOSE_POPUP_STRING[] = "CLOSE_POPUP";
static constexpr char POPUP_STRING[] = "POPUP";
static constexpr char LOAD_URP_STRING[] = "LOAD_URP";
static constexpr char PLAY_STRING[] = "PLAY";
static constexpr char STOP_STRING[] = "STOP";
static constexpr char PAUSE_STRING[] = "PAUSE";
static constexpr char CONNECT_STRING[] = "CONNECT";
static constexpr char DISCONNECT_STRING[] = "DISCONNECT";
static constexpr char SHUTDOWN_STRING[] = "SHUTDOWN";
static constexpr char IS_RUNNING_STRING[] = "IS_RUNNING";
static constexpr char CLOSE_SAFETY_POPUP_STRING[] = "CLOSE_SAFETY_POPUP";
static constexpr char POWER_ON_STRING[] = "POWER_ON";
static constexpr char POWER_OFF_STRING[] = "POWER_OFF";
static constexpr char BRAKE_RELEASE_STRING[] = "BRAKE_RELEASE";
static constexpr char UNLOCK_PROTECTIVE_STOP_STRING[] = "UNLOCK_PROTECTIVE_STOP";
static constexpr char RESTART_SAFETY_STRING[] = "RESTART_SAFETY";
static constexpr char POLYSCOPE_VERSION_STRING[] = "POLYSCOPE_VERSION";
static constexpr char SERIAL_NUMBER_STRING[] = "SERIAL_NUMBER";
static constexpr char ROBOT_MODE_STRING[] = "ROBOT_MODE";
static constexpr char PROGRAM_STATE_STRING[] = "PROGRAM_STATE";
static constexpr char ROBOT_MODEL_STRING[] = "ROBOT_MODEL";
static constexpr char LOADED_PROGRAM_STRING[] = "LOADED_PROGRAM";
static constexpr char SAFETY_STATUS[] = "SAFETY_STATUS";
static constexpr char IS_PROGRAM_SAVED[] = "IS_PROGRAM_SAVED";
static constexpr char IS_IN_REMOTE_CONTROL[] = "IS_IN_REMOTE_CONTROL";

constexpr int MAX_CONTROLLERS = 1;
constexpr double DEFAULT_POLL_TIME = 0.10; // seconds
constexpr double DEFAULT_CONTROLLER_TIMEOUT = 1.0;

class URDashboard : public asynPortDriver {
  public:
    URDashboard(const char *asyn_port_name, const char *robot_port_name);
    virtual void poll(void);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars,
                                  size_t *nActual);

  private:
    std::unique_ptr<ur_rtde::DashboardClient> ur_dashboard_;
    double poll_time_;
    bool try_connect();

  protected:
    asynUser *pasynUserURRobot_;
    int isConnectedIndex_;
    int closePopupIndex_;
    int popupIndex_;
    int loadURPIndex_;
    int playIndex_;
    int stopIndex_;
    int pauseIndex_;
    int connectIndex_;
    int disconnectIndex_;
    int shutdownIndex_;
    int isRunningIndex_;
    int closeSafetyPopupIndex_;
    int powerOnIndex_;
    int powerOffIndex_;
    int brakeReleaseIndex_;
    int unlockProtectiveStopIndex_;
    int restartSafetyIndex_;
    int polyscopeVersionIndex_;
    int serialNumberIndex_;
    int robotModeIndex_;
    int programStateIndex_;
    int robotModelIndex_;
    int loadedProgramIndex_;
    int safetyStatusIndex_;
    int isProgramSavedIndex_;
    int isInRemoteControlIndex_;
};

#endif
