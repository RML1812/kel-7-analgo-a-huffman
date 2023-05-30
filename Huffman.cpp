#include <iostream>
#include <queue>
#include <chrono>

using namespace std;

int order = 1;

// Representasi node pada pohon Huffman
struct HuffmanNode {
    char ch;
    int freq;
    int order;
    HuffmanNode *left, *right;

    HuffmanNode(char ch, int freq, int order) {
        this->ch = ch;
        this->freq = freq;
        this->order = order;
        left = right = nullptr;
    }
};

// Operator untuk membandingkan dua HuffmanNode berdasarkan frekuensi
struct compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        if(l->freq == r->freq){
            return l->order > r->order;
        }else{
            return l->freq > r->freq;
        }
    }
};

// Membangun pohon Huffman dari tabel frekuensi karakter
HuffmanNode* build_huffman_tree(const vector<pair<pair<char, int>, int>>& freq_table) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare> pq;
    
    // Membuat node Huffman untuk setiap karakter yang muncul
    for (const auto& entry : freq_table) {
        pq.push(new HuffmanNode(entry.first.first, entry.first.second, entry.second));
    }
    
    // Membangun pohon Huffman
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        HuffmanNode* node = new HuffmanNode('$', left->freq + right->freq, ++order);
        node->left = left;
        node->right = right;
        pq.push(node);
    }
    
    return pq.top();
}

// Membuat tabel kode Huffman dari tabel frekuensi karakter
vector<pair<char, string>> generate_huffman_codes(const vector<pair<pair<char, int>, int>>& freq_table) {
    HuffmanNode* root = build_huffman_tree(freq_table);
    vector<pair<char, string>> huffman_codes;
    generate_huffman_codes_helper(root, "", huffman_codes);
    return huffman_codes;
}

void generate_huffman_codes_helper(HuffmanNode* root, string code, vector<pair<char, string>>& huffman_codes) {
    if (!root) return;
    if (root->ch != '$') huffman_codes.push_back({root->ch, code});
    generate_huffman_codes_helper(root->left, code + "0", huffman_codes);
    generate_huffman_codes_helper(root->right, code + "1", huffman_codes);
}

// Mengodekan string menggunakan tabel kode Huffman
string encode_string(const string& input_str, const vector<pair<char, string>>& huffman_codes) {
    string encoded_str = "";
    for (char c : input_str) {
        for (const auto& entry : huffman_codes) {
            if (entry.first == c) {
                encoded_str += entry.second;
                break;
            }
        }
    }
    return encoded_str;
}

int main() {
    // Membaca input dari user
    string input_str;
    cout << "Masukkan string: ";
    getline(cin, input_str);

    // Menghitung frekuensi karakter dalam input string
    vector<pair<pair<char, int>, int>> freq_table;
    for (char c : input_str) {
        bool found = false;
        for (auto& entry : freq_table) {
            if (entry.first.first == c) {
                entry.first.second++;
                found = true;
                order--;
                break;
            }
        }
        if (!found) {
            freq_table.push_back({{c, 1}, order});
        }
        order++;
    }

    // Menghitung waktu untuk membangun pohon Huffman
    auto start_time = chrono::high_resolution_clock::now();
    HuffmanNode* root = build_huffman_tree(freq_table);
    auto end_time = chrono::high_resolution_clock::now();
    cout << "Waktu pembangunan pohon Huffman: "
         << chrono::duration_cast<chrono::microseconds>(end_time - start_time).count()
         << " ms\n";

    // Menghitung waktu untuk mengodekan teks
    vector<pair<char, string>> huffman_codes = generate_huffman_codes(freq_table);
    start_time = chrono::high_resolution_clock::now();
    string encoded_str = encode_string(input_str, huffman_codes);
    end_time = chrono::high_resolution_clock::now();
    cout << "Waktu encoding: "
         << chrono::duration_cast<chrono::microseconds>(end_time - start_time).count()
         << " mikrodetik\n";

    // Menampilkan tabel kode Huffman
    cout << "Tabel kode Huffman:\n";
    for (const auto& entry : huffman_codes) {
        cout << entry.first << " : " << entry.second << "\n";
    }

    // Menampilkan teks yang sudah dikodekan
    cout << "Teks yang sudah dikodekan: " << encoded_str << "\n";

    return 0;
}
