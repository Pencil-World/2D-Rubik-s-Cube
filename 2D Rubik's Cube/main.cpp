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

vector<Board*>& GROW() {
    int step = 1'000'000, counter = 2;
    array<double, 15> domains{ 0, 1 };
    vector<Board*> tree{ new Board(), new Board() };

    //string temp;
    //for (fstream file("domains.txt", ios::in); file >> temp; domains[counter] = stoi(temp), ++counter) {}
    //for (fstream file("tree.txt", ios::in); getline(file, temp); tree.push_back(Board::decode(temp))) {}
    //if (tree.size() < domains[counter - 1]) --counter;
    //int lim = tree.size() / step + step;

    //for (; counter < domains.size(); domains[counter] = tree.size(), ++counter) {
    //    for (int i = domains[counter - 2]; i < domains[counter - 1]; ++i) {
    //        transform(begin(actions), end(actions), back_inserter(tree), [&tree, i](Action action) { return tree[i]->move(action); });
    //        if (tree.size() > lim) {
    //            tm date;
    //            time_t now = time(0);
    //            localtime_s(&date, &now);
    //            cout << counter - 1 << " " << tree.size() << " " << put_time(&date, "%c") << endl;
    //            for_each(begin(tree) + (lim - step), begin(tree) + lim, [file = fstream("tree.txt", ios::app)](Board* elem) mutable { file << elem->encode() << endl; });
    //            lim += step;
    //        }
    //    }

    //    { fstream("domains.txt", ios::app) << tree.size() << endl; }
    //}

    sort(begin(tree), end(tree), [](Board* a, Board* b) { return *a < *b; });
    tree.erase(unique(begin(tree), end(tree), [](Board* a, Board* b) { return *a == *b; }), end(tree));
    return tree;
}

void SWAP(int a, int b, const vector<Board*>& tree) {
    int lim, step, counter = 2;
    lim = step = 1'000'000;
    vector<Board*> search = { new Board({ a, b }) };
    for (int counter = 0; counter < 15; ++counter) {
        for (Board* elem : search)
            transform(begin(actions), end(actions), back_inserter(search), [elem](Action action) { return elem->move(action); });
        tm date;
        time_t now = time(0);
        localtime_s(&date, &now);
        cout << counter - 1 << " " << tree.size() << " " << put_time(&date, "%c") << endl;
        auto it = ranges::find_first_of(begin(tree), end(tree), begin(search) + lim - step, begin(tree) + lim);
        if (it != end(tree)) {
            cout << "SWAP successful" << endl;
            fstream("strategies", ios::app) << (*it)->history() << " " << (*ranges::find(search, *it))->history(true) << endl;
            return;
        }
    }

    cout << "SWAP failed" << endl;
    system("pause");
}

int main() {
    clear();
    vector<Board*> tree = GROW();
    //SWAP(0, 1, tree);
    //SWAP(0, 2, tree);
    //SWAP(0, 5, tree);
    //SWAP(0, 6, tree);
    //SWAP(0, 10, tree);
    return 0;
}