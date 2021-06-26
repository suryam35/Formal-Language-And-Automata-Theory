// Name : Suryam Arnav Kalra
// Roll No. : 19CS30050

#include<iostream>
#include <fstream>
// #include <map>
using namespace std;

class NFA {

public:
    unsigned int numOfStates;    // Q
    unsigned int sizeOfAlphabet;  // sigma
    unsigned int **transitionFunction; // delta function
    unsigned int startState; // start state
    unsigned int finalState; // final state
    NFA(/*int n , int s , int start , int final*/) {
        // numOfStates = n;
        // sizeOfAlphabet = s;
        startState = 0;
        finalState = 0;
        transitionFunction = 0;
    }
    void readNFA(string fileName) {
        ifstream in;
        in.open(fileName);
        in >> numOfStates;            // takes input from the file given
        in >> sizeOfAlphabet;
        transitionFunction = new unsigned int*[numOfStates];
        for(int i = 0 ; i < numOfStates ; i++) {
            transitionFunction[i] = new unsigned int[sizeOfAlphabet];      // assigns memory to the transition function
            for(int j = 0 ; j < sizeOfAlphabet ; j++) {                    // and sets the value to 0
                transitionFunction[i][j] = 0;
            }
        }
        int n;
        in >> n;
        while(n != -1) {
            startState |= (1U << n);         // take the start state
            in >> n;
        }
        in >> n;
        while(n != -1) {
            finalState |= (1U << n);          // take the final state
            in >> n;
        }
        in >> n;
        while(n != -1) {
            int edge , f;
            in >> edge >> f;
            transitionFunction[n][edge] |= (1U << f);       // set the transition function
            in >> n;
        }
        in.close();
    }
    void printNFA() {
        cout << "+++ Input NFA\n";
        cout << "Number of states: " << numOfStates << endl;
        cout << "Input Alphabet : {0";
        for(int i = 1 ; i < sizeOfAlphabet ; i++) {
            cout << " ," << i;
        }
        cout << "}\n";
        cout << "Start states : {";
        for(int i = 0 ; i <= 31 ; i++) {
            if((1U << i) & startState) {
                cout << i << " ,";
            }
        }
        cout << "}\n";
        cout << "Final states : {";
        for(int i = 0 ; i <= 31 ; i++) {
            if((1U << i) & finalState) {
                cout << i << " ,";
            }
        }
        cout << "}\n";
        cout << "Transition function \n";
        for(int i = 0 ; i < numOfStates ; i++) {
            for(int j = 0 ; j < sizeOfAlphabet ; j++) {
                cout << "Delta(" << i << "," << j << ") = {";
                for(int bit = 0 ; bit <= 31 ; bit++) {
                    if (transitionFunction[i][j] & (1U << bit)) {
                        cout << bit << ",";
                    }
                }
                cout << "}\n";
            }
        }
    }
};

