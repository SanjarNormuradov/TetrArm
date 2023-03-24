#ifndef __GRIPPER_CENTER_AGENT__H
#define __GRIPPER_CENTER_AGENT__H 

#include "enviro.h"
#include <string>
#include <cmath>

using namespace enviro;
using namespace std;

class gripperCenterController : public Process, public AgentInterface {
    public:
        gripperCenterController() : Process(), AgentInterface() {
            Vx = 0;
            Vy = 0;
            turboSpeed = false;
        }
        void init() {
            watch("keydown", [this](Event& e) {
                string pressKey = e.value()["key"];
                if (pressKey == "ArrowUp") {
                    // cout << "[Gripper Center]  MoveUp\n";
                    if (turboSpeed) {
                        Vy = -vYt;
                    } else {
                        Vy = -vY;
                    }
                } 
                else if (pressKey == "ArrowDown") {
                    // cout << "[Gripper Center] MoveDown\n";
                    if (turboSpeed) {
                        Vy = vYt;
                    } else {
                        Vy = vY;
                    }
                }
                if (pressKey == "ArrowLeft") {
                    // cout << "[Gripper Center] MoveLeft\n";
                    if (turboSpeed) {
                        Vx = -vXt;
                    } else {
                        Vx = -vX;
                    }
                }
                else if (pressKey == "ArrowRight") {
                    // cout << "[Gripper Center] MoveRight\n";
                    if (turboSpeed) {
                        Vx = vXt;
                    } else {
                        Vx = vX;
                    }
                }
                if (pressKey == "Shift") {
                    turboSpeed = true;
                }
            });
            watch("keyup", [this](Event& e) {
                string pressKey = e.value()["key"];
                if (pressKey == "ArrowUp") {
                    Vy = 0;
                } 
                else if (pressKey == "ArrowDown") {
                    Vy = 0;
                }
                if (pressKey == "ArrowLeft") {
                    Vx = 0;
                }
                else if (pressKey == "ArrowRight") {
                    Vx = 0;
                }
                if (pressKey == "Shift") {
                    turboSpeed = false;
                }
            });
            notice_collisions_with("objectGripper", [&](Event &e) {
                // cout << "\n[Gripper Center] Collision with objectGripper detected\n";
                int object_gripper_id = e.value()["id"];
                // cout << "[Gripper Center] Find Object Gripper...\n";
                Agent& object_gripper = find_agent(object_gripper_id); 
                attach_to(object_gripper);
                // cout << "[Gripper Center] Gripper Attached to Object Gripper\n";
                ignore_collisions_with("objectGripper");
            });
        }
        void start() {}
        void update() {
            omni_track_velocity(Vx, Vy, 70);
        }
        void stop() {}

    private:
        double Vx, Vy;
        double const vX = 1.0, vY = 1.0, vXt = 5.0, vYt = 5.0;
        bool turboSpeed;
};

class gripperCenter : public Agent {
    public:
        gripperCenter(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
    private:
        gripperCenterController c;
};

DECLARE_INTERFACE(gripperCenter)

#endif