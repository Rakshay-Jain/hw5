#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool backtrackSchedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsUsed,
    size_t day,
    size_t slot);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0){
        return false;
    }
    sched.clear();
    // Add your code below
    size_t totalDays = avail.size();
    size_t totalWorkers = avail[0].size();

    if (totalWorkers==0){
        return false;
    }
    if (dailyNeed > totalWorkers){
        return false;
    }
    if (maxShifts * totalWorkers < dailyNeed * totalDays){
        return false;
    }
    sched.resize(totalDays);
    std::vector<size_t> shiftsUsed(totalWorkers, 0);
    return backtrackSchedule(avail, dailyNeed, maxShifts, sched, shiftsUsed, 0, 0);




}

bool backtrackSchedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsUsed,
    size_t day,
    size_t slot){
        size_t totalDays = avail.size();
        size_t totalWorkers = avail[0].size();

        if (day == totalDays){
            return true;
        }
        if(slot == dailyNeed){
            return backtrackSchedule(avail, dailyNeed, maxShifts, sched, shiftsUsed, day + 1, 0);
        }

        for (size_t w=0; w<totalWorkers; w++){
            if(!avail[day][w]){
                continue;
            }
            if(shiftsUsed[w] >= maxShifts){
                continue;
            }
            if(std::find(sched[day].begin(), sched[day].end(), w) != sched[day].end()){
                continue;
            }

            sched[day].push_back(static_cast<Worker_T>(w));
            shiftsUsed[w]++;
            if(backtrackSchedule(avail, dailyNeed, maxShifts, sched, shiftsUsed, day, slot + 1)){
                return true;
            }
            shiftsUsed[w]--;
            sched[day].pop_back();
        }
        return false;
    }

