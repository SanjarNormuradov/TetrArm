#ifndef __OBJECT_GRIPPER_AGENT__H
#define __OBJECT_GRIPPER_AGENT__H 

#include "enviro.h"
#include <string>

using namespace enviro;
using namespace std;

class objectGripperController : public Process, public AgentInterface {
    public:
        objectGripperController() : Process(), AgentInterface() {
            gripped = false;
        }
        void init() {
            notice_collisions_with("gripperCenter", [&](Event &e) {
                // cout << "\n[Object Gripper] Collision with gripperCenter detected\n";
                int gripper_center_id = e.value()["id"];
                // cout << "[Object Gripper] Find Gripper Center...\n";
                Agent& gripper_center = find_agent(gripper_center_id); 
                attach_to(gripper_center);
                // cout << "[Object Gripper] Gripper Attached to Gripper Center\n";
                ignore_collisions_with("gripperCenter");
            });
            notice_collisions_with("buildingBlock", [&](Event &e) {
                // cout << "\n[Object Gripper] Collision with buildingBlock detected\n";
                if (gripped) {
                    int block_id = e.value()["id"];
                    Agent& block = find_agent(block_id); 
                    attach_to(block);
                    // cout << "\n[Object Gripper] Gripper Attached to Block\n";
                    emit(Event("Block Gripped", {
                        {"block_id", block_id}
                    }));
                }
            });
            watch("Building Block Removed", [this](Event& e) {
                gripped = false;
            });
            watch("keydown", [this](Event& e) {
                string pressKey = e.value()["key"];
                if (pressKey == " ") {
                    gripped = true;
                }
                if (pressKey == "ArrowLeft") {
                    if (turboSpeed) {
                        Vx = -vXt;
                    } else {
                        Vx = -vX;
                    }
                }
                else if (pressKey == "ArrowRight") {
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
        }
        void start() {}
        void update() {
            track_velocity(Vx, 0, 70, 10);
        }
        void stop() {}

    private:
        bool gripped, turboSpeed;
        double Vx;
        double const vX = 1.0, vXt = 5.0;
};

class objectGripper : public Agent {
    public:
        objectGripper(json spec, World& world) : Agent(spec, world) {
            add_process(ogCtrl);
            // decorate("<rect x='-26' y='9' width='10' height='17' style='fill: grey'/><rect x='16' y='9' width='10' height='17' style='fill: grey'/>");
            prevent_rotation();
        }
    private:
        objectGripperController ogCtrl;
};

DECLARE_INTERFACE(objectGripper)

#endif