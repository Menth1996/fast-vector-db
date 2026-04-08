#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <queue>

// A simplified mock implementation of a Vector Index for demonstration

class VectorIndex {
private:
    int dimension;
    int max_elements;
    std::vector<std::vector<float>> data;
    std::vector<int> ids;

    // Helper: Compute L2 distance
    float l2_distance(const std::vector<float>& a, const std::vector<float>& b) {
        float dist = 0.0f;
        for (int i = 0; i < dimension; ++i) {
            float diff = a[i] - b[i];
            dist += diff * diff;
        }
        return dist;
    }

public:
    VectorIndex(int dim, int max_elems) : dimension(dim), max_elements(max_elems) {
        std::cout << "Initialized VectorIndex with dim=" << dim << ", max_elements=" << max_elems << std::endl;
    }

    bool add_item(const std::vector<float>& vec, int id) {
        if (data.size() >= max_elements) {
            std::cerr << "Index is full!" << std::endl;
            return false;
        }
        if (vec.size() != dimension) {
            std::cerr << "Vector dimension mismatch!" << std::endl;
            return false;
        }
        data.push_back(vec);
        ids.push_back(id);
        return true;
    }

    // Returns pairs of (distance, id)
    std::vector<std::pair<float, int>> knn_query(const std::vector<float>& query, int k) {
        if (query.size() != dimension) {
            std::cerr << "Query dimension mismatch!" << std::endl;
            return {};
        }

        // Max heap to keep track of the k smallest distances
        std::priority_queue<std::pair<float, int>> pq;

        for (size_t i = 0; i < data.size(); ++i) {
            float dist = l2_distance(query, data[i]);
            if (pq.size() < k) {
                pq.push({dist, ids[i]});
            } else if (dist < pq.top().first) {
                pq.pop();
                pq.push({dist, ids[i]});
            }
        }

        // Extract results and reverse to get ascending order
        std::vector<std::pair<float, int>> results;
        while (!pq.empty()) {
            results.push_back(pq.top());
            pq.pop();
        }
        std::reverse(results.begin(), results.end());
        return results;
    }
    
    int size() const {
        return data.size();
    }
};

int main() {
    // Simple test
    VectorIndex idx(3, 100);
    idx.add_item({1.0, 0.0, 0.0}, 1);
    idx.add_item({0.0, 1.0, 0.0}, 2);
    idx.add_item({0.0, 0.0, 1.0}, 3);
    
    auto results = idx.knn_query({0.9, 0.1, 0.0}, 2);
    std::cout << "KNN Results:" << std::endl;
    for (const auto& res : results) {
        std::cout << "ID: " << res.second << ", Dist: " << res.first << std::endl;
    }
    return 0;
}
