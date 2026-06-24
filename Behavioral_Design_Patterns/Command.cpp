#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm> 
#include <cctype>    
#include <stack>
using namespace std;


class Light {
public:
    void on() { cout << "Light turned ON" << endl; }
    void off() { cout << "Light turned OFF" << endl; }
};

class Thermostat {
private:
    int currentTemperature = 20;

public:
    void setTemperature(int temp) {
        cout << "Thermostat set to " << temp << "C" << endl;
        currentTemperature = temp;
    }

    int getCurrentTemperature() const { return currentTemperature; }
};

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class LightOnCommand : public Command {
private:
    Light* light;

public:
    LightOnCommand(Light* light) : light(light) {}
    void execute() override { light->on(); }
    void undo() override { light->off(); }
};

class LightOffCommand : public Command {
private:
    Light* light;

public:
    LightOffCommand(Light* light) : light(light) {}
    void execute() override { light->off(); }
    void undo() override { light->on(); }
};

class SetTemperatureCommand : public Command {
private:
    Thermostat* thermostat;
    int newTemperature;
    int previousTemperature;

public:
    SetTemperatureCommand(Thermostat* thermostat, int temperature)
        : thermostat(thermostat), newTemperature(temperature), previousTemperature(0) {}

    void execute() override {
        previousTemperature = thermostat->getCurrentTemperature();
        thermostat->setTemperature(newTemperature);
    }

    void undo() override {
        thermostat->setTemperature(previousTemperature);
    }
};

class RemoteControl {
private:
    stack<Command*> history;

public:
    void executeCommand(Command* command) {
        command->execute();
        history.push(command);
    }

    void undoLast() {
        if (!history.empty()) {
            Command* lastCommand = history.top();
            history.pop();
            lastCommand->undo();
        } else {
            cout << "Nothing to undo." << endl;
        }
    }
};

int main() {
    Light light;
    Thermostat thermostat;

    LightOnCommand lightOn(&light);
    LightOffCommand lightOff(&light);
    SetTemperatureCommand setTemp(&thermostat, 25);

    RemoteControl remote;

    cout << "--- Executing Commands ---" << endl;
    remote.executeCommand(&lightOn);
    remote.executeCommand(&setTemp);
    remote.executeCommand(&lightOff);

    cout << "\n--- Undoing Commands ---" << endl;
    remote.undoLast();
    remote.undoLast();
    remote.undoLast();
    remote.undoLast();

    return 0;
}