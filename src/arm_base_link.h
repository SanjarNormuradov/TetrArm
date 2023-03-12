#ifndef __ARM_BASE_LINK_AGENT__H
#define __ARM_BASE_LINK_AGENT__H 

#include "enviro.h"
#include <iostream>

using namespace enviro;

class armBaseLinkController : public Process, public AgentInterface {

    public:
    armBaseLinkController() : Process(), AgentInterface() {}

    void init() {
        decorate("<rect x='-30' y='-31' width='60' height='61' style='fill: grey'/>");
        prevent_rotation();
        notice_collisions_with("armShoulderLink", [&](Event &e) {
            int arm_shoulder_link_id = e.value()["id"];
            Agent& arm_shoulder_link = find_agent(arm_shoulder_link_id); 
            attach_to(arm_shoulder_link);
            std::cout << "\n\nCollision with armShoulderLink detected\n\n";
            ignore_collisions_with("armShoulderLink");
        });
    }
    void start() {}
    void update() {}
    void stop() {}

};

class armBaseLink : public Agent {
    public:
    armBaseLink(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    armBaseLinkController c;
};

DECLARE_INTERFACE(armBaseLink)

#endif