#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

int main() {
    ifstream fin("input.asm");
    if (!fin.is_open()) {
        cout << "Error: Cannot open input.asm\n";
        return 1;
    }

    vector<string> lines;
    string line;

    while (getline(fin, line)) lines.push_back(line);
    fin.close();

    vector<vector<string>> code;
    for (auto &s : lines) {
        stringstream ss(s);
        string word;
        vector<string> tokens;
        while (ss >> word) tokens.push_back(word);
        code.push_back(tokens);
    }

    // Separate MACRO definitions
    vector<vector<string>> macros;
    bool insideMacro = false;
    for (auto &tokens : code) {
        if (insideMacro) {
            macros.push_back(tokens);
            if (tokens[0] == "MEND") insideMacro = false;
        } else if (tokens[0] == "MACRO") {
            macros.push_back(tokens);
            insideMacro = true;
        }
    }

    // Create output.asm without macro definitions
    ofstream fout("output.asm");
    bool skip = false;
    for (auto &s : lines) {
        if (s.find("MACRO") != string::npos) { skip = true; continue; }
        if (skip && s.find("MEND") != string::npos) { skip = false; continue; }
        if (!skip) fout << s << "\n";
    }
    fout.close();   

    // MNT, MDT, KPTAB, PNTAB
    vector<vector<string>> MNT, MDT, KPTAB;
    map<string, int> PNTAB;

    int start = 0;
    for (auto &s : macros) {
        if (s[0] == "MACRO") { start = 1; continue; }

        if (s[0] == "MEND") {
            MDT.push_back({"MEND"});
            start = 0;
            continue;
        }

        if (start == 1) {
            int p = 0;
            for (size_t i = 1; i < s.size(); ++i)
                if (s[i].find('=') == string::npos)
                    p++;

            MNT.push_back({s[0], to_string(p),
                           to_string((int)s.size() - 1 - p),
                           to_string((int)MDT.size()),
                           to_string((int)KPTAB.size())});

            for (size_t i = 1; i < s.size(); ++i) {
                string param = s[i];
                size_t eq = param.find('=');
                string name = param.substr(1, eq - 1);
                PNTAB[name] = PNTAB.size() + 1;

                if (eq != string::npos) {
                    string val = param.substr(eq + 1);
                    KPTAB.push_back({name, val});
                }
            }
            start = 2;
        } 
        else {
            vector<string> temp;
            for (auto &c : s) { 
                if (c[0] == '&') {
                    string name = c.substr(1);
                    temp.push_back("(P," + to_string(PNTAB[name]) + ")");
                } else {
                    temp.push_back(c);
                }
            }
            MDT.push_back(temp);
        }
    }

    // Write MNT    
    ofstream f1("mnt.txt");
    for (auto &v : MNT) {
        for (auto &x : v) f1 << x << " ";
        f1 << "\n";
    }
    f1.close();

    // Write MDT
    ofstream f2("mdt.txt");
    for (auto &v : MDT) {
        for (auto &x : v) f2 << x << " ";
        f2 << "\n";
    }
    f2.close();

    // Write KPTAB
    ofstream f3("kptab.txt");
    for (auto &v : KPTAB) {
        for (auto &x : v) f3 << x << " ";
        f3 << "\n";
    }
    f3.close();

    cout << "Macro Pass 1 completed successfully.\n";
    return 0;
}
