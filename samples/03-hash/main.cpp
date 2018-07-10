
 #include <pride/pride.hpp>
 #include <algorithm>
 #include <iostream>
 #include <string>
 #include <fstream>
 #include <sstream>
 #include <utility>
 #include <unordered_map>
 #include <vector>
 #include <iomanip>

 std::vector<std::string> word_list;

 std::pair<std::string, std::string> split_path(const std::string path)
 {
     std::string copy = path;
     std::replace(copy.begin(), copy.end(), '\\', '/');
     auto idx = path.find_last_of("/\\");
     return std::make_pair(path.substr(0, idx), path.substr(idx + 1));
 }

 std::string get_file_dir()
 {
     return split_path(__FILE__).first;
 }

 struct collision_info_t
 {
     pride::hash64_t hash = 0;
     std::vector<std::string> words;
 };

 void read_word_list()
 {
     std::cout << "Reading word list... ";
     std::string filename = get_file_dir() + "/word-list.txt";
     std::ifstream ifs(filename.c_str());
     if (!ifs)
     {
         std::cerr << "Cannot open file\n";
         std::exit(1);
     }

     std::string line;
     int count = 0;
     while(std::getline(ifs, line))
         word_list.push_back(line);
     ifs.close();
     std::cout << "Complete\n";
 }

 template<typename Hash, typename Func>
 std::vector<collision_info_t> process(std::string name, Func func)
 {
     std::unordered_map<Hash, std::vector<std::string>> buffer;
     std::vector<collision_info_t> result;
     std::cout << "Starting to process " << name << '\n';

     std::cout << "Generating " << name << " hash... ";
     for (auto& s : word_list)
     {
         Hash h = func(s.c_str(), s.size());
         buffer[h].push_back(s);
     }
     std::cout << "Complete\n";

     std::cout << "Processing hash colisions... ";
     for(const auto& pair : buffer)
     {
         if (pair.second.size() > 1)
         {
             collision_info_t info;
             info.hash = pair.first;
             for(const auto& s : pair.second)
                 info.words.push_back(s);
             result.push_back(info);
         }
     }
     std::cout << "Complete\n";
     return result;
 }

 template<typename T>
 std::string to_string_with_precision(const T a_value, const int n = 5)
 {
     std::ostringstream out;
     out << std::setprecision(n) << a_value;
     return out.str();
 }

 int main(int argc, char* argv[])
 {
    using namespace pride;
    using std::cout;

    read_word_list();
    cout << "There are " << word_list.size() << " amount of words\n";
    cout << '\n';

    auto crc32_result      = process<pride::hash32_t>(std::string("crc32"),      [](auto key, size_t len){ return pride::hash::crc32(key, len); });      cout << '\n';
    auto fasthash32_result = process<pride::hash32_t>(std::string("fasthash32"), [](auto key, size_t len){ return pride::hash::fasthash32(key, len); }); cout << '\n';
    auto fasthash64_result = process<pride::hash64_t>(std::string("fasthash64"), [](auto key, size_t len){ return pride::hash::fasthash64(key, len); }); cout << '\n';
    auto fnv1a32_result    = process<pride::hash32_t>(std::string("fnv1a32"),    [](auto key, size_t len){ return pride::hash::fnv1a32(key, len); });    cout << '\n';
    auto fnv1a64_result    = process<pride::hash64_t>(std::string("fnv1a64"),    [](auto key, size_t len){ return pride::hash::fnv1a64(key, len); });    cout << '\n';
    auto mm332_result      = process<pride::hash32_t>(std::string("mm332"),      [](auto key, size_t len){ return pride::hash::mm332(key, len); });      cout << '\n';
    auto mm364_result      = process<pride::hash64_t>(std::string("mm364"),      [](auto key, size_t len){ return pride::hash::mm364(key, len); });      cout << '\n';
    auto xxhash32_result   = process<pride::hash32_t>(std::string("xxhash32"),   [](auto key, size_t len){ return pride::hash::xxhash32(key, len); });   cout << '\n';
    auto xxhash64_result   = process<pride::hash64_t>(std::string("xxhash64"),   [](auto key, size_t len){ return pride::hash::xxhash64(key, len); });   cout << '\n';

    size_t precision = 3;
    auto crc32_pst      = to_string_with_precision((static_cast<float>(crc32_result.size())      / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto fasthash32_pst = to_string_with_precision((static_cast<float>(fasthash32_result.size()) / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto fasthash64_pst = to_string_with_precision((static_cast<float>(fasthash64_result.size()) / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto fnv1a32_pst    = to_string_with_precision((static_cast<float>(fnv1a32_result.size())    / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto fnv1a64_pst    = to_string_with_precision((static_cast<float>(fnv1a64_result.size())    / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto mm332_pst      = to_string_with_precision((static_cast<float>(mm332_result.size())      / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto mm364_pst      = to_string_with_precision((static_cast<float>(mm364_result.size())      / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto xxhash32_pst   = to_string_with_precision((static_cast<float>(xxhash32_result.size())   / static_cast<float>(word_list.size())) * 100, precision) + " %";
    auto xxhash64_pst   = to_string_with_precision((static_cast<float>(xxhash64_result.size())   / static_cast<float>(word_list.size())) * 100, precision) + " %";

    size_t width = 18;
    std::string bar(width * 3 + 4, '-');
    std::string underheader(width, '-');

    #define setl() std::setw(width) << std::left
    #define setr() std::setw(width) << std::right

    cout << bar << '\n';
    cout << '|' << setr() << "Hash Name"  << '|' << setl() << "Collistions"            << '|' << setr() << "Percentage"   << '|' << '\n';
    cout << '|' << setr() << underheader  << '|' << setl() << underheader              << '|' << setr() << underheader    << '|' << '\n';
    cout << '|' << setr() << "crc32"      << '|' << setl() << crc32_result.size()      << '|' << setr() << crc32_pst      << '|' << '\n';
    cout << '|' << setr() << "fasthash32" << '|' << setl() << fasthash32_result.size() << '|' << setr() << fasthash32_pst << '|' << '\n';
    cout << '|' << setr() << "fasthash64" << '|' << setl() << fasthash64_result.size() << '|' << setr() << fasthash64_pst << '|' << '\n';
    cout << '|' << setr() << "fnv1a32"    << '|' << setl() << fnv1a32_result.size()    << '|' << setr() << fnv1a32_pst    << '|' << '\n';
    cout << '|' << setr() << "fnv1a64"    << '|' << setl() << fnv1a64_result.size()    << '|' << setr() << fnv1a64_pst    << '|' << '\n';
    cout << '|' << setr() << "mm332"      << '|' << setl() << mm332_result.size()      << '|' << setr() << mm332_pst      << '|' << '\n';
    cout << '|' << setr() << "mm364"      << '|' << setl() << mm364_result.size()      << '|' << setr() << mm364_pst      << '|' << '\n';
    cout << '|' << setr() << "xxhash32"   << '|' << setl() << xxhash32_result.size()   << '|' << setr() << xxhash32_pst   << '|' << '\n';
    cout << '|' << setr() << "xxhash64"   << '|' << setl() << xxhash64_result.size()   << '|' << setr() << xxhash64_pst   << '|' << '\n';
    cout << bar << '\n';

    return 0;
 }
