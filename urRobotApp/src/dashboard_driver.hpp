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
static constexpr char QUIT_STRING[] = "QUIT";
static constexpr char SHUTDOWN_STRING[] = "SHUTDOWN";
static constexpr char IS_RUNNING_STRING[] = "IS_RUNNING";
static constexpr char CLOSE_SAFETY_POPUP_STRING[] = "CLOSE_SAFETY_POPUP";
static constexpr char POWER_ON_STRING[] = "POWER_ON";
static constexpr char POWER_OFF_STRING[] = "POWER_OFF";
static constexpr char BRAKE_RELEASE_STRING[] = "BRAKE_RELEASE";
static constexpr char UNLOCK_PROTECTIVE_STOP_STRING[] = "UNLOCK_PROTECTIVE_STOP";
static constexpr char RESTART_SAFETY_STRING[] = "RESTART_SAFETY";
static constexpr char POLYSCOPE_VERSION_STRING[] = "POLYSCOPE_VERSION";
static constexpr char GET_SERIAL_NUMBER_STRING[] = "SERIAL_NUMBER";

constexpr int MAX_CONTROLLERS = 1;
constexpr double DEFAULT_POLL_TIME = 0.20;
constexpr double DEFAULT_CONTROLLER_TIMEOUT = 1.0;

class URRobotDashboard : public asynPortDriver {
  public:
    URRobotDashboard(const char *asyn_port_name, const char *robot_port_name);
    virtual void main_loop(void);

  private:
    std::unique_ptr<ur_rtde::DashboardClient> ur_dashboard_;
    double poll_time_;

  protected:
    asynUser *pasynUserURRobot_;
    int isConnectedIndex_;
    int closePopupIndex_;
    int popupIndex_;
    int loadURPIndex_;
    int playIndex_;
    int stopIndex_;
    int pauseIndex_;
    int quitIndex_;
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
};