class DFA {

public:
    unsigned int numOfStates;           // Q --> number of states
    unsigned int sizeOfAlphabet;       // sigma -> size of alphabet
    unsigned int **transitionFunction;    // transition function
    unsigned int startState;                    // start state 
    unsigned int *finalState;                 // the set of final states
    DFA() {
        startState = 0;
        finalState = 0;
        transitionFunction = 0;
    }
    void subsetcons(NFA &nfa) {
        numOfStates = (1U << nfa.numOfStates);
        sizeOfAlphabet = nfa.sizeOfAlphabet;
        startState = nfa.startState;
        transitionFunction = new unsigned int*[numOfStates];
        for(int i = 0 ; i < numOfStates ; i++) {
            transitionFunction[i] = new unsigned int[sizeOfAlphabet];
            for(int j = 0 ; j < sizeOfAlphabet ; j++) {
                transitionFunction[i][j] = 0;
            }
        }
        for(int i = 0 ; i < numOfStates ; i++) {
            for(int j = 0 ; j < sizeOfAlphabet ; j++) {
                for(int bit = 0 ;bit <= 31 ; bit++) {
                    if(i & (1U << bit)) {
                        transitionFunction[i][j] |= nfa.transitionFunction[bit][j];
                    }
                }
            }
        }
        int size = (numOfStates+31)/32;
        finalState = new unsigned int[size];
        for(int i =  0 ; i < size ; i++) {
            finalState[i] = 0;
        }
        for(int i = 0 ; i < size ; i++) {
            for(int bit = 0 ; bit <= 31 ; bit++) {
                unsigned int state = bit + 32*i;
                for(int b = 0 ; b <= 31 ; b++) {
                    if(state & 1U<<b) {
                        if(nfa.finalState & 1U << b) {
                            finalState[i] |= 1U << bit;
                        }
                    }
                }
            }
        }
    }
    void printDFA() {
        cout << "Number Of States: " << numOfStates << endl;
        cout << "Input Alphabet : {0";
        for(int i = 1 ; i < sizeOfAlphabet ; i++) {
            cout << " ," << i;
        }
        cout << "}\n";
        cout << "Start state : " << startState << endl;
        if(numOfStates > 64) {
            int count = 0;
            int size = (numOfStates+31)/32;
            for(int i = 0 ; i < size ; i++) {
                for(int bit = 0 ; bit <= 31 ; bit++){
                    if(finalState[i] & (1U << bit)){
                        count++;
                    }
                }
            }
            cout << count << " final states\n";
            cout << "Transition Function : Skipped \n";
        }
        else {
            cout << "Final States : {";
            int size = (numOfStates+31)/32;
            for(int i = 0 ; i < size ; i++) {
                for(int bit = 0 ; bit <= 31 ; bit++){
                    if(finalState[i] & (1U << bit)){
                        cout << bit + 32*i << ",";
                    }
                }
            }
            cout << "}\n";
            cout << "Transition Function \n";
            cout << "a\\p|";
            for(int i = 0 ; i < numOfStates ; i++) {
                if(i <= 9) {
                    cout << "  " << i;
                }
                else{
                    cout << " " << i;
                }
            }
            cout << "\n---+";
            for(int i = 0 ; i < 3*numOfStates ; i++) {
                cout << "-";
            }
            cout << "\n";
            for(int j = 0 ; j < sizeOfAlphabet ; j++) {
                cout <<" "<< j << " |";
                for(int i = 0 ; i < numOfStates ; i++) {
                    if(transitionFunction[i][j] <= 9) {
                        cout << "  " << transitionFunction[i][j];
                    }
                    else {
                        cout << " " << transitionFunction[i][j];
                    }
                }
                cout << "\n";
            }
        }
    }
};

struct mapping {
    unsigned int state;                      // alternative structure to map
    int mapped_to;
};

void dfs(DFA& dfa , unsigned int *a , int start) {
    int ind = start/32;
    int rem = start%32;                                   // depth first search to find the set of reachable states
    a[ind] |= 1U << rem;
    for(int j = 0 ; j < dfa.sizeOfAlphabet ; j++) {
        unsigned int state = dfa.transitionFunction[start][j];
        ind = state/32;
        rem = state%32;
        if((a[ind] & 1U << rem) == 0) {
            dfs(dfa , a , state);
        }
    }
}

unsigned int* findreachable(DFA& dfa) {
    int size = (dfa.numOfStates+31)/32;
    unsigned int *a = new unsigned int[size];
    for(int i = 0 ; i < size ; i++) {
        a[i] = 0;                                   // auxiliary function to call dfs to get the set of reachable states
    }
    dfs(dfa , a , dfa.startState);
    return a;
}

int search(mapping maps[] , int length , unsigned int find) {
    for(int i = 0 ; i < length ; i++) {
        if(maps[i].state == find) {                   // auciliary function to help search in the map data structure 
            return maps[i].mapped_to;                 // that we have created
        }
    }
    return 1e9;
}

