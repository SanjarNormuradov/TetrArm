#ifndef __CONVEYOR_GEAR_AGENT__H
#define __CONVEYOR_GEAR_AGENT__H 

#include "enviro.h"
#include <string>


using namespace enviro;
using namespace std;

class EmergencyStop : public State, public AgentInterface {
    public:
        EmergencyStop() : State("Emergency Stop") {}
        void entry(const Event& e) {
            // cout << "Emergency Stop Entry\n";
            emit(Event("stateEmergencyStop"));
        }
        void during() {
            // cout << "Emergency Stop During\n";
            // if (!transitionEvent.empty()) {
            //     emit(Event(transitionEvent));
            //     cout << "Event emitted\n";
            //     transitionEvent.clear();
            // }
            track_velocity(0, 0, 0, 100000);
        }
        void exit(const Event& e) {
            // cout << "Emergency Stop Exit\n";
        }
        // string transitionEvent;
};

class StartConveyor : public State, public AgentInterface {
    public:
        StartConveyor() : State("Start Conveyor") {}
        void entry(const Event& e) {
            // cout << "Start Conveyor Entry\n";
            emit(Event("stateStartConveyor"));
        }
        void during() {
            // cout << "Start Conveyor During\n";
            // if (!transitionEvent.empty()) {
            //     emit(Event(transitionEvent));
            //     cout << "Event emitted\n";
            //     transitionEvent.clear();
            // }
            track_velocity(0, -0.3, 0, 100000);
        }
        void exit(const Event& e) {
            // cout << "Start Conveyor Exit\n";
        }
        // string transitionEvent;
};

class StopConveyor : public State, public AgentInterface {
    public:
        StopConveyor() : State("Stop Conveyor") {}
        void entry(const Event& e) {
            // cout << "Stop Conveyor Entry\n";
            emit(Event("stateStopConveyor"));
        }
        void during() {
            // cout << "Stop Conveyor During\n";
            // if (!transitionEvent.empty()) {
            //     emit(Event(transitionEvent));
            //     cout << "Event emitted\n";
            //     transitionEvent.clear();
            // }
            track_velocity(0, 0, 0, 100000);
        }
        void exit(const Event& e) {
            // cout << "Stop Conveyor Exit\n";
        }
        // string transitionEvent;
};

class SensorStopConveyor : public State, public AgentInterface {
    public:
        SensorStopConveyor() : State("Sensor Stop Conveyor") {}
        void entry(const Event& e) {
            // cout << "Sensor Stop Conveyor Entry\n";
            emit(Event("stateSensorStopConveyor"));
        }
        void during() {
            // cout << "Sensor Stop Conveyor During\n";
            // if (!transitionEvent.empty()) {
            //     emit(Event(transitionEvent));
            //     cout << "Event emitted\n";
            //     transitionEvent.clear();
            // }
            track_velocity(0, 0, 0, 100000);
        }
        void exit(const Event& e) {
            // cout << "Sensor Stop Conveyor Exit\n";
        }
        // string transitionEvent;
};

