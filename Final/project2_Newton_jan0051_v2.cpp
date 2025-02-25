//Jacob Newton
//project2_Newton_jan0051_v2.cpp
//To compile: g++ project2_Newton_jan0051_v2.cpp -o project2_Newton_jan0051_v2
//To execute: ./project2_Newton_jan0051_v2
//ChatGPT was used to give me an idea of how to properly test the probabilistic functions (Aaron_shoots1, Bob_shoots)
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cassert>
using namespace std;

//Helper macro for testing, taking an assertion and message to be printed as arguments
#define TEST_ASSERT(assertion, msg)   \
    do {                              \
        assert(assertion);            \
        cout << msg << endl; \
    } while(0)

//Constants
const int NUM_TRIALS = 10000;
const double AARON_PROBABILITY = 1.0/3.0;
const double BOB_PROBABILITY = 1.0/2.0;
const double CHARLIE_PROBABILITY = 1.0;

//Logic Function Prototypes
bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive);
void Aaron_shoots1(bool &B_alive, bool &C_alive);
void Bob_shoots(bool &A_alive, bool &C_alive);
void Charlie_shoots(bool &A_alive, bool &B_alive);
void Aaron_shoots2(bool &B_alive, bool &C_alive);
void run_simulation(bool strat2);

void press_enter() {
    cout << "Press Enter to continue...";
    cin.ignore();
    cout << "\n";
}

bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive) { //Returns true if at least two of the three players are alive
    return (A_alive + B_alive + C_alive >= 2);
}

//Aaron uses stategy 1, shoots best shooter alive
void Aaron_shoots1(bool &B_alive, bool &C_alive) {
    if (C_alive) {
        if ((rand() % 100) < (AARON_PROBABILITY * 100)) C_alive = false;
    }
    else if (B_alive) {
        if ((rand() % 100) < (AARON_PROBABILITY * 100)) B_alive = false;
    }
}

void Bob_shoots(bool &A_alive, bool &C_alive) {
    if (C_alive) {
        if ((rand() % 100) < (BOB_PROBABILITY * 100)) C_alive = false;
    }
    else if (A_alive) {
        if ((rand() % 100) < (BOB_PROBABILITY * 100)) A_alive = false;
    }
}

void Charlie_shoots(bool &A_alive, bool &B_alive) { //Charlie always hits his target
    if (B_alive) {
        B_alive = false;
    }
    else if (A_alive) {
        A_alive = false;
    }
}

void Aaron_shoots2(bool &B_alive, bool &C_alive) {
    if (B_alive && C_alive) { //Aaron misses his first shot intentionally
        return;
    }
    Aaron_shoots1(B_alive, C_alive); //Aaron proceeds after the first round using strategy 1
}

void run_simulation(bool strat2, int &aaron_wins, int &bob_wins, int &charlie_wins) {
    aaron_wins = 0, bob_wins = 0, charlie_wins = 0;

    for (int i = 0; i < NUM_TRIALS; i++) {
        bool A_alive = true, B_alive = true, C_alive = true;

        while (at_least_two_alive(A_alive, B_alive, C_alive)) {
            if (A_alive) (strat2 ? Aaron_shoots2(B_alive, C_alive) : Aaron_shoots1(B_alive, C_alive));
            if (B_alive) Bob_shoots(A_alive, C_alive);
            if (C_alive) Charlie_shoots(A_alive, B_alive);
        }
        if (A_alive) aaron_wins++;
        if (B_alive) bob_wins++;
        if (C_alive) charlie_wins++;
    }
    cout << "Aaron won " << aaron_wins << "/10000 duels or " << (double)aaron_wins/NUM_TRIALS * 100 << "%\n";
    cout << "Bob won " << bob_wins << "/10000 duels or " << (double)bob_wins/NUM_TRIALS * 100 << "%\n";
    cout << "Charlie won " << charlie_wins << "/10000 duels or " << (double)charlie_wins/NUM_TRIALS * 100 << "%\n";
}

//Test Driver prototypes
void test_at_least_two_alive();
void test_Aaron_shoots1();
void test_Bob_shoots();
void test_Charlie_shoots();
void test_Aaron_shoots2();

//Unit tests

