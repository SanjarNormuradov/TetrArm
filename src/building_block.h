#ifndef __BUILDING_BLOCK_AGENT__H
#define __BUILDING_BLOCK_AGENT__H 

#include "enviro.h"

using namespace enviro;
using namespace std;

class buildingBlockController : public Process, public AgentInterface {
    public:
        buildingBlockController() : Process(), AgentInterface() {
            move = false;
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
        }
        void start() {}
        void update() {
            if (move) {
                // cout << "[Building Blocks] Move\n";
                track_velocity(-3, 0, 70);
            } else {
                // cout << "[Building Blocks] Stop\n";
                track_velocity(0, 0);
            }
        }
        void stop() {}

    private:
        bool move;
};

class buildingBlock : public Agent {
    public:
    buildingBlock(json spec, World& world) : Agent(spec, world) {
        add_process(bbCtrl);
    }
    private:
    buildingBlockController bbCtrl;
};

DECLARE_INTERFACE(buildingBlock)

#endif