// BankSim3000
//
// The purpose of this bank and teller simulation is to help a bank manager to make an informed
// decision on how many tellers to hire at a branch with longer than desired wait times.

#include <iostream>
#include <variant>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>
#include <string>

using namespace std;

const size_t MIN_TELLERS = 1;
const size_t MAX_TELLERS = 5;

// Integer time units.
using Time = int;

// We will be tracking teller state in a variable std::vector.
using TellerIndex = size_t;

// Arrival event containing only the arrival and transaction times.
struct ArrivalEvent {
    Time arrivalTime;
    Time transactionTime;
};

// This is a common idiom in FP, wrapping a type in another to yield better
// semantics (meaning) while gaining some static type checking. This stacking can
// usually be optimized out by the compiler. It could also be a provisional
// placeholder for types that might be expanded later.
struct Customer {
    ArrivalEvent arrivalEvent;
};

// A departure event including the expected departure time and the
// teller being departed from.
struct DepartureEvent {
    Time departureTime;
    TellerIndex tellerIndex;
};

// Either an arrival or departure event. Variant can be thought of as a safer union.
using Event = std::variant<ArrivalEvent, DepartureEvent>;

// Helper function to get the time from either an arrival or departure event.
Time get_event_time(const Event& e) {
    if (holds_alternative<ArrivalEvent>(e)) {
        return get<ArrivalEvent>(e).arrivalTime;
    }
    return get<DepartureEvent>(e).departureTime;
}

// A compare functor / function object for the priority queue. Creates a min-heap.
struct CompareEvent {
    bool operator()(const Event& e1, const Event& e2) {
        return get_event_time(e1) > get_event_time(e2);
    }
};

// Holds availability and when the teller started to become busy. Also automatically
// accumulates elapsed busy time.
//
// Hint: Use startWork and stopWork from the event processing methods to track
//       teller activity.
class Teller {
private:
    // Optional is an ADT that either holds nothing when the teller isn't
    // busy, represented by nullopt, or a time value of when the teller
    // started working.
    optional<Time> startBusy;
    // Accumulated busy time for the teller.
    Time elapsedTimeBusy;

public:
    Teller() : startBusy(nullopt), elapsedTimeBusy(0) {}

    bool isAvailable() {
        return !startBusy.has_value();
    }

    void startWork(Time currentTime) {
        startBusy = currentTime;
    }

    void stopWork(Time currentTime) {
        Time elapsedTime = currentTime - startBusy.value();
        elapsedTimeBusy += elapsedTime;
        startBusy = nullopt;
    }

    // Returns the final elapsed time a teller has been working after the simulation is finished.
    Time elapsedTimeWorking() {
        return elapsedTimeBusy;
    }
};

struct SimulationResults {
    vector<Time> elapsedTimeBusy;

    // Finds the max teller time and is a good measure of the overall time.
    Time maxTellerBusyTime() {
        return *max_element(elapsedTimeBusy.begin(), elapsedTimeBusy.end());
    }

    SimulationResults(vector<Time> elapsedTimeBusy) : elapsedTimeBusy(elapsedTimeBusy) {}
};

// A line of customers waiting to be served by a teller.
using BankLine = queue<Customer>;
// The event priority queue used by the simulation.
using EventQueue = priority_queue<Event, vector<Event>, CompareEvent>;
// A list of arrival events used to start the simulation.
using SimulationInput = vector<ArrivalEvent>;

class BankSim3000 {
private:
    // Input is stored locally to help restart the simulation for multiple tellers.
    SimulationInput simulationInput;
    // The event queue. Initially this is loaded with the simulation input.
    EventQueue eventQueue;
    // The bank line. Initially this is empty.
    BankLine bankLine;

    // One teller simulation state for each teller.
    vector<Teller> tellers;

    // Resets the tellers vector to the requested size and initialized to the default constructor.
    void resetTellers(size_t tellerCount) {
        if (tellerCount != tellers.size()) {
            tellers.reserve(tellerCount);
        }

        tellers.clear();

        for (size_t i = 0; i < tellerCount; ++i) {
            tellers.emplace_back();
        }
    }

    // Clears the bank line.
    void clearBankLine() {
        assert(bankLine.empty()); // It should already be cleared after a complete simulation run.
        while (!bankLine.empty()) {
            bankLine.pop();
        }
    }

    // Clears the event queue and initializes it to our input data.
    void setupEventQueue() {
        assert(eventQueue.empty()); // Should also already be empty after a complete simulation.
        while (!eventQueue.empty()) {
            eventQueue.pop();
        }

        for (int i = 0; i < simulationInput.size(); i++) {
            eventQueue.push(simulationInput.at(i));
        }

    }

    // Sets up the simulation for the given number of tellers.
    void setupSimulation(size_t tellerCount) {
        if (tellerCount < MIN_TELLERS) {
            throw invalid_argument("Teller count must >= " + to_string(MIN_TELLERS));
        }
        if (tellerCount > MAX_TELLERS) {
            throw invalid_argument("Teller count must be <= " + to_string(MAX_TELLERS));
        }

        setupEventQueue();

        resetTellers(tellerCount);

        clearBankLine();
    }