//Tests the at_least_two_alive function with all possible combinations of player states
void test_at_least_two_alive() {
    cout << "Unit Testing 1: Function - at_least_two_alive()\n";
    cout << "\tCase 1: Aaron alive, Bob alive, Charlie alive\n";
    TEST_ASSERT(at_least_two_alive(true, true, true) == true, "\tCase passed ...");

    cout << "\tCase 2: Aaron dead, Bob alive, Charlie alive\n";
    TEST_ASSERT(at_least_two_alive(false, true, true) == true, "\tCase passed ...");

    cout << "\tCase 3: Aaron alive, Bob dead, Charlie alive\n";
    TEST_ASSERT(at_least_two_alive(true, false, true) == true, "\tCase passed ...");

    cout << "\tCase 4: Aaron alive, Bob alive, Charlie dead\n";
    TEST_ASSERT(at_least_two_alive(true, true, false) == true, "\tCase passed ...");

    cout << "\tCase 5: Aaron dead, Bob dead, Charlie alive\n";
    TEST_ASSERT(at_least_two_alive(false, false, true) == false, "\tCase passed ...");

    cout << "\tCase 6: Aaron dead, Bob alive, Charlie dead\n";
    TEST_ASSERT(at_least_two_alive(false, true, false) == false, "\tCase passed ...");

    cout << "\tCase 7: Aaron alive, Bob dead, Charlie dead\n";
    TEST_ASSERT(at_least_two_alive(true, false, false) == false, "\tCase passed ...");

    cout << "\tCase 8: Aaron dead, Bob dead, Charlie dead\n";
    TEST_ASSERT(at_least_two_alive(false, false, false) == false, "\tCase passed ...");
}

//Runs a smaller scale simulation to ensure that Aaron is hitting his targets at (nearly) the correct probability
//Because Aaron's behavior is not deterministic, we need to run a large number of trials to avoid a single "miss" from causing the test to fail
void test_Aaron_shoots1() {
    bool B_alive, C_alive;
    const int trials = 1000;
    double tolerance = 0.05;

    cout << "Unit Testing 2: Function - Aaron_shoots1()\n";
    
    cout << "\tCase 1: Bob alive, Charlie alive\n";
    int kill_count_Charlie = 0;
    for (int i = 0; i < trials; i++) { 
        B_alive = true;
        C_alive = true;
        Aaron_shoots1(B_alive, C_alive);
        if (!C_alive) kill_count_Charlie++;
    }
    double probKillCharlie = (double)kill_count_Charlie / trials;
    TEST_ASSERT(abs(probKillCharlie - AARON_PROBABILITY) < tolerance, "\tAaron is shooting at Charlie");
   

    cout << "\tCase 2: Bob dead, Charlie alive\n";
    int kill_count_Charlie2 = 0;
    for (int i = 0; i < trials; i++) {
        B_alive = false;
        C_alive = true;
        Aaron_shoots1(B_alive, C_alive);
        if (!C_alive) kill_count_Charlie2++;
    }
    double probKillCharlie2 = (double)kill_count_Charlie2 / trials;
    TEST_ASSERT(abs(probKillCharlie2 - AARON_PROBABILITY) < tolerance, "\tAaron is shooting at Charlie");

    cout << "\tCase 3: Bob alive, Charlie dead\n";
    int kill_count_Bob = 0;
    for (int i = 0; i < trials; i++) {
        B_alive = true;
        C_alive = false;
        Aaron_shoots1(B_alive, C_alive);
        if (!B_alive) kill_count_Bob++;
    }
    double probKillBob = (double)kill_count_Bob / trials;
    TEST_ASSERT(abs(probKillBob - AARON_PROBABILITY) < tolerance, "\tAaron is shooting at Bob");
}

//Runs a smaller scale simulation to ensure that Bob is hitting his targets at (nearly) the correct probability
//Same principle as test_Aaron_shoots1
void test_Bob_shoots() {
    bool A_alive, C_alive;
    const int trials = 1000;
    double tolerance = 0.05;
    cout << "Unit Testing 3: Function - Bob_shoots()\n";

    cout << "\tCase 1: Aaron alive, Charlie alive\n"; 
    int kill_count_Charlie = 0;
    for (int i = 0; i < trials; i++) {
        A_alive = true;
        C_alive = true;
        Bob_shoots(A_alive, C_alive);
        if (!C_alive) kill_count_Charlie++;
    }
    double probKillCharlie = (double)kill_count_Charlie / trials;
    TEST_ASSERT(abs(probKillCharlie - BOB_PROBABILITY) < tolerance, "\tBob is shooting at Charlie");

    cout << "\tCase 2: Aaron dead, Charlie alive\n";
    int kill_count_Charlie2 = 0;
    for (int i = 0; i < trials; i++) {
        A_alive = false;
        C_alive = true;
        Bob_shoots(A_alive, C_alive);
        if (!C_alive) kill_count_Charlie2++;
    }
    double probKillCharlie2 = (double)kill_count_Charlie2 / trials;
    TEST_ASSERT(abs(probKillCharlie2 - BOB_PROBABILITY) < tolerance, "\tBob is shooting at Charlie");

    cout << "\tCase 3: Aaron alive, Charlie dead\n";
    int kill_count_Aaron = 0;
    for (int i = 0; i < trials; i++) {
        A_alive = true;
        C_alive = false;
        Bob_shoots(A_alive, C_alive);
        if (!A_alive) kill_count_Aaron++;
    }
    double probKillAaron = (double)kill_count_Aaron / trials;
    TEST_ASSERT(abs(probKillAaron - BOB_PROBABILITY) < tolerance, "\tBob is shooting at Aaron");
}

