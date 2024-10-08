#pragma once
#include<algorithm>
#include<array>
#include<map>
#include<string>
using namespace std;

enum class Action { Up, Right, Down, Left };
const map<Action, Action> inverse{ { Action::Up, Action::Down }, { Action::Right, Action::Left }, { Action::Down, Action::Left }, { Action::Left, Action::Right } };

class Board {
private:
    Board(pair<int, int>& _agent, array<array<int, 4>, 4>& _board, Action _edge, int _path) : agent(_agent), board(_board), edge(_edge), path(_path) {}

    void __swap__(int a, int b) {
        swap(board[a / 4][a % 4], board[b / 4][b % 4]);
    }
public:
    // friend overcomplicates
    pair<int, int> agent;
    array<array<int, 4>, 4> board;
    Action edge;
    int path;

    Board(int pos = 0, int _path = -1) : path(_path) {
        generate(begin(board), end(board), [n = 0]() mutable { n += 4; return array<int, 4>({ n - 4, n - 3, n - 2, n - 1 }); });
        __swap__(0, pos);
    }

    operator string() const {
        string string = "";
        for (auto& arr : board) {
            for (auto& elem : arr)
                string += elem + (elem < 10 ? "  " : " ");
            string += '\n';
        }

        return string;
    }

    Board* move(int parent, Action action) {
        pair<int, int> _agent = agent;
        array<array<int, 4>, 4> _board = board;
        switch (action) {
            case Action::Up:
                _agent.first = agent.first == 0 ? 3 : agent.first - 1;
                _board[0][agent.second] = board[1][agent.second], _board[1][agent.second] = board[2][agent.second], _board[2][agent.second] = board[3][agent.second], _board[3][agent.second] = board[0][agent.second];
                break;
            case Action::Right:
                _agent.second = agent.second == 3 ? 0 : agent.second + 1;
                ranges::rotate(_board[agent.first], end(_board[agent.first]) - 1);
                break;
            case Action::Down:
                _agent.first = agent.first == 3 ? 0 : agent.first + 1;
                _board[3][agent.second] = board[2][agent.second], _board[2][agent.second] = board[1][agent.second], _board[1][agent.second] = board[0][agent.second], _board[0][agent.second] = board[3][agent.second];
                break;
            case Action::Left:
                _agent.second = agent.second == 0 ? 3 : agent.second - 1;
                ranges::rotate(_board[agent.first], begin(_board[agent.first]) + 1);
                break;
        }

        return new Board(_agent, _board, action, parent);
    }

    string TraverseMiddleToFront(const vector<Board*>& tree) {
        return to_string(static_cast<int>(inverse.at(edge))) + (path == -1 ? "" : tree[path]->TraverseMiddleToFront(tree));
    }

    string TraverseBackToMiddle(const vector<Board*>& tree) {
        return (path == -1 ? "" : tree[path]->TraverseBackToMiddle(tree)) + to_string(static_cast<int>(edge));
    }

    void Traverse(string strategy) {
        Board* board = this;
        for (char elem : strategy) {
            cout << static_cast<string>(*board) << endl;
            Action action = static_cast<Action>(elem - '0');
            Board* temp = board->move(-1, action);
            delete board;
            board = temp;
        }
    }
};

inline bool operator==(const Board& lhs, const Board& rhs) { return lhs.board == rhs.board; }

ostream& operator<<(ostream& os, Board*& obj) {
    os.put(obj->agent.first + '0');
    os.put(obj->agent.second + '0');
    for (auto& arr : obj->board) {
        for (auto& elem : arr)
            os.put(elem + '0');
    }

    os.put(static_cast<char>(obj->edge));
    os << obj->path;
    os.put('\n');
    return os;
}

istream& operator>>(istream& os, Board*& obj) {
    obj = new Board();
    obj->agent.first = os.get() - '0', obj->agent.second = os.get() - '0';
    for (auto& arr : obj->board) {
        for (auto& elem : arr)
            elem = os.get() - '0';
    }

    obj->edge = static_cast<Action>(os.get());
    os >> obj->path;
    os.get();
    return os;
}
