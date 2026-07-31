#include <iostream>
#include <string>
#include <vector>
#include "../include/console.hpp"
#include "../include/input_buffer.hpp"
using namespace std;

void console(){
    string in;
    vector<string> history;
    while(1)
    {
        cout << "ironhold>";
        getline(cin,in);
        history.push_back(in);
        if(in==".quit") break;
        if(in == ".help")
        {
            cout <<   ".quit — flush open state and exit cleanly\n";
            cout << ".help — print a summary of available commands\n";
            cout << ".version — prints version\n";
            cout << ".status — prints World's current status:\n";
        }
        else if(in == ".version")
        {
            cout << "Chronicle v0.1 — Ironhold World Engine\n";
        }
        else if(in ==".status")
        {
            cout << "World Offline\n";
        }
        else if(in.substr(0,1)==".") 
        {
            cout << "State::UNKNOWN for unrecognized dot-commands with a hint to try .help\n";
        }
        else {
            cout << "[World command queued: <input>]";
        }
        InputBuffer buffer;
        buffer.processAndLog(history, "ironhold_clean.log");


    }
}