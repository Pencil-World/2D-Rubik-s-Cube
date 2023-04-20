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
const int step = 1'000;

// create a strategy and prove permeability
// add augmentation functionality to SNAKE
// solves the board in real time using strat

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
    int counter = 2;
    array<int, 100 + 2> domains{ 0, 1 };

    string temp1;
    for (fstream file("domains.txt", ios::in); file >> temp1; domains[counter] = stoi(temp1), ++counter) {}
    
    Board* temp2;
    for (fstream file("tree.txt", ios::in); file >> temp2; tree.push_back(temp2)) {}

    //counter = *ranges::find_if(search, [it](Board* obj) { return **it == *obj; });
    int lim = static_cast<int>(tree.size()) / step + step;
    for (; counter < domains.size(); domains[counter] = static_cast<int>(tree.size()), ++counter) {
        for (int i = domains[counter - 2]; i < domains[counter - 1]; ++i) {
            for_each(begin(actions), end(actions), [&tree, i](Action action) { Board* board = tree[i]->move(i, action); 
                (ranges::find_if(tree, [&board](Board* obj) { return *board == *obj; }) == end(tree)) ? tree.push_back(board) : delete board; });
            if (tree.size() > lim) {
                print(counter - 1, static_cast<int>(tree.size()));
                for_each(begin(tree) + (lim - step), begin(tree) + lim, [file = fstream("tree.txt", ios::app)](Board* obj) mutable { file << obj; });
                lim += step;
                tree.reserve(tree.size() + step);
            }
        }

        { fstream("domains.txt", ios::app) << tree.size() << endl; }
    }

    tree.shrink_to_fit();
}

void SWAP(int pos, const vector<Board*>& tree) {
    cout << endl;
    print("SWAP " + to_string(pos));
    vector<Board*> search = { new Board(pos) };
    int i = 0, lim = step;
    for (int counter = 0; counter < 12; ++counter) {
        search.reserve(search.size() + step);
        for (; i < lim; ++i)
            for_each(begin(actions), end(actions), [&search, i](Action action) { Board* board = search[i]->move(i, action); 
                (ranges::find_if(search, [&board](Board* obj) { return *board == *obj; }) == end(search)) ? search.push_back(board) : delete board; });

        auto it = find_first_of(begin(tree), end(tree), begin(search) + (lim - step), end(search) + lim, [](Board* a, Board* b) { return *a == *b; });
        if (it != end(tree)) {
            cout << "SWAP successful" << endl << static_cast<string>(**it);
            fstream("strategies.txt", ios::app) << (*it)->TraverseFrontToMiddle(tree) << (*ranges::find_if(search, [it](Board* obj) { return **it == *obj; }))->TraverseBackToMiddle(tree) << endl;
            return;
        }

        lim += step;
        print(counter + 1, static_cast<int>(search.size()));
    }

    print("SWAP failed");
    system("pause");
}

int main() {
    print("start program");
    vector<Board*> tree{ { new Board() } };
    GROW(tree);

    SWAP(1, tree);
    SWAP(2, tree);
    SWAP(5, tree);
    SWAP(6, tree);
    SWAP(10, tree);
    return 0;
}