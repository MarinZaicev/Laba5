#ifndef TRANSPORT_SYSTEM_H
#define TRANSPORT_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <set>

enum class Command {
    CREATE_TRL,
    TRL_IN_STOP,
    STOPS_IN_TRL,
    SHOW_ALL,
    EXIT,
    UNKNOWN
};

Command parseCommand(const std::string& input);
void processCommand(Command cmd);

#endif
