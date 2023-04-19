#pragma once
#include<algorithm>
#include<array>
#include<cmath>
    using std::pow;
#include<map>
#include<numeric>
    using std::accumulate;
#include<string>
using namespace std;

enum class Action { Up, Right, Down, Left };
const map<Action, string> table{ { Action::Up, "Up" }, { Action::Right, "Right" }, { Action::Down, "Down" }, { Action::Left, "Left" } };

class Board {
private:
    Board(pair<int, int>&& _agent, array<array<int, 4>, 4>&& _board, Action _edge, int _path) : agent(_agent), board(_board), edge(_edge), path(_path) {}

    void __swap__(int a, int b) {
        swap(board[a / 4][a % 4], board[b / 4][b % 4]);
    }
public:
    // friend overcomplicates
    pair<int, int> agent;
    array<array<int, 4>, 4> board;
    Action edge;
    int path;

    Board(int _path = -1, int pos = 0) : path(_path) {
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

    string TraverseFrontToMiddle(vector<Board*>& tree) {
        return table.at(edge) + (path == -1 ? " " + tree[path]->TraverseFrontToMiddle(tree) : "");
    }

    string TraverseBackToMiddle(vector<Board*>& tree) {
        const map<Action, Action> inverse{ { Action::Up, Action::Down }, { Action::Right, Action::Left }, { Action::Down, Action::Left }, { Action::Left, Action::Right } };
        return (path == -1 ? tree[path]->TranverseBackToMiddle(tree) + " " : "") + table.at(inverse.at(edge));
    }
};

inline bool operator==(const Board& lhs, const Board& rhs) { return lhs.board == rhs.board; }

ostream& operator<<(ostream& os, const Board*& obj) {
    os.put(obj->agent.first + '0');
    os.put(obj->agent.second + '0');
    for (auto& arr : obj->board) {
        for (auto& elem : arr)
            os.put(elem + '0');
    }

    os.put(obs->edge);
    os << obs->path << '\n';
    return os;
}

istream& operator>>(istream& os, Board*& obj) {
    obj = new Board();
    obj->agent.first = os.get() - '0', obj->agent.second = os.get() - '0';
    for (auto& arr : obj->board) {
        for (auto& elem : arr)
            elem = os.get() - '0';
    }

    os.get(obj->edge);
    os >> obj->path;
    return os;
}
