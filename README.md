# Fast Vector DB

![C++](https://img.shields.io/badge/C++-17-blue)
![Python](https://img.shields.io/badge/Python-Bindings-yellow)
![License](https://img.shields.io/badge/license-MIT-green)

A blazing fast, in-memory vector database designed specifically for Retrieval-Augmented Generation (RAG) pipelines. Core engine written in C++ with seamless Python bindings.

## Features
- HNSW (Hierarchical Navigable Small World) index for approximate nearest neighbor search
- Cosine similarity and L2 distance metrics
- Thread-safe concurrent insertions and queries
- Zero-copy Python bindings via pybind11

## Installation
```bash
pip install fast-vector-db
```

## Usage
```python
import fast_vector_db as fvdb
import numpy as np

# Initialize index (dimension=768, max_elements=10000)
index = fvdb.Index(768, 10000)

# Add vectors
vectors = np.random.rand(100, 768).astype(np.float32)
ids = np.arange(100)
index.add_items(vectors, ids)

# Query
query = np.random.rand(1, 768).astype(np.float32)
labels, distances = index.knn_query(query, k=5)
print(f"Nearest IDs: {labels}")
```
