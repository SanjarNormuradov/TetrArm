#ifndef __TOY_BOX_AGENT__H
#define __TOY_BOX_AGENT__H 

#include "enviro.h"

using namespace enviro;
using namespace std;

class toyBoxController : public Process, public AgentInterface {
    public:
        toyBoxController() : Process(), AgentInterface() {}
        void init() {
            notice_collisions_with("buildingBlock", [&](Event &e) {
                // cout << "\n[Toy Box] Collision with Building Block detected\n";
                int block_id = e.value()["id"];
                remove_agent(block_id);
                // cout << "\n[Toy Box] Building Block Removed\n";
                emit(Event("Building Block Removed"));
            });
        }
        void start() {}
        void update() {}
        void stop() {}
};

class toyBox : public Agent {
    public:
        toyBox(json spec, World& world) : Agent(spec, world) {
            add_process(tbCtrl);
            prevent_rotation();
            decorate("<rect x='-250' y='-240' width='10' height='241' style='fill: red'/><rect x='240' y='-240' width='10' height='241' style='fill: red'/>");
        }
    private:
        toyBoxController tbCtrl;
};

DECLARE_INTERFACE(toyBox)

#endif