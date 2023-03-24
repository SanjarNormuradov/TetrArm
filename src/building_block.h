#ifndef __BUILDING_BLOCK_AGENT__H
#define __BUILDING_BLOCK_AGENT__H 

#include "enviro.h"

using namespace enviro;
using namespace std;

class buildingBlockController : public Process, public AgentInterface {
    public:
        buildingBlockController() : Process(), AgentInterface() {
            move = false;
            gripped = false;
            Vx = 0;
        }
        void init() {
            ignore_collisions_with("conveyorBelt");
            watch("Move Blocks", [this](Event& e) {
                // cout << "[Building Blocks] Move Blocks\n";
                move = true;
            });
            watch("Stop Blocks", [this](Event& e) {
                // cout << "[Building Blocks] Stop Blocks\n";
                move = false;
            });
            watch("Block Gripped", [this](Event& e) {
                if (e.value()["block_id"] == id()) {
                    // cout << "[Building Blocks] Block Gripped\n";
                    gripped = true;
                }
            });
            watch("Block Added", [this](Event& e) {
                if (e.value()["block_id"] == id()) {
                    colour = e.value()["colour"];
                    // cout << "[Building Blocks] Block Added. Colour: " << colour << endl;
                }
            });
            notice_collisions_with("objectGripper", [&](Event &e) {
                // cout << "\n[Bulding Block] Collision with objectGripper detected\n";
                if (gripped) {
                    int gripper_id = e.value()["id"];
                    Agent& gripper = find_agent(gripper_id);
                    attach_to(gripper);
                    // cout << "[Bulding Block] Block Attached to Gripper\n";
                    ignore_collisions_with("objectGripper");
                }
            });
            watch("keydown", [this](Event& e) {
                string pressKey = e.value()["key"];
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
                else if (pressKey == "Shift") {
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
                else if (pressKey == "Shift") {
                    turboSpeed = false;
                }
            });
        }
        void start() {}
        void update() {
            if (!gripped) {
                if (move) {
                    // cout << "[Building Blocks] Move\n";
                    track_velocity(-3, 0, 70);
                } else {
                    // cout << "[Building Blocks] Stop\n";
                    track_velocity(0, 0, 70);
                }
            } else {
                // cout << "[Bulding Block] Gripped Block: x = " << x() << "; y = " << y() << endl;
                track_velocity(Vx, 0, 70);
            }
        }
        void stop() {}

    private:
        bool move, turboSpeed, 
             gripped;
        double Vx;
        double const vX = 1.0, vXt = 5.0;
        int box_id;
        string colour;
};

class buildingBlock : public Agent {
    public:
        buildingBlock(json spec, World& world) : Agent(spec, world) {
            add_process(bbCtrl);
            prevent_rotation();
        }
    private:
        buildingBlockController bbCtrl;
};

DECLARE_INTERFACE(buildingBlock)

#endif