#include <bits/stdc++.h>
using namespace std;

struct Sym{
    string name;
    int addr;
    Sym(string n= "", int a = -1){
        name = n;
        addr = a;
    }
};

struct Lit{
    string name;
    int addr;
    Lit(string n = "", int a = -1){
        name = n;
        addr = a;
    }
};

int main(){
    unordered_map<string, pair<string,string>> MOT = {
        {"STOP", {"IS","00"}}, {"ADD", {"IS","01"}}, {"SUB", {"IS","02"}}, {"MULT", {"IS","03"}}, {"MOVER", {"IS","04"}},
        {"MOVEM", {"IS","05"}}, {"COMP", {"IS","06"}}, {"BC", {"IS","07"}}, {"DIV", {"IS","08"}}, {"READ", {"IS","09"}},
        {"PRINT", {"IS","10"}},
        {"START", {"AD","01"}}, {"END", {"AD","02"}}, {"ORIGIN", {"AD","03"}}, {"EQU", {"AD","04"}}, {"LTORG", {"AD","05"}},
        {"DS", {"DL","01"}}, {"DC", {"DL","02"}},
    };

    unordered_map<string, int> reg = {
    {"AREG", 1},
    {"BREG", 2},
    {"CREG", 3}
    };

    vector<Sym> SYMTAB;
    unordered_map<string,int> symIndex;
    vector<Lit> LITTAB;
    vector<int> POOLTAB;

    struct ICLine{
        string address;
        vector<string> tokens;
    };
    vector<ICLine> IC;

    auto addSymbol = [&](const string &s)-> int {
        if(symIndex.count(s)) return symIndex[s];
        int i = SYMTAB.size();
        SYMTAB.emplace_back(s, -1);
        symIndex[s] = i;
        return i; 
    };

    auto setSymbolAddr = [&](const string &s, int addr) {
        int idx = addSymbol(s);
        SYMTAB[idx].addr = addr;
    };

    auto addLiteral = [&](const string &s)-> int {
        for(int i = 0;i < LITTAB.size();i++){
            if(LITTAB[i].name == s) return i;
        }
        LITTAB.emplace_back(s, -1);
        return LITTAB.size() - 1;
    };

    auto allocateLiteral = [&](int &LC, int poolStart) {
        int i = poolStart - 1;
        for(int j = i;j < LITTAB.size();j++){
            if(LITTAB[j].addr == -1){
                LITTAB[j].addr = LC;
                LC += 1;
            }
        }
    };

    vector<string> lines;
    ifstream fin("input.txt");
    if(!fin){
        cerr << "Error opening input.txt\n";
        return 1;
    }   
    string t;
    while(getline(fin,t)){
        lines.push_back(t);
    }
    fin.close();

    int LC = 0;
    bool startSeen = false;
    POOLTAB.push_back(1);

    for(string line: lines){
        vector<string> curr;
        string token;
        for (char c : line) {
            if (c == ' ' || c == ',' || c == '\t') {
                if (!token.empty()) {
                    curr.push_back(token);
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty()) curr.push_back(token);

        if (curr.empty()) continue;

        string label = "", opcode = "", operand1 = "", operand2 = "";

        if(MOT.count(curr[0]) > 0){
            opcode = curr[0];
            if(curr.size() > 1) operand1 = curr[1];
            if(curr.size() > 2) operand2 = curr[2];
        }
        else{
            label = curr[0];
            opcode = curr[1];
            if(curr.size() > 2) operand1 = curr[2];
            if(curr.size() > 3) operand2 = curr[3];
        }

        string cls = MOT[opcode].first;
        string code = MOT[opcode].second;

        if(!label.empty()){
            setSymbolAddr(label, LC);
            int i = addSymbol(label);
            cout << "(S, " << i + 1 << ") ";
        }
        cout << "(" << cls << "," << code << ")";

        if(cls == "IS"){
            if(!operand1.empty()){
                if(reg.count(operand1) > 0){
                    cout << " (" << reg[operand1] << ")";

                    if(!operand2.empty()){
                        string opnd = operand2;
                        if(opnd[0] == '='){
                            int i = addLiteral(opnd);
                            cout << " (L," << i + 1 << ")";
                        }
                        else if(isdigit(opnd[0])) {
                            cout << " (C," << opnd << ")";
                        }
                        else{
                            int i = addSymbol(opnd);
                            cout << " (S," << i + 1 << ")";
                        }
                    }
                }
                else{
                    string opnd = operand1;
                    if(opnd[0] == '='){
                        int i = addLiteral(opnd);
                        cout << " (L," << i + 1 << ")";
                    }
                    else if(isdigit(opnd[0])) {
                        cout << " (C," << opnd << ")";
                    }
                    else{
                        int i = addSymbol(opnd);
                        cout << " (S," << i + 1 << ")";
                    }
                }
            }
        }
        else if(cls == "DL"){
            cout << " (DL," << operand1 << ")";
            LC += stoi(operand1);
        }
        else if(opcode == "START"){
            LC = stoi(operand1);
            cout << ", (C," << operand1 << ")";
            startSeen = true;
        }
        else if(opcode == "END" || opcode == "LTORG"){
            allocateLiteral(LC, POOLTAB.back());
            if(opcode == "LTORG"){
                POOLTAB.push_back(LITTAB.size() + 1);
            }
        }
        cout << endl;
    }
    cout << "\nSYMBOL TABLE:\n";
    for (int i = 0; i < SYMTAB.size(); ++i)
        cout << i+1 << "\t" << SYMTAB[i].name << "\t" << SYMTAB[i].addr << "\n";

    cout << "\nLITERAL TABLE:\n";
    for (int i = 0; i < LITTAB.size(); ++i)
        cout << i+1 << "\t" << LITTAB[i].name << "\t" << LITTAB[i].addr << "\n";

    cout << "\nPOOL TABLE:\n";
    for (int i = 0; i < POOLTAB.size(); ++i)
        cout << i+1 << "\t" << POOLTAB[i] << "\n";
}