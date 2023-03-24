#ifndef __CONVEYOR_BELT_AGENT__H
#define __CONVEYOR_BELT_AGENT__H 

#include "enviro.h"
#include <chrono>
#include <string>
#include <random>
#include <vector>

using namespace enviro;
using namespace std;

class conveyorBeltController : public Process, public AgentInterface {
    public:
        conveyorBeltController() : Process(), AgentInterface() {
            colours.push_back("red");
            colours.push_back("green");
            colours.push_back("black");
            colours.push_back("yellow");
            colours.push_back("blue");
            colours.push_back("orange");
            colours.push_back("magenta");
            colours.push_back("azure");
            colours.push_back("pink");
            colours.push_back("violet");
            colours.push_back("cyan");
            colours.push_back("grey");
            colours.push_back("brown");
            colours.push_back("lightgreen");
            colours.push_back("lightblue");
            colourSize = static_cast<int> (colours.size());
            blockGenFreq = chrono::seconds(3);
            elapsedTime = high_resolution_clock::duration::zero();
            prevGenRemain = high_resolution_clock::duration::zero();
            processFirstStart = false;
        }
        void init() {
            prevent_rotation();
            watch("stateEmergencyStop", [this](Event& e) {
                // cout << "[Conveyor Belt] Emergency Stop\n";
                currState = "Emergency Stop";
                if (processFirstStart) {
                    currTime = chrono::high_resolution_clock::now();
                    prevGenRemain = currTime - lastTime;
                }
            });
            watch("stateStartConveyor", [this](Event& e) {
                // cout << "[Conveyor Belt] Start Conveyor\n";
                currState = "Start Conveyor";
                lastTime = chrono::high_resolution_clock::now();
                processFirstStart = true;
            });
            watch("stateStopConveyor", [this](Event& e) {
                // cout << "[Conveyor Belt] Stop Conveyor\n";
                currState = "Stop Conveyor";
                currTime = chrono::high_resolution_clock::now();
                prevGenRemain = currTime - lastTime;
            });
            watch("stateSensorStopConveyor", [this](Event& e) {
                // cout << "[Conveyor Belt] Sensor Stop Conveyor\n";
                currState = "Sensor Stop Conveyor";
                currTime = chrono::high_resolution_clock::now();
                prevGenRemain = currTime - lastTime;
            });
        }
        void start() {
            currValue = sensor_value(0);
            prevValue = sensor_value(0);
        }
        void update() {
            currValue = sensor_value(0);
            if (currValue < 8) {
                if (prevValue > 8) {
                    // cout << "[Conveyor Belt] Sensor Detected Object\n";
                    emit(Event("Sensor Detected Object"));
                }
            } else {
                // cout << "[Conveyor Belt] Sensor Value: " << sensor_value(0) << endl;
                emit(Event("Sensor Scanning..."));
                if (currState == "Start Conveyor") {
                    currTime = chrono::high_resolution_clock::now();
                    elapsedTime = (currTime - lastTime) + prevGenRemain;
                    if (elapsedTime > blockGenFreq) {
                        mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
                        uniform_int_distribution<int> colourDist(0, colourSize - 1);
                        colour_id = colourDist(gen);
                        colour = colours[colour_id];
                        // cout << "[Conveyor Belt] Adding Block...\n";
                        Agent& agnt = add_agent("buildingBlock", 540, 45, 0, {{"fill", colour}});
                        // cout << "[Conveyor Belt] Block Added. Colour: " << colour << endl;
                        emit(Event("Block Added", {
                            {"colour", colour},
                            {"block_id", agnt.get_id()}
                        }));
                        // cout << "Block Added\n";
                        lastTime = chrono::high_resolution_clock::now();
                        prevGenRemain = high_resolution_clock::duration::zero();
                    }
                }
            }
            prevValue = currValue;
        }
        void stop() {}

    private:
        double currValue, prevValue;
        vector<string> colours;
        mt19937 gen;
        uniform_int_distribution<int> colourDist;
        int colour_id, colourSize;
        string colour, currState;
        chrono::high_resolution_clock::time_point lastTime,
                                                  currTime;
        chrono::high_resolution_clock::duration blockGenFreq, elapsedTime, prevGenRemain;
        bool processFirstStart;
};

class conveyorBelt : public Agent {
    public:
        conveyorBelt(json spec, World& world) : Agent(spec, world) {
            add_process(cbCtrl);
            decorate("<path d='M -450 0 A 60 60 0 1 0 -450 120 H 0' stroke='black' stroke-width='10' style='fill: none'/>");
            prevent_rotation();
        }
    private:
        conveyorBeltController cbCtrl;
};

DECLARE_INTERFACE(conveyorBelt)

#endif