    // Processes either an arrival or a departure event.
    void processEvent(Time currentTime, const Event& e) {
        if (holds_alternative<ArrivalEvent>(e)) {
            ArrivalEvent arrivalEvent = get<ArrivalEvent>(e);
            processArrival(currentTime, arrivalEvent);
        }
        else {
            assert(holds_alternative<DepartureEvent>(e));
            DepartureEvent departureEvent = get<DepartureEvent>(e);
            processDeparture(currentTime, departureEvent);
        }
    }

    // Helper used by processArrival.
    // Returns the index of an available teller or nullopt if all are busy.
    optional<size_t> searchAvailableTellers() {
        for (size_t i = 0; i < tellers.size(); ++i) {
            if (tellers[i].isAvailable()) {
                return i;
            }
        }
        return nullopt;
    }

    // Process arrival events.
    //
    // If teller is not available or the bank line is full then we're busy,
    // place customer at the end of the bank line. Otherwise, we weren't
    // busy so start teller work and add a new departure event to the event queue.
    void processArrival(Time currentTime, const ArrivalEvent& arrivalEvent) {
        auto teller = searchAvailableTellers();

        bool is_teller_available = teller.has_value();

        if (is_teller_available) {
            tellers.at(teller.value()).startWork(arrivalEvent.arrivalTime);
            eventQueue.push(DepartureEvent { arrivalEvent.arrivalTime + arrivalEvent.transactionTime, teller.value() });
        }
        else {
            bankLine.push(Customer{ arrivalEvent });
        }
    }

    // Process departure events.
    //
    // If bank line is empty then the teller should stop working.
    // Otherwise, take the next customer off the bank line and enqueue a new departure
    // event into the event priority queue.
    void processDeparture(Time currentTime, const DepartureEvent& departureEvent) {
        size_t tellerIndex = departureEvent.tellerIndex;
        if (bankLine.empty()) {
            tellers.at(tellerIndex).stopWork(currentTime);
        }
        else {
            ArrivalEvent arrivalEvent = bankLine.front().arrivalEvent;
            bankLine.pop();
            eventQueue.push(DepartureEvent{ currentTime + arrivalEvent.transactionTime, tellerIndex });
        }

    }

    // Runs the simulation.
    void runSimulation() {
        while (!eventQueue.empty()) {
            // Remove event.
            Event e = eventQueue.top();
            eventQueue.pop();

            processEvent(get_event_time(e), e);
        }
    }

    SimulationResults gatherResults() {
        // Transform is like map in more functional languages. It takes an input vector and fills
        // a new vector with the results of the given function passed as a parameter.
        vector<Time> elapsedTimeBusy(tellers.size());
        transform(tellers.begin(), tellers.end(), elapsedTimeBusy.begin(), [](auto teller) {
            return teller.elapsedTimeWorking();
            });

        return SimulationResults{ elapsedTimeBusy };
    }

public:

    BankSim3000(SimulationInput simulationInput) : simulationInput(simulationInput) {}

    Time maxTellerBusyTime(size_t tellerCount) {
        setupSimulation(tellerCount);

        runSimulation();

        return gatherResults().maxTellerBusyTime();
    }
};

int main() {
    // Do not change the input.
    SimulationInput SimulationInput00 = { {20, 6}, {22, 4}, {23, 2}, {30, 3} };

    BankSim3000 bankSim(SimulationInput00);

    cout << "Time waiting with 1 teller: " << bankSim.maxTellerBusyTime(1) << endl;
    cout << "Time waiting with 2 tellers: " << bankSim.maxTellerBusyTime(2) << endl;
    cout << "Time waiting with 3 tellers: " << bankSim.maxTellerBusyTime(3) << endl;
    cout << "Time waiting with 4 tellers: " << bankSim.maxTellerBusyTime(4) << endl;
    cout << "Time waiting with 5 tellers: " << bankSim.maxTellerBusyTime(5) << endl;

    return 0;
}

// Simulation results:
// Time waiting with 1 teller: 15
// Time waiting with 2 tellers: 11
// Time waiting with 3 tellers: 9
// Time waiting with 4 tellers: 9
// Time waiting with 5 tellers: 9
//
// ========================================== SHORT ANSWER QUESTIONS ==========================================
// 
// A: What number of tellers should the branch manager hire? Explain your reasoning
//  Based on this simulation, the branch manager should hire 3 tellers, since the results do not improve
//  for more than 3 tellers. However, this simulation only shows one instance of a group of 3 customers
//  arriving at the same time; more data would be required for a useful conclusion. 
// 
// B: What kind of simulation is this and why?
//  This is an event-driven simulation, since the exact time and length of each event is known,
//  eliminating the need to process on every step like in a time-based simulation. 
// 
// C: Why use the priority queue for the event queue and a regular queue for the bank line?
//  The event queue needs to keep events ordered by time and always have the next event at the top,
//  which a priority queue is perfect for. The bank line represents a more standard real-life queue
//  where incoming people are sorted in the order they arrived, which a regular queue simulates. 
// 
// D: Can you think of any other problems, aside from banking, that an event simulation could solve? What values would it track?
//  An event-based simulation could be used to optimize lines at a theme park. It would track the availability of rides based on
//  the amount of time they take, place guests into the queues for the rides they want, and take the appropriate
//  number of guests out of the queue if they are queued for a ride that is currently available. Rides with multiple
//  vehicles would be easy to simulate by tracking an count of available vehicles that decreases when the
//  ride starts and increases when it ends. 
//