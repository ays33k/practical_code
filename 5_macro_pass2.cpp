#include <bits/stdc++.h>
using namespace std;

vector<vector<string>> tokenize(string filename) {
    ifstream fin(filename);
    string line;
    vector<vector<string>> code;
    while (getline(fin, line)) {
        stringstream ss(line);
        vector<string> tokens;
        string word;
        while (ss >> word) tokens.push_back(word);
        if (!tokens.empty()) code.push_back(tokens);
    }
    return code;
}

bool isMacroCall(vector<string> line, vector<vector<string>> &MNT) {
    for (auto &m : MNT) if (line[0] == m[0]) return true;
    return false;
}

int getMacroIndex(string name, vector<vector<string>> &MNT) {
    for (int i = 0; i < MNT.size(); i++) if (MNT[i][0] == name) return stoi(MNT[i][1]);
    return -1;
}

vector<string> extractActualArgs(vector<string> &line) {
    vector<string> args;
    for (int i = 1; i < line.size(); i++) {
        string a = line[i];
        a.erase(remove(a.begin(), a.end(), ','), a.end());
        args.push_back(a);
    }
    return args;
}

vector<vector<string>> expandMacro(string name, vector<string> &args,
                                   vector<vector<string>> &MNT,
                                   vector<vector<string>> &MDT) {
    vector<vector<string>> expanded;
    int idx = getMacroIndex(name, MNT);
    for (int i = idx; i < MDT.size(); i++) {
        if (MDT[i][0] == "MEND") break;
        if (isMacroCall(MDT[i], MNT)) {
            vector<string> innerArgs = extractActualArgs(MDT[i]);
            vector<vector<string>> innerExp = expandMacro(MDT[i][0], innerArgs, MNT, MDT);
            for (auto &line : innerExp) expanded.push_back(line);
            continue;
        }
        vector<string> line = MDT[i];
        for (auto &word : line) {
            for (int j = 0; j < args.size(); j++) {
                string key = "#" + to_string(j + 1);
                size_t pos = word.find(key);
                if (pos != string::npos) word.replace(pos, key.size(), args[j]);
            }
        }
        expanded.push_back(line);
    }
    return expanded;
}

vector<vector<string>> pass2(vector<vector<string>> &code,
                             vector<vector<string>> &MNT,
                             vector<vector<string>> &MDT) {
    vector<vector<string>> output;
    for (auto line : code) {
        if (isMacroCall(line, MNT)) {
            vector<string> args = extractActualArgs(line);
            vector<vector<string>> exp = expandMacro(line[0], args, MNT, MDT);
            for (auto &l : exp) output.push_back(l);
        } else output.push_back(line);
    }
    return output;
}

void writeOutput(vector<vector<string>> &out, string filename) {
    ofstream fout(filename);
    for (auto &line : out) {
        for (auto &word : line) fout << word << " ";
        fout << "\n";
    }
}

int main() {
    vector<vector<string>> MNT = {{"ONE", "0"}, {"TWO", "4"}};
    vector<vector<string>> MDT = {
        {"MOVER", "#3,", "#1"},
        {"ADD", "#3,", "#2"},
        {"MOVEM", "#3,", "#1"},
        {"MEND"},
        {"MOVER", "#3,", "#1"},
        {"ADD", "#3,", "#2"},
        {"MOVEM", "#3,", "#1"},
        {"ONE", "O,", "9,", "&E=CREG"},
        {"MEND"}
    };
    vector<vector<string>> code = tokenize("input.asm");
    vector<vector<string>> expanded = pass2(code, MNT, MDT);
    writeOutput(expanded, "output.asm");
}
