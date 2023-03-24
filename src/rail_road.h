#ifndef __RAIL_ROAD_AGENT__H
#define __RAIL_ROAD_AGENT__H 

#include "enviro.h"
#include <string>

using namespace enviro;
using namespace std;

class railRoadController : public Process, public AgentInterface {
    public:
        railRoadController() : Process(), AgentInterface() {
            Vx = 0;
            turboSpeed = false;
            // addAgent = false;
        }
        void init() {
            watch("keydown", [this](Event& e) {
                string pressKey = e.value()["key"];
                // cout << "Released Key: ||" << pressKey <<"||" << endl;
                if (pressKey == "ArrowLeft") {
                    // cout << "[Rail Road] MoveLeft\n";
                    if (turboSpeed) {
                        Vx = -vXt;
                    } else {
                        Vx = -vX;
                    }
                }
                else if (pressKey == "ArrowRight") {
                    // cout << "[Object Gripper] MoveRight\n";
                    if (turboSpeed) {
                        Vx = vXt;
                    } else {
                        Vx = vX;
                    }
                }
                else if (pressKey == "Shift") {
                    turboSpeed = true;
                }
            });
            watch("keyup", [this](Event& e) {
                string pressKey = e.value()["key"];
                // cout << "Released Key: ||" << pressKey <<"||" << endl;
                if (pressKey == "ArrowLeft") {
                    Vx = 0;
                }
                else if (pressKey == "ArrowRight") {
                    Vx = 0;
                }
                else if (pressKey == "Shift") {
                    turboSpeed = false;
                }
            });
        }
        void start() {}
        void update() {
            track_velocity(Vx, 0, 70, 10);
        }
        void stop() {}

    private:
        double Vx;
        double const vX = 1.0, vXt = 5.0;
        bool turboSpeed;
};

class railRoad : public Agent {
    public:
        railRoad(json spec, World& world) : Agent(spec, world) {
            add_process(rrCtrl);
            prevent_rotation();
        }
    private:
        railRoadController rrCtrl;
};

DECLARE_INTERFACE(railRoad)

#endif