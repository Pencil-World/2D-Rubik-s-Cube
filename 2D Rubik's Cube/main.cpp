#include<algorithm>
#include<iomanip>
using std::put_time;
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

#include"Board.h"

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
    const vector<Action> actions{ Action::Up, Action::Right, Action::Down, Action::Left };
    const int step = 100'000;

    int counter = 2;
    array<int, 16 + 2> domains{ 0, 1 };

    Board* temp1;
    for (fstream file("tree.txt", ios::in); file >> temp1; tree.push_back(temp1)) {}

    //int temp2;
    //for (fstream file("domains.txt", ios::in); file >> temp2 && temp2 < tree.size(); domains[counter] = temp2, ++counter) {}
    //
    //if (tree.size() > 1) domains[counter - 2] = tree.back()->path;
    //int lim = static_cast<int>(tree.size()) / step + step;
    //for (; counter < domains.size(); ++counter) {
    //    for (int i = domains[counter - 2]; i < domains[counter - 1]; ++i) {
    //        for_each(begin(actions), end(actions), [&tree, i](Action action) { Board* board = tree[i]->move(i, action); 
    //            (ranges::find_if(tree, [&board](Board* obj) { return *board == *obj; }) == end(tree)) ? tree.push_back(board) : delete board; });
    //        if (tree.size() > lim) {
    //            print(counter - 1, static_cast<int>(tree.size()));
    //            for_each(begin(tree) + max(1, (lim - step)), begin(tree) + lim, [file = fstream("tree.txt", ios::app)](Board* obj) mutable { file << obj; });
    //            lim += step;
    //            tree.reserve(tree.size() + step);
    //        }
    //    }

    //    domains[counter] = static_cast<int>(tree.size());
    //    for_each(begin(domains) + 2, begin(domains) + counter + 1, [file = fstream("domains.txt", ios::out | ios::trunc)](int elem) mutable { file << elem << endl; });
    //}

    tree.shrink_to_fit();
}

void SWAP(int pos, const vector<Board*>& tree) {
    cout << endl;
    print("SWAP " + to_string(pos));
    const map<Action, string> table{ { Action::Up, "Up" }, { Action::Right, "Right" }, { Action::Down, "Down" }, { Action::Left, "Left" } };

    vector<Board*> search = { new Board(pos) };
    search.reserve(tree.size());

    int i = 1;
    for (int parent = tree[i]->path; search.size() < tree.size() - 1; parent = tree[++i]->path)
        search.push_back(search[parent]->move(parent, tree[i]->edge));

    auto it = find_first_of(begin(tree) + 646655, end(tree), begin(search) + 646655, end(search), [](Board* a, Board* b) { return *a == *b; });
    if (it != end(tree)) {
        auto middle = ranges::find_if(search, [it](Board* obj) { return **it == *obj; });
        string strategy = (*middle)->TraverseBackToMiddle(tree) + (*it)->TraverseMiddleToFront(tree);

        cout << "SWAP successful" << endl;
        search.front()->Traverse(strategy);
        cout << static_cast<string>(**it) << endl;

        fstream file("strategies.txt", ios::app);
        for (char elem : strategy)
            file << table.at(static_cast<Action>(elem - '0')) << " ";
        file << endl;
        file.close();
    } else
        print("SWAP failed");

    for (Board* elem : search)
        delete elem;
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