#ifndef _RTDE_CONTROL_DRIVER_HPP_
#define _RTDE_CONTROL_DRIVER_HPP_

#include "ur_rtde/rtde_control_interface.h"
#include "ur_rtde/rtde_receive_interface.h"
#include <asynPortDriver.h>

static constexpr char DISCONNECT_STRING[] = "DISCONNECT";
static constexpr char RECONNECT_STRING[] = "RECONNECT";
static constexpr char IS_CONNECTED_STRING[] = "IS_CONNECTED";

static constexpr char J1CMD_STRING[] = "J1CMD";
static constexpr char J2CMD_STRING[] = "J2CMD";
static constexpr char J3CMD_STRING[] = "J3CMD";
static constexpr char J4CMD_STRING[] = "J4CMD";
static constexpr char J5CMD_STRING[] = "J5CMD";
static constexpr char J6CMD_STRING[] = "J6CMD";

static constexpr char MOVEJ_STRING[] = "MOVEJ";

static constexpr int NUM_JOINTS = 6;
static constexpr int MAX_CONTROLLERS = 1;
static constexpr double POLL_PERIOD = 0.02; // seconds
static constexpr double DEFAULT_CONTROLLER_TIMEOUT = 1.0;

struct JointStates {
    double J1 = 0.0;
    double J2 = 0.0;
    double J3 = 0.0;
    double J4 = 0.0;
    double J5 = 0.0;
    double J6 = 0.0;
};

struct Pose {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double roll = 0.0;
    double pitch = 0.0;
    double yaw = 0.0;
};

class RTDEControl : public asynPortDriver {
  public:
    RTDEControl(const char *asyn_port_name, const char *robot_port_name);
    virtual void poll(void);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);

  private:
    std::unique_ptr<ur_rtde::RTDEControlInterface> rtde_control_;
    std::unique_ptr<ur_rtde::RTDEReceiveInterface> rtde_receive_;
    const char *robot_ip_;
    bool try_connect();
    JointStates joint_cmds;

  protected:
    asynUser *pasynUserURRobot_;

    int disconnectIndex_;
    int reconnectIndex_;
    int isConnectedIndex_;
    int J1CmdIndex_;
    int J2CmdIndex_;
    int J3CmdIndex_;
    int J4CmdIndex_;
    int J5CmdIndex_;
    int J6CmdIndex_;
    int moveJIndex_;
};

#endif
