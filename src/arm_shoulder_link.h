#ifndef __ARM_SHOULDER_LINK_AGENT__H
#define __ARM_SHOULDER_LINK_AGENT__H 

#include "enviro.h"
#include <iostream>

using namespace enviro;

class armShoulderLinkController : public Process, public AgentInterface {

    public:
    armShoulderLinkController() : Process(), AgentInterface() {}

    void init() {
        decorate("<circle x='0' y='0' r='30' style='fill: lightblue'/><rect x='-25' y='109' width='50' height='51' style='fill: grey'/>");
        notice_collisions_with("armBaseLink", [&](Event &e) {
            int arm_base_link_id = e.value()["id"];
            Agent& arm_base_link = find_agent(arm_base_link_id); 
            attach_to(arm_base_link);
            std::cout << "\n\nCollision with armBaseLink detected\n\n";
            ignore_collisions_with("armBaseLink");
        });
        notice_collisions_with("armElbowLink", [&](Event &e) {
            int arm_elbow_link_id = e.value()["id"];
            Agent& arm_elbow_link = find_agent(arm_elbow_link_id); 
            attach_to(arm_elbow_link);
            std::cout << "\n\nCollision with armElbowLink detected\n\n";
            ignore_collisions_with("armElbowLink");
        });
    }
    void start() {}
    void update() {}
    void stop() {}

};

class armShoulderLink : public Agent {
    public:
    armShoulderLink(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    armShoulderLinkController c;
};

DECLARE_INTERFACE(armShoulderLink)

#endif