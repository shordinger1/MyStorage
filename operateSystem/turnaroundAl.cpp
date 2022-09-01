#include <bits/stdc++.h>
using namespace std;
#define log cout
class Tick
{
private:
    int RunTime;
    int MaxTick;
    int TimePerTick;
    //int *CPUTick;
    string *CPUTickId;

public:
    Tick()
    {
        RunTime = 0;
        MaxTick = 10000;
        TimePerTick = 50;
        //CPUTick = new int[MaxTick];
        CPUTickId = new string[MaxTick];
    }

    bool run(string id)
    {
        if (RunTime >= MaxTick)
            return false;
        log << "CPU: Running the Process id=" << id << " ,in CPUTime" << RunTime << '\n';
        CPUTickId[RunTime] = id;
        RunTime++;
        return true;
    }

    int tick() { return RunTime; }
    int rate() { return TimePerTick; }
} runtime;
class Process
{
private:
    string id;
    int priority;
    int remain;
    int need;
    bool state;

public:
    Process(){};
    Process(string id, int priority, int need) : id(id), priority(priority), remain(need), need(need), state(1){};
    void change_priority(int p) { priority += p; }
    void run()
    {
        if (runtime.run(id))
        {
            remain -= runtime.rate();
            if (remain <= 0)
            {
                state = 0;
            }
            change_priority(-1);
            log << "Process " << id << " Running successful\n"
                << "PRIORITY: " << priority << ";"
                << "REMAIN: " << remain << ";"
                << "STATE: " << state << ";\n";
        }
        else
        {
            //log << "Error:CPU process time not enough\n";
        }
    }
    bool states() { return state; }
    int pro() { return priority; }
};
class TurnAround
{
private:
    Process *process;
    const static int ProcessNumber = 50;

public:
    TurnAround()
    {
        process = new Process[ProcessNumber];
        for (int i = 0; i < ProcessNumber; i++)
        {
            stringstream str;
            str << 'P' << i;
            process[i] = Process(str.str(), rand() % (10) + 1, rand() % 500 + 1);
        }
    }

    int check()
    {
        int res = -1;
        int pro = 0;
        for (int i = 0; i < ProcessNumber; i++)
        {
            if (process[i].states())
            {
                if (process[i].pro() > pro)
                {
                    res = i;
                    pro = process[i].pro();
                }
            }
        }
        return res;
    }

    void turn()
    {
        int now = check();
        log << "turn begin:\n";
        while (now >= 0)
        {
            process[now].run();
            for (int i = 0; i < ProcessNumber; i++)
            {
                if (i != now && process[i].states())
                {
                    process[i].change_priority(1);
                }
            }
            now = check();
        }
        log << "turn end:\n";
    }
} ProcessTurnAroundObject;

int main()
{
    ProcessTurnAroundObject.turn();
}
