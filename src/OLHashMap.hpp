/// Open addressing linear probing hashmap
template <typename K, typename V>
class OLHashMap {
    struct Node {
    };

    DynArray<Node> m_buckets;

public:
private: // INCOMPLETE: NOT IMPLEMENTED
    OLHashMap();
    ~OLHashMap();
    OLHashMap(const OLHashMap &);
    OLHashMap(OLHashMap &&);
    OLHashMap &operator=(const OLHashMap &);
    OLHashMap &operator=(OLHashMap &&);

    void insert(K key, V value);
};
