#pragma once
#include<algorithm>
#include<array>
#include<list>
#include<map>
#include<numeric>
#include<sstream>
#include<string>
#include<vector> // gar - ar - bage
using namespace std;

enum class Action { Up, Right, Down, Left };
const map<Action, string> table{ { Action::Up, "Up" }, { Action::Right, "Right" }, { Action::Down, "Down" }, { Action::Left, "Left" } };

class Board {
public:
    pair<int, int> agent;
    array<array<int, 4>, 4> board;
    list<Action> path;

    Board& operator=(const Board& rhs) = default;

    Board(pair<int, int> _agent = { 0, 0 }, array<array<int, 4>, 4> _board = {}, list<Action> _path = {}) : agent({ _agent.second, _agent.first }), board(_board), path(_path) {
        if (board.front().front() == board.back().back()) {
            generate(begin(board), end(board), [n = 0]() mutable { n += 4; return array<int, 4>({n - 4, n - 3, n - 2, n - 1}); });
            if (agent.first + agent.second != 0)
                __swap__(0, 4 * agent.first + agent.second);
        }
    }

    static Board decode(string json) {
        stringstream stream(json);
        Board board{};

        stream >> board.agent.first >> board.agent.second;
        generate(begin(board.board), end(board.board), [&stream]() { array<int, 4> n{}; stream >> n[0] >> n[1] >> n[2] >> n[3]; return n; });
        for (int temp; stream >> temp; board.path.push_back(static_cast<Action>(temp))) {}
        return board;
    }

    string encode() {
        stringstream stream;
        stream << agent.first << " " << agent.second;
        for (auto& arr : board)
            for (auto elem : arr)
                stream << " " << elem;
        for (auto elem : path)
            stream << " " << static_cast<int>(elem);
        return stream.str();
    }

    void __swap__(int a, int b) {
        swap(board[a / 4][a % 4], board[b / 4][b % 4]);
    }

    Board move(Action action) {
        Board copy = *this;
        copy.path.push_back(action);
        int temp;
        switch (action) {
            case Action::Up:
                copy.agent.first = copy.agent.first == 0 ? 3 : copy.agent.first - 1;
                temp = copy.board[0][copy.agent.second];
                copy.board[0][copy.agent.second] = copy.board[1][copy.agent.second], copy.board[1][copy.agent.second] = copy.board[2][copy.agent.second], copy.board[2][copy.agent.second] = copy.board[3][copy.agent.second];
                copy.board[3][copy.agent.second] = temp;
                break;
            case Action::Right:
                copy.agent.second = copy.agent.second == 3 ? 0 : copy.agent.second + 1;
                ranges::rotate(copy.board[copy.agent.first], end(copy.board[copy.agent.first]) - 1);
                break;
            case Action::Down:
                copy.agent.first = copy.agent.first == 3 ? 0 : copy.agent.first + 1;
                temp = copy.board[3][copy.agent.second];
                copy.board[3][copy.agent.second] = copy.board[2][copy.agent.second], copy.board[2][copy.agent.second] = copy.board[1][copy.agent.second], copy.board[1][copy.agent.second] = copy.board[0][copy.agent.second];
                copy.board[0][copy.agent.second] = temp;
                break;
            case Action::Left:
                copy.agent.second = copy.agent.second == 0 ? 3 : copy.agent.second - 1;
                ranges::rotate(copy.board[copy.agent.first], begin(copy.board[copy.agent.first]) + 1);
                break;
        }

        return copy;
    }

    string history(bool reverse = false) const {
        if (!reverse)
            return accumulate(begin(path), end(path), string(), [this](string total, Action elem) { return total + table.at(elem); });
        const map<Action, Action> inverse{ { Action::Up, Action::Down }, { Action::Right, Action::Left }, { Action::Down, Action::Left }, { Action::Left, Action::Right } };
        return accumulate(rbegin(path), rend(path), string(), [this, &inverse](string total, Action elem) { return total + table.at(inverse.at(elem)); });
    }
};

inline bool operator==(const Board& lhs, const Board& rhs) { return lhs.board == rhs.board; }
inline bool operator<(const Board& lhs, const Board& rhs) { return lhs.board < rhs.board; }
ostream& operator<<(ostream& os, const Board& obj) {
    for (auto& arr : obj.board) {
        for (auto& elem : arr)
            os << elem << (elem < 10 ? "  " : " ");
        os << endl;
    }

    return os;
}