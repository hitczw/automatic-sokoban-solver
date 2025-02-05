#pragma once
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>
#include <stack>

template<typename ResultType, typename NodeType, Method alg>
class Solver_template {
public:
    using Node = NodeType;
    using Result = ResultType;
    Result solve(
        const Node* start,
        const Node* goal,
        std::function<void(const Node*, std::function<void(const Node*)>)> get_neighbors,
        std::function<bool(const Node*)> is_visited,
        std::function<void(const Node*)> mark_visited,
        std::function<bool(const Node*, const Node*)> is_equal,
        std::function<int(const Node*, const Node*)> heuristic = nullptr
    ) {
        if (is_equal(start, goal)) {
            if constexpr (std::is_same_v<Result, bool>) {
                return true; // 返回是否有解
            }
            else if constexpr (std::is_same_v<Result, std::vector<Node>>) {
                return {*start}; // 返回具体路径
            }
        }

        using Container_Type = std::conditional_t<
                alg == Method::bfs,
                std::queue<const Node*>,
                std::conditional_t<alg == Method::dfs, std::stack<const Node*>, std::priority_queue<std::pair<int, const Node*>, std::vector<std::pair<int, const Node*>>, decltype(&Solver_template::compare_pairs)>>
                >;
        Container_Type container;
        if constexpr (alg == Method::a_star) {
            // 对于 A*，需要传入比较函数对象
            container = std::priority_queue<std::pair<int, const Node*>, std::vector<std::pair<int, const Node*>>, decltype(&Solver_template::compare_pairs)>(&Solver_template::compare_pairs);
        }
        std::unordered_map<const Node*, const Node*> parent; // 仅当需要路径时使用TODO:可考虑采用链表存储
        if constexpr (alg == Method::a_star){
            container.push({heuristic(start, goal), start});
        }
        else{
            container.push(start);
        }

        mark_visited(start); 
        while (!container.empty()){
            const Node* current = nullptr;
            if constexpr (alg == Method::bfs){
                current = container.front();
            }
            else if constexpr (alg == Method::dfs){
                current = container.top();
            }
            else{
                current = container.top().second;
            }
            container.pop();

            // 使用回调函数处理邻居节点
            bool found = false;
            get_neighbors(current, [&](const Node* neighbor) {
                if (!is_visited(neighbor)) { // 检查是否已访问
                    mark_visited(neighbor);  // 标记为已访问
                    if constexpr (!std::is_same_v<Result, bool>) {
                        parent[neighbor] = current; // 仅当需要路径时记录父节点
                    }
                    if constexpr (alg == Method::a_star){
                        int f_score = heuristic(neighbor, goal);
                        container.push({f_score, neighbor});
                    }
                    else{
                        container.push(neighbor);
                    }

                    if (is_equal(neighbor, goal)){
                        found = true;
                        goal = neighbor;
                    }
                    //可优化，先判断是否是终点节点 todo
                }
            });
            if (found == true)
            {
                if constexpr (std::is_same_v<Result, bool>) {
                    return true; // 返回是否有解
                }
                else if constexpr (std::is_same_v<Result, std::vector<Node>>) {
                    return get_path(start, goal, parent); // 返回具体路径
                }
            }

        }

        return get_default_result();
    }

private:
    // 返回具体路径
    std::vector<Node> get_path(const Node* start, const Node* goal, 
                               const std::unordered_map<const Node*, const Node*>& parent) {
        std::vector<Node> path;
        const Node* current = goal;
        while (current != start) {
            path.push_back(*current);
            current = parent.at(current);
        }
        path.push_back(*start);
        return path;
    }

    // 默认返回值
    Result get_default_result() {
        if constexpr (std::is_same_v<Result, bool>) {
            return false; // 无解
        } else if constexpr (std::is_same_v<Result, std::vector<Node>>) {
            return {}; // 空路径
        }
    }

    static bool compare_pairs(const std::pair<int, const Node*>& a, const std::pair<int, const Node*>& b) {
        return a.first > b.first;
    }
};