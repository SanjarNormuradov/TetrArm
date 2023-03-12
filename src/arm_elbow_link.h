#ifndef __ARM_ELBOW_LINK_AGENT__H
#define __ARM_ELBOW_LINK_AGENT__H 

#include "enviro.h"

using namespace enviro;

class armElbowLinkController : public Process, public AgentInterface {

    public:
    armElbowLinkController() : Process(), AgentInterface() {}

    void init() {
        decorate("<circle x='0' y='0' r='25' style='fill: lightblue'/><rect x='-20' y='109' width='40' height='41' style='fill: grey'/>");
        notice_collisions_with("armShoulderLink", [&](Event &e) {
            int arm_shoulder_link_id = e.value()["id"];
            Agent& arm_shoulder_link = find_agent(arm_shoulder_link_id); 
            attach_to(arm_shoulder_link);
            std::cout << "\n\nCollision with armShoulderLink detected\n\n";
            ignore_collisions_with("armShoulderLink");
        });
        notice_collisions_with("armWristLink", [&](Event &e) {
            int arm_wrist_link_id = e.value()["id"];
            Agent& arm_wrist_link = find_agent(arm_wrist_link_id); 
            attach_to(arm_wrist_link);
            std::cout << "\n\nCollision with armWristLink detected\n\n";
            ignore_collisions_with("armWristLink");
        });
    }
    void start() {}
    void update() {}
    void stop() {}

};

class armElbowLink : public Agent {
    public:
    armElbowLink(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    armElbowLinkController c;
};

DECLARE_INTERFACE(armElbowLink)

#endif