DFA& rnumreachable(DFA& dfa , unsigned int* a) {                // function to enumerate the reachable states and 
    DFA *dfa2 = new DFA;                                       // create a DFA out of the set of reachable states
    int cnt = 0;
    // map<unsigned int , int> m;
    mapping maps[100000];
    int length = 0;
    for(int i = 0 ; i < (dfa.numOfStates+31)/32 ; i++) {
        for(int bit = 0 ; bit <= 31 ; bit++) {
            if(a[i]& 1U << bit) {
                // m[bit+32*i] = cnt;
                maps[length].state = bit + 32*i;
                maps[length].mapped_to = cnt;
                length++;
                cnt++;
            }
        }
    }
    dfa2->numOfStates = cnt;
    dfa2->sizeOfAlphabet = dfa.sizeOfAlphabet;
    // dfa2->startState = m[dfa.startState];
    dfa2->startState = search(maps , length , dfa.startState);
    dfa2->transitionFunction = new unsigned int*[dfa2->numOfStates];
    for(int i = 0 ; i < cnt ; i++) {
        dfa2->transitionFunction[i] = new unsigned int[dfa2->sizeOfAlphabet];
        for(int j = 0 ; j < dfa2->sizeOfAlphabet ; j++) {
            dfa2->transitionFunction[i][j] = 0;
        }
    }
    for(int i = 0 ; i < (dfa.numOfStates+31)/32 ; i++) {
        for(int bit = 0 ; bit <= 31 ; bit++) {
            if(a[i]& 1U << bit) {
                int state = 32*i + bit;
                for(int j = 0 ; j < dfa.sizeOfAlphabet ; j++) {
                    // dfa2->transitionFunction[m[state]][j] = m[dfa.transitionFunction[state][j]];
                    int state1 = search(maps , length , state);
                    int state2 = search(maps , length , dfa.transitionFunction[state][j]);
                    dfa2->transitionFunction[state1][j] = state2;
                }
            }
        }
    }
    int size = (cnt+31)/32;
    dfa2->finalState = new unsigned int[size];
    for(int i = 0 ; i < size ; i++) {
        dfa2->finalState[i] = 0;
    }
    for(int i = 0 ; i < (dfa.numOfStates+31)/32; i++) {
        for(int bit = 0 ; bit <= 31 ; bit++) {
            if(dfa.finalState[i] & 1U << bit & a[i]) {
                int state = bit + 32*i;
                // int new_state = m[state];
                int new_state = search(maps , length , state);
                int ind = new_state/32;
                int rem = new_state % 32;
                dfa2->finalState[ind] |= 1U << rem;
            }
        }
    }
    return *dfa2;
}

bool **findequivalent(DFA& dfa) {                 // returns the equivalent states using the myhill nerode theorem
    bool **marked;
    marked = new bool*[dfa.numOfStates];
    for(int i = 0 ; i < dfa.numOfStates ; i++) {
        marked[i] = new bool[dfa.numOfStates];
        for(int j = 0 ; j < dfa.numOfStates ; j++) {
            marked[i][j] = 0;
        }   
    }
    for(int i = 0 ; i < dfa.numOfStates ; i++) {
        for(int j = i+1 ; j < dfa.numOfStates ; j++) {
            int ind1 = i/32;
            int rem1 = i%32;
            int ind2 = j/32;                                         // this loop sets the final and non-final state
            int rem2 = j%32;                                         // or vice versa to be as non equivalent states   
            if(dfa.finalState[ind1] & (1U << rem1)) {
                if((dfa.finalState[ind2] & (1U << rem2))== 0) {
                    marked[i][j] = 1;
                }
            }
            else {
                if(dfa.finalState[ind2] & 1U << rem2) {
                    marked[i][j] = 1;
                }
            }
        }
    }
    while(true) {
        int count = 0;
        for(int i = 0 ; i < dfa.numOfStates ; i++) {
            for(int j = i+1 ; j < dfa.numOfStates ; j++) {
                for(int k = 0 ; k < dfa.sizeOfAlphabet ; k++) {                // the while loop runs until no new 
                    unsigned int state1 = dfa.transitionFunction[i][k];        // set of equivalent states can be found
                    unsigned int state2 = dfa.transitionFunction[j][k];
                    if(state1 > state2) {
                        swap(state1, state2);
                    }
                    if(marked[state1][state2] && !marked[i][j]) {
                        marked[i][j] = 1;
                        count++;
                    }
                }
            }
        }  
        if(count == 0) {
            break;
        } 
    }
    return marked;
}

