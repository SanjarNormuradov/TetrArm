#ifndef __ARM_WRIST_LINK_AGENT__H
#define __ARM_WRIST_LINK_AGENT__H 

#include "enviro.h"

using namespace enviro;

class armWristLinkController : public Process, public AgentInterface {

    public:
    armWristLinkController() : Process(), AgentInterface() {}

    void init() {
        decorate("<circle x='0' y='0' r='20' style='fill: lightblue'/>");
        notice_collisions_with("armElbowLink", [&](Event &e) {
            int arm_elbow_link_id = e.value()["id"];
            Agent& arm_elbow_link = find_agent(arm_elbow_link_id); 
            attach_to(arm_elbow_link);
            std::cout << "\n\nCollision with armElbowLink detected\n\n";
            std::cout << "\n\nCollision with armElbowLink detected\n\n";
            ignore_collisions_with("armElbowLink");
        });
    }
    void start() {}
    void update() {}
    void stop() {}

};

class armWristLink : public Agent {
    public:
    armWristLink(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    armWristLinkController c;
};

DECLARE_INTERFACE(armWristLink)

#endif