class conveyorGearSM : public StateMachine, public AgentInterface {
    public:
        conveyorGearSM() : StateMachine() {
            set_initial(emergency_stop);
            add_transition("Start Conveyor: ES -> SC", emergency_stop, start_conveyor);
            add_transition("Stop Conveyor: SC -> !SC", start_conveyor, stop_conveyor);
            add_transition("Start Conveyor: !SC -> SC", stop_conveyor, start_conveyor);
            add_transition("Sensor Stop Conveyor: SC -> SSC", start_conveyor, sensor_stop_conveyor);
            add_transition("Sensor Start Conveyor: SSC -> SC", sensor_stop_conveyor, start_conveyor);
            add_transition("Emergency Stop: SC -> ES", start_conveyor, emergency_stop);
            add_transition("Emergency Stop: !SC -> ES", stop_conveyor, emergency_stop);
            add_transition("Emergency Stop: SSC -> ES", sensor_stop_conveyor, emergency_stop);
            sensorScanRead = false;
        }
        void init() {
            // cout << "StateMachine init\n";
            // cout << "Initial State: " << _initial->name() << "; Currrent State: " << ((_current == NULL) ? "none" : _current->name()) << ";\n";
            watch("button_click", [this](const Event& e) {
                // cout << "\nButton is Clicked\n";
                string clickedButton = e.value()["value"];
                // e.stop_propagation(); It stops handling following events, so 1st gear will be updated and the other 2 not
                // cout << clickedButton << endl;
                string currentState = (_current == NULL) ? _initial->name() : _current->name();
                // cout << "Clicked Button: Name: " << clickedButton << "; Current State: " << currentState << ";\n";
                // cout << "Initial State: " << _initial->name() << "; Currrent State: " << _current->name() << ";\n";

                if ((currentState == "Emergency Stop") && (clickedButton == "startConveyor")) {
                    // cout << "Start Conveyor: ES -> SC\n";
                    // emergency_stop.transitionEvent = "Start Conveyor: ES -> SC";
                    emit(Event("Start Conveyor: ES -> SC"));
                    sensorScanRead = false;
                } 
                else if ((currentState == "Start Conveyor") && (clickedButton == "stopConveyor")) {
                    // cout << "Stop Conveyor: SC -> !SC\n";
                    // start_conveyor.transitionEvent = "Stop Conveyor: SC -> !SC";
                    emit(Event("Stop Conveyor: SC -> !SC"));
                    emit(Event("Stop Blocks"));
                } 
                else if ((currentState == "Stop Conveyor") && (clickedButton == "startConveyor")) {
                    // cout << "Start Conveyor: !SC -> SC\n";
                    // stop_conveyor.transitionEvent = "Start Conveyor: !SC -> SC";
                    emit(Event("Start Conveyor: !SC -> SC"));
                    sensorScanRead = false;        
                }
                else if ((currentState == "Start Conveyor") && (clickedButton == "emergencyStop")) {
                    // cout << "Emergency Stop: SC -> ES\n";
                    // start_conveyor.transitionEvent = "Emergency Stop: SC -> ES";
                    emit(Event("Emergency Stop: SC -> ES"));      
                    emit(Event("Stop Blocks"));           
                } 
                else if ((currentState == "Stop Conveyor") && (clickedButton == "emergencyStop")) {
                    // cout << "Emergency Stop: !SC -> ES\n";
                    // stop_conveyor.transitionEvent = "Emergency Stop: !SC -> ES";
                    emit(Event("Emergency Stop: !SC -> ES"));             
                }
                else if ((currentState == "Sensor Stop Conveyor") && (clickedButton == "emergencyStop")) {
                    // cout << "Emergency Stop: SSC -> ES\n";
                    // stop_conveyor.transitionEvent = "Emergency Stop: SSC -> ES";
                    emit(Event("Emergency Stop: SSC -> ES"));             
                }
            });
            watch("Sensor Detected Object", [this](Event& e) {
                string currentState = (_current == NULL) ? _initial->name() : _current->name();
                if (currentState == "Start Conveyor") {
                    // cout << "[Conveyor Gear] Sensor Detected Object\n";
                    emit(Event("Sensor Stop Conveyor: SC -> SSC"));
                    emit(Event("Stop Blocks"));
                }
            });
            watch("Sensor Scanning...", [this](Event& e) {
                string currentState = (_current == NULL) ? _initial->name() : _current->name();
                if (currentState == "Sensor Stop Conveyor") {
                    // cout << "[Conveyor Gear] Sensor Scanning...\n";
                    emit(Event("Sensor Start Conveyor: SSC -> SC"));
                    sensorScanRead = false;
                } else if ((currentState == "Start Conveyor") && !sensorScanRead) {
                    // cout << "[Conveyor Gear] Move Blocks\n";
                    emit(Event("Move Blocks"));
                    sensorScanRead = true;
                }
            });
            StateMachine::init();
        }
        // void start() {
        //     StateMachine::start();
        //     cout << "StateMachine start\n";
        //     cout << "Initial State: " << _initial->name() << "; Currrent State: " << _current->name() << ";\n";
        // }
        void update() {
            // cout << "\nStateMachine update\n";
            // cout << "Initial State: " << _initial->name() << "; Currrent State: " << _current->name() << ";\n";
            StateMachine::update();
        }

    private:
        EmergencyStop emergency_stop;
        StartConveyor start_conveyor;
        StopConveyor stop_conveyor;
        SensorStopConveyor sensor_stop_conveyor;
        bool sensorScanRead;
};

class conveyorGear : public Agent {
    public:
        conveyorGear(json spec, World& world) : Agent(spec, world) {
            add_process(cgSM);
            decorate("<circle cx='0' cy='0' r='40' style='fill: white'/><circle cx='0' cy='0' r='20' style='fill: grey'/><line x1='45' y1='0' x2='-45' y2='0' stroke='grey' stroke-width='10'/><line x1='0' y1='45' x2='0' y2='-45' stroke='grey' stroke-width='10'/>");
        }
    private:
        conveyorGearSM cgSM;
};

DECLARE_INTERFACE(conveyorGear)

#endif