DFA& collapse(DFA& dfa , bool** marked) {                // a function to create a DFA out of the set of equivalent states
    cout << "\n+++ Equivalent states\n";                 // and return it
    int *visited = new int[dfa.numOfStates];
    for(int i = 0 ; i < dfa.numOfStates ; i++) {
        visited[i] = false;
    }
    int number = 0;
    // map<unsigned int , unsigned int> m;
    mapping maps[100000];
    int length = 0;
    for(int i = 0 ; i < dfa.numOfStates ; i++) {
        if(!visited[i]) {
            visited[i] = true;
            // m[i] = number;
            maps[length].state = i;
            maps[length].mapped_to = number;
            length++;
            cout << "Group";
            if(number <= 9) {
                cout << "  " << number<< ": {" << i << " ,";
            }
            else {
                cout << " " << number<< ": {" << i << " ,";
            }
            for(int j = i+1 ; j < dfa.numOfStates ; j++) {
                if(!marked[i][j]) {
                    // m[j] = number;
                    maps[length].state = j;
                    maps[length].mapped_to = number;
                    length++;
                    cout << j << " ,";
                    visited[j] = true;
                }
            }
            cout << "}\n";
            number++;
        }
    }
    DFA *dfa2 = new DFA;
    dfa2->numOfStates = number;
    dfa2->sizeOfAlphabet = dfa.sizeOfAlphabet;
    // dfa2->startState = m[dfa.startState];
    dfa2->startState = search(maps , length , dfa.startState);
    dfa2->transitionFunction = new unsigned int*[number];
    for(int i = 0 ; i < number ; i++) {
        dfa2->transitionFunction[i] = new unsigned int[dfa.sizeOfAlphabet];
        for(int j = 0 ; j < dfa.sizeOfAlphabet ; j++) {
            dfa2->transitionFunction[i][j] = 0;
        }
    }
    for(int i = 0 ; i < dfa.numOfStates ; i++) {
        for(int j = 0 ; j < dfa.sizeOfAlphabet ; j++) {
            // dfa2->transitionFunction[m[i]][j] = m[dfa.transitionFunction[i][j]];
            int state1 = search(maps , length , i);
            int state2 = search(maps , length , dfa.transitionFunction[i][j]);
            dfa2->transitionFunction[state1][j] = state2;
        }
    }
    int size = (number+31)/32;
    dfa2->finalState = new unsigned int[size];
    for(int i = 0 ; i < size ; i ++){
        dfa2->finalState[i] = 0;
    }
    for(int i = 0 ; i < (dfa.numOfStates+31)/32 ; i++) {
        for(int bit = 0 ; bit <= 31 ; bit++) {
            if(dfa.finalState[i] & 1U << bit) {
                // unsigned int state = m[bit+32*i];
                unsigned int state = search(maps , length , bit+32*i);
                int ind = state/32;
                int rem = state%32;
                dfa2->finalState[ind] |= 1U << rem;
            }
        }
    }
    return *dfa2;
}

int main() {
    NFA nfa;
    string filename;
    cout << "Enter the file name : ";
    cin >> filename;
    nfa.readNFA(filename);
    nfa.printNFA();
    cout << "\n+++ Converted DFA\n";
    DFA dfa;
    dfa.subsetcons(nfa);
    dfa.printDFA();
    cout << "\n+++ Reachable states:\n{";
    unsigned int *reachable = findreachable(dfa);
    for(int i = 0 ; i < (dfa.numOfStates+31)/32 ; i++) {
        for(int bit = 0 ; bit <= 31 ; bit++) {
            if(reachable[i]& 1U << bit) {
                cout << bit + 32*i << ",";
            }
        }
    }
    cout << "}\n";
    cout << "\n+++Reduced DFA after removing unreachable states\n";
    DFA dfa2 = rnumreachable(dfa , reachable);
    dfa2.printDFA();
    bool **marked = findequivalent(dfa2);
    DFA dfa3 = collapse(dfa2 , marked);
    cout << "\n+++ Reduced DFA after collapsing equivalent states\n";
    dfa3.printDFA();
    return 0;
}