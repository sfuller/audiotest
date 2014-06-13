#include "../src/AudioSystem.hpp"
#include "../src/Sound.hpp"
#include "../src/VorbisSound.hpp"
#include "../src/PCMSound.hpp"
#include "../src/PCMData.hpp"

#include <lua.hpp>
#include <string>
#include <iostream>

using namespace Metalancer;

PCMData soundData;
PCMSound sound1;

int play(lua_State* state)
{
    sound1.Play();
    return 0;
}

int stop(lua_State* state)
{
    sound1.Stop();
    return 0;
}

int load(lua_State* state)
{
    const char* filename = luaL_checkstring(state, -1);
    soundData.OpenFile(filename);
    return 0;
}

int main(int argc, char** argv)
{
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);
    
    lua_pushcfunction(state, play);
    lua_setglobal(state, "play");
    
    lua_pushcfunction(state, stop);
    lua_setglobal(state, "stop");
    
    lua_pushcfunction(state, load);
    lua_setglobal(state, "loadsound");
    
    AudioSystem system;

    //Sound sound1;
    //Sound sound2;

    //VorbisSound sound1;
    //VorbisSound sound2;

    //sound1.OpenFile("begining.wav");

    //sound1.OpenFile("test.ogg");
    //sound2.OpenFile("hay.ogg");

    sound1.SetData(&soundData);

    system.AddSound(&sound1);
    //system.AddSound(&sound2);

    system.Play();

    //sound1.Play();
    sound1.Update(1.0f);
    //sound1.Pause();

    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while(true)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime);
        float floatDuration = duration.count();
        sound1.Update(floatDuration);
        lastTime = currentTime;
        
        std::string inputString;
        std::cout << "]";
        std::getline(std::cin, inputString);
        int result = luaL_dostring(state, inputString.c_str());
        if(result)
        {
            std::cerr << lua_tostring(state, -1) << std::endl;
        }
    }

}

