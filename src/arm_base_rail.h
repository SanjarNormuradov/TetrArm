#ifndef __ARM_BASE_RAIL_AGENT__H
#define __ARM_BASE_RAIL_AGENT__H 

#include "enviro.h"

using namespace enviro;

class armBaseRailController : public Process, public AgentInterface {

    public:
    armBaseRailController() : Process(), AgentInterface() {}

    void init() {
        prevent_rotation();
    }
    void start() {}
    void update() {}
    void stop() {}

};

class armBaseRail : public Agent {
    public:
    armBaseRail(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    armBaseRailController c;
};

DECLARE_INTERFACE(armBaseRail)

#endif