//Because Charlie's behavior is deterministic, we can test his function with a simple test case
void test_Charlie_shoots() {
    bool A_alive, B_alive;

    cout << "Unit Testing 4: Function - Charlie_shoots()\n";

    cout << "\tCase 1: Aaron alive, Bob alive\n";
    A_alive = true;
    B_alive = true;
    Charlie_shoots(A_alive, B_alive);
    TEST_ASSERT(!B_alive || !A_alive, "\tCharlie is shooting at Bob");

    cout << "\tCase 2: Aaron dead, Bob alive\n";
    A_alive = false;
    B_alive = true;
    Charlie_shoots(A_alive, B_alive);
    TEST_ASSERT(!B_alive, "\tCharlie is shooting at Bob");

    cout << "\tCase 3: Aaron alive, Bob dead\n";
    A_alive = true;
    B_alive = false;
    Charlie_shoots(A_alive, B_alive);
    TEST_ASSERT(!A_alive, "\tCharlie is shooting at Aaron");
}

void test_Aaron_shoots2() {
    bool B_alive, C_alive;
    const int trials = 1000;
    double tolerance = 0.05;

    cout << "Unit Testing 5: Function - Aaron_shoots2()\n";

    cout << "\tCase 1: Bob alive, Charlie alive\n"; //If Bob and Charlie are alive, Aaron will always miss his first shot
    B_alive = true;                                 //This makes running a simulation for this case unnecessary
    C_alive = true;
    Aaron_shoots2(B_alive, C_alive);
    TEST_ASSERT(B_alive && C_alive, "\tAaron intentionally misses his first shot" << "\n" << "\tBoth Bob and Charlie are alive");

    cout << "\tCase 2: Bob dead, Charlie alive\n";
    int kill_Count_Charlie = 0;
    for (int i = 0; i < trials; i++) {
        B_alive = false;
        C_alive = true;
        Aaron_shoots2(B_alive, C_alive);
        if (!C_alive) kill_Count_Charlie++;
    }
    double probKillCharlie = (double)kill_Count_Charlie / trials;
    TEST_ASSERT(abs(probKillCharlie - AARON_PROBABILITY) < tolerance, "\tAaron is shooting at Charlie");
    
    cout << "\tCase 3: Bob alive, Charlie dead\n";
    int kill_Count_Bob = 0;
    for (int i = 0; i < trials; i++) {
        B_alive = true;
        C_alive = false;
        Aaron_shoots2(B_alive, C_alive);
        if (!B_alive) kill_Count_Bob++;
    }
    double probKillBob = (double)kill_Count_Bob / trials;
    TEST_ASSERT(abs(probKillBob - AARON_PROBABILITY) < tolerance, "\tAaron is shooting at Bob");
}

int main() {
    srand(time(0));

    int aaron_wins1, bob_wins1, charlie_wins1;
    int aaron_wins2, bob_wins2, charlie_wins2;

    cout << "*** Welcome to Jacob's Truel of the Fates Simulator ***\n";
    test_at_least_two_alive();
    press_enter();
    test_Aaron_shoots1();
    press_enter();
    test_Bob_shoots();
    press_enter();
    test_Charlie_shoots();
    press_enter();
    test_Aaron_shoots2();
    press_enter();

    cout << "Ready to test strategy 1 (run 10000 times):\n";
    press_enter();
    run_simulation(false, aaron_wins1, bob_wins1, charlie_wins1);

    cout << "\nReady to test strategy 2 (run 10000 times):\n";
    press_enter();
    run_simulation(true, aaron_wins2, bob_wins2, charlie_wins2);

    if (aaron_wins1 > aaron_wins2) {
        cout << "\nStrategy 1 is better than strategy 2.\n";
    }
    else {
        cout << "\nStrategy 2 is better than strategy 1.\n";
    }
    return 0;
}

