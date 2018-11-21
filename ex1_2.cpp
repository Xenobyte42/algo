#include <iostream>
#include <string>

#define ALREADY_EXIST -1
#define NOT_EXIST -2

static const size_t primary_size = 8;


template<typename T>
class Hash;

template<>
class Hash<std::string> {
  public:
    size_t operator()(const std::string& val, size_t table_size) {
        size_t hash = 0;
        for (auto i : val) {
            hash = static_cast<size_t>(hash * 127 + i) % table_size;
        }
        return hash + 1;
    }
};


template<typename T>
class Comp;

template<>
class Comp<std::string> {
  public:
    bool operator()(const std::string& str1, const std::string& str2) {
        return str1 == str2;
    }
};


template<
    typename Value,
    typename Hash1 = Hash<Value>,
    typename Hash2 = Hash<Value>,
    typename Comp = Comp<Value>
>
class HashTable {    
  public:
    
    struct Node {
      Value val;
      bool is_deleted = false;
      bool is_empty = true;
    };
    
    HashTable(Hash1 hash1 = Hash1(), Hash2 hash2 = Hash2() , Comp comp = Comp())
    :
    hash1(hash1),
    hash2(hash2),
    comp(comp),
    max_keys_count(primary_size) {
        table = new Node[max_keys_count];
    }
    
    
    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;
    
    ~HashTable() {
        delete [] table;
    }

    bool is_empty() const {
        return items_count > 0;
    }

    size_t size() const {
        return items_count;
    }

    bool in_table(Value& val) {
        size_t i = 0;
        size_t idx = hash(val, i);
        while (i < max_keys_count && table[idx].is_empty == false) {
            if (table[idx].val == val && table[idx].is_deleted == false) {
                return true;
            }
            i++;
            idx = hash(val, i);
        }
        return false;
    }

    ssize_t push(Value& val) {
        if (items_count >= max_keys_count * fill_rate) {
            grow();
        }
        if (in_table(val)) {
            // Если такой элемент уже есть, ошибка
            return ALREADY_EXIST;
        }
        size_t i = 0;
        size_t idx = hash(val, i);
        while (i < max_keys_count && table[idx].is_empty == false) {

            if (table[idx].val == val && table[idx].is_deleted == false) {
                return ALREADY_EXIST;
            }
            if (table[idx].is_deleted == true) {
                table[idx].val = val;
                table[idx].is_deleted = false;
                table[idx].is_empty = false;
                items_count++;
                return 0;
            }
            i++;
            idx = hash(val, i);
        }
        if (table[idx].is_empty == false) {
            grow();
            return push(val);
        }
        
        
        table[idx].val = val;
        table[idx].is_empty = false;
        items_count++;
        return 0;
    }

    ssize_t pop(Value& val) {
        for (size_t i = 0; i < max_keys_count; i++) {
            size_t idx = hash(val, i);
            if (table[idx].is_empty == false) {
                if (table[idx].val == val && table[idx].is_deleted == false) {
                    table[idx].is_deleted = true;

                    items_count--;
                    return 0;
                }
            } else {
                return NOT_EXIST;
            }
        }
        return NOT_EXIST;
    }

  private:
    size_t hash(Value& val, size_t i) {
        return((hash1(val, max_keys_count) + i * hash2(val, max_keys_count)) % max_keys_count);
    }
      
    void grow() {
        size_t old_max_keys_count = max_keys_count;
        max_keys_count = 2 * old_max_keys_count;
        
        Node* new_table = new Node[max_keys_count];
        for (size_t i = 0; i < old_max_keys_count; i++) {
                                                                                    
            if (table[i].is_empty == false && table[i].is_deleted == false) {
                size_t j = 0;

                size_t idx = hash(table[i].val, j);
                while (j < max_keys_count) {
                    if (new_table[idx].is_empty == true) {
                        break;
                    }
                    j++;
                    idx = hash(table[i].val, j);
                }
                new_table[idx].val = table[i].val;
                new_table[idx].is_empty = false;
            }
        }

        delete [] table;
        table = new_table;
    }

    static constexpr double fill_rate = 0.75;

    Hash1 hash1;
    Hash2 hash2;
    Comp comp;

    size_t items_count = 0;
    size_t max_keys_count;

    Node* table;
};


int main() {
    HashTable<std::string> hash_table;
    std::string operation;
    std::string text;
    while (std::cin >> operation >> text) {
        ssize_t error = -1;
        if (operation == "+") {
            error = hash_table.push(text);
        } else if (operation == "-") {
            error = hash_table.pop(text);
        } else if (operation == "?") {
            bool exist = hash_table.in_table(text);
            if (exist) {
                error = 0;
            } else {
                error = NOT_EXIST;
            }
        }

        if (error) {
            std::cout << "FAIL" << std::endl;
        } else {
            std::cout << "OK" << std::endl;
        }
    }
    return 0;
}
