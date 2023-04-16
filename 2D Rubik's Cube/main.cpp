#include<algorithm>
#include<array>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

#include"Board.h"
const vector<Action> actions{ Action::Up, Action::Right, Action::Down, Action::Left };

// create a strategy and prove permeability
// add augmentation functionality to SNAKE
// solves the board in real time using strat

void clear() {
    fstream("domains.txt", ios::out | ios::trunc).close();
    fstream("tree.txt", ios::out | ios::trunc).close();
}

void print(string message) {
    tm date;
    time_t now = time(0);
    localtime_s(&date, &now);
    cout << message << " " << put_time(&date, "%c") << endl;
}

void print(int depth, int breadth) {
    print(to_string(depth) + " " + to_string(breadth));
}

void GROW(vector<Board*>& tree) {
    print("GROW");
    int step = 1'000'000, counter = 2;
    array<double, 5 + 2> domains{ 0, 1 };

    int lim = tree.size() / step + step;
    for (; counter < domains.size(); domains[counter] = tree.size(), ++counter) {
        for (int i = domains[counter - 2]; i < domains[counter - 1]; ++i) {
            transform(begin(actions), end(actions), back_inserter(tree), [&tree, i](Action action) { return tree[i]->move(action); });
            if (tree.size() > lim) {
                print(counter - 1, tree.size());
                lim += step;
            }
        }
    }

    sort(begin(tree), end(tree), [](Board* a, Board* b) { return *a < *b; });
    tree.erase(unique(begin(tree), end(tree), [](Board* a, Board* b) { return *a == *b; }), end(tree));
}

void SWAP(int a, int b, const vector<Board*>& tree) {
    print("SWAP " + to_string(a) + " " + to_string(b));
    vector<Board*> search = { new Board({ a, b }) };
    for (int counter = 0; counter < 12; ++counter) {
        auto lim = search.size();
        search.reserve(5 * search.size());
        for (Board* elem : search) {
            transform(begin(actions), end(actions), back_inserter(search), [elem](Action action) { return elem->move(action); });
            delete elem;
        }

        search.erase(begin(search), begin(search) + lim);
        print(counter + 1, search.size());

        auto it = ranges::find_first_of(tree, search, [](Board* a, Board* b) { return *a == *b; });
        if (it != end(tree)) {
            cout << "SWAP successful" << endl;
            fstream("strategies.txt", ios::app) << (*it)->history() << (*ranges::find_if(search, [it](Board* obj) { return **it == *obj; }))->history(true) << endl;
            return;
        }
    }

    print("SWAP failed");
    system("pause");
}

int main() {
    print("start program");
    clear();
    vector<Board*> tree{ { new Board() } };
    GROW(tree);

    SWAP(1, 0, tree);
    SWAP(2, 0, tree);
    SWAP(1, 1, tree);
    SWAP(2, 1, tree);
    SWAP(2, 2, tree);
    return 0;
}