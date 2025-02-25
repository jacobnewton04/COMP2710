//Jacob Newton
//project2_Newton_jan0051_v1.cpp
//To compile: g++ project2_Newton_jan0051_v1.cpp -o project2_Newton_jan0051_v1
//To execute: ./project2_Newton_jan0051_v1
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cassert>
using namespace std;



//Constants
const int NUM_TRIALS = 10000;
const double AARON_PROBABILITY = 1.0/3.0;
const double BOB_PROBABILITY = 1.0/2.0;
const double CHARLIE_PROBABILITY = 1.0;

//Function Prototypes
bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive);
void Aaron_shoots1(bool &B_alive, bool &C_alive);
void Bob_shoots(bool &A_alive, bool &C_alive);
void Charlie_shoots(bool &A_alive, bool &B_alive);
void Aaron_shoots2(bool &B_alive, bool &C_alive);
void run_simulation(bool strat2);

void press_enter() {
    cout << "Press Enter to continue...";
    cin.ignore();
}

bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive) {
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

int main() {
    srand(time(0));

    int aaron_wins1, bob_wins1, charlie_wins1;
    int aaron_wins2, bob_wins2, charlie_wins2;

    cout << "*** Welcome to Jacob's Truel of the Fates Simulator ***\n";
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