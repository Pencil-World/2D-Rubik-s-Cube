#pragma once
#include<algorithm>
#include<array>
#include<map>
#include<numeric>
    using std::accumulate;
#include<sstream>
#include<string>
#include<vector>
using namespace std;

enum class Action { Up, Right, Down, Left };
const map<Action, string> table{ { Action::Up, "Up" }, { Action::Right, "Right" }, { Action::Down, "Down" }, { Action::Left, "Left" } };

class Board {
private:
    pair<int, int> agent;
    vector<Action> path;

    Board(pair<int, int>& _agent, array<array<int, 4>, 4>& _board, vector<Action>& _path) : agent(_agent), board(_board), path(_path) {}

    void __swap__(int a, int b) {
        swap(board[a / 4][a % 4], board[b / 4][b % 4]);
    }
public:
    array<array<int, 4>, 4> board;

    Board(int pos = 0) {
        generate(begin(board), end(board), [n = 0]() mutable { n += 4; return array<int, 4>({ n - 4, n - 3, n - 2, n - 1 }); });
        __swap__(0, pos);
    }

    Board* move(Action action) {
        pair<int, int> _agent = agent;
        array<array<int, 4>, 4> _board = board;
        vector<Action> _path = path;
        _path.push_back(action);
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

        return new Board(_agent, _board, _path);
    }

    string history(bool reverse = false) const {
        if (!reverse)
            return accumulate(begin(path), end(path), string(), [](string total, Action elem) { return total + table.at(elem) + " "; });
        const map<Action, Action> inverse{ { Action::Up, Action::Down }, { Action::Right, Action::Left }, { Action::Down, Action::Left }, { Action::Left, Action::Right } };
        return accumulate(rbegin(path), rend(path), string(), [&inverse](string total, Action elem) { return total + table.at(inverse.at(elem)) + " "; });
    }
};

inline bool operator==(const Board& lhs, const Board& rhs) { return lhs.board == rhs.board; }
ostream& operator<<(ostream& os, const Board& obj) {
    for (auto& arr : obj.board) {
        for (auto& elem : arr)
            os << elem << (elem < 10 ? "  " : " ");
        os << endl;
    }

    return os;
}