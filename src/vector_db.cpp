
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>
#include <algorithm>

// Function to calculate dot product of two vectors
double dot_product(const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size()) {
        throw std::runtime_error("Vector dimensions must match for dot product.");
    }
    double product = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        product += v1[i] * v2[i];
    }
    return product;
}

// Function to calculate magnitude of a vector
double magnitude(const std::vector<double>& v) {
    double sum_sq = 0.0;
    for (double val : v) {
        sum_sq += val * val;
    }
    return std::sqrt(sum_sq);
}

// Function to calculate cosine similarity between two vectors
double cosine_similarity(const std::vector<double>& v1, const std::vector<double>& v2) {
    double dp = dot_product(v1, v2);
    double mag1 = magnitude(v1);
    double mag2 = magnitude(v2);

    if (mag1 == 0.0 || mag2 == 0.0) {
        return 0.0; // Avoid division by zero
    }
    return dp / (mag1 * mag2);
}

// In-memory Vector Database Class
class InMemoryVectorDB {
public:
    // Add a vector to the database with an associated ID
    void add_vector(const std::string& id, const std::vector<double>& vec) {
        if (vec.empty()) {
            std::cerr << "Warning: Attempted to add an empty vector for ID: " << id << std::endl;
            return;
        }
        if (vectors_.empty()) {
            dimension_ = vec.size();
        } else if (vec.size() != dimension_) {
            throw std::runtime_error("All vectors must have the same dimension.");
        }
        vectors_[id] = vec;
        std::cout << "Added vector for ID: " << id << std::endl;
    }

    // Search for the top_k most similar vectors to a query vector
    std::vector<std::pair<std::string, double>> search(const std::vector<double>& query_vec, int top_k) const {
        if (query_vec.empty()) {
            std::cerr << "Error: Query vector cannot be empty." << std::endl;
            return {};
        }
        if (query_vec.size() != dimension_) {
            throw std::runtime_error("Query vector dimension must match database vectors.");
        }

        std::vector<std::pair<std::string, double>> similarities;
        for (const auto& entry : vectors_) {
            similarities.push_back({entry.first, cosine_similarity(query_vec, entry.second)});
        }

        // Sort by similarity in descending order
        std::sort(similarities.begin(), similarities.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        // Return top_k results
        if (similarities.size() > top_k) {
            similarities.resize(top_k);
        }
        return similarities;
    }

    // Get a vector by its ID
    const std::vector<double>* get_vector(const std::string& id) const {
        auto it = vectors_.find(id);
        if (it != vectors_.end()) {
            return &it->second;
        }
        return nullptr;
    }

    size_t size() const {
        return vectors_.size();
    }

    size_t dimension() const {
        return dimension_;
    }

private:
    std::map<std::string, std::vector<double>> vectors_;
    size_t dimension_ = 0;
};

int main() {
    InMemoryVectorDB db;

    // Add some vectors
    db.add_vector("doc1", {0.1, 0.2, 0.3, 0.4});
    db.add_vector("doc2", {0.2, 0.3, 0.4, 0.5});
    db.add_vector("doc3", {0.9, 0.8, 0.7, 0.6});
    db.add_vector("doc4", {0.15, 0.25, 0.35, 0.45});

    std::cout << "\nDatabase size: " << db.size() << ", Dimension: " << db.dimension() << std::endl;

    // Perform a search
    std::vector<double> query = {0.1, 0.2, 0.3, 0.4};
    std::cout << "\nSearching for vectors similar to {0.1, 0.2, 0.3, 0.4} (top 2):" << std::endl;
    auto results = db.search(query, 2);

    for (const auto& result : results) {
        std::cout << "  ID: " << result.first << ", Similarity: " << result.second << std::endl;
    }

    // Test getting a vector
    const std::vector<double>* vec = db.get_vector("doc2");
    if (vec) {
        std::cout << "\nRetrieved vector for doc2: ";
        for (double val : *vec) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Test with an invalid query dimension
    try {
        db.search({1.0, 2.0}, 1);
    } catch (const std::runtime_error& e) {
        std::cerr << "\nError during search: " << e.what() << std::endl;
    }

    return 0;
}

# Commit timestamp: 2025-10-07 00:00:00 - 80
