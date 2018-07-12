
 #include <pride/pride.hpp>
 #include <algorithm>
 #include <chrono>
 #include <fstream>
 #include <iomanip>
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <tuple>
 #include <unordered_map>
 #include <utility>
 #include <vector>

class stopwatch_t
{
public:
    stopwatch_t() : _beg(_clock::now()) {}
    void reset() { _beg = _clock::now(); }
    uint64_t delta() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>
            (_clock::now() - _beg).count(); }

private:
    using _clock = std::chrono::high_resolution_clock;
    // using _second = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<_clock> _beg;
};

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
     while(std::getline(ifs, line))
         word_list.push_back(line);
     ifs.close();
     std::cout << "Complete\n";
 }

 template<typename Hash, typename Func>
 std::vector<collision_info_t> process(std::string name, Func func, double& time_result)
 {
    std::unordered_map<Hash, std::vector<std::string>> buffer;
    std::vector<collision_info_t> result;
    std::vector<uint64_t> times;
    std::cout << "Starting to process " << name << '\n';

    std::cout << "Generating " << name << " hash... ";
    times.reserve(word_list.size());
    stopwatch_t t;

    for (auto& s : word_list)
    {
        t.reset();
        Hash h = func(s.c_str(), s.size());
        auto dt = t.delta();
        times.push_back(dt);
        buffer[h].push_back(s);
    }
    std::cout << "Complete\n";

    std::cout << "Processing hash colisions... ";
    result.reserve(buffer.size());
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
    time_result = 0;
    uint64_t sum = 0;
    for (auto t : times)
        sum += t;

    std::cout << "time result: " << time_result  << '\n';

    time_result = ((double)sum / (double)times.size());
    std::cout << "time result: " << time_result  << '\n';
    return result;
 }

 template<typename T>
 std::string to_string_with_precision(const T a_value, const size_t n = 5)
 {
     std::ostringstream out;
     out << std::setprecision(n) << a_value;
     return out.str();
 }

 int main(int argc, char* argv[])
 {
    using namespace pride;
    using std::cout;
    using result_tuple = std::tuple<std::vector<collision_info_t>, std::string, double>;

    read_word_list();
    cout << "There are " << word_list.size() << " amount of words\n";
    cout << '\n';

    result_tuple crc32;
    result_tuple fasthash32;
    result_tuple fasthash64;
    result_tuple fnv1a32;
    result_tuple fnv1a64;
    result_tuple mm332;
    result_tuple mm364;
    result_tuple xxhash32;
    result_tuple xxhash64;

    std::get<0>(crc32)      = process<pride::hash32_t>(std::string("crc32"),      [](auto key, size_t len){ return pride::hash::crc32(key, len); },      std::get<2>(crc32)); cout << '\n';
    std::get<0>(fasthash32) = process<pride::hash32_t>(std::string("fasthash32"), [](auto key, size_t len){ return pride::hash::fasthash32(key, len); }, std::get<2>(fasthash32)); cout << '\n';
    std::get<0>(fasthash64) = process<pride::hash64_t>(std::string("fasthash64"), [](auto key, size_t len){ return pride::hash::fasthash64(key, len); }, std::get<2>(fasthash64)); cout << '\n';
    std::get<0>(fnv1a32)    = process<pride::hash32_t>(std::string("fnv1a32"),    [](auto key, size_t len){ return pride::hash::fnv1a32(key, len); },    std::get<2>(fnv1a32)); cout << '\n';
    std::get<0>(fnv1a64)    = process<pride::hash64_t>(std::string("fnv1a64"),    [](auto key, size_t len){ return pride::hash::fnv1a64(key, len); },    std::get<2>(fnv1a64)); cout << '\n';
    std::get<0>(mm332)      = process<pride::hash32_t>(std::string("mm332"),      [](auto key, size_t len){ return pride::hash::mm332(key, len); },      std::get<2>(mm332)); cout << '\n';
    std::get<0>(mm364)      = process<pride::hash64_t>(std::string("mm364"),      [](auto key, size_t len){ return pride::hash::mm364(key, len); },      std::get<2>(mm364)); cout << '\n';
    std::get<0>(xxhash32)   = process<pride::hash32_t>(std::string("xxhash32"),   [](auto key, size_t len){ return pride::hash::xxhash32(key, len); },   std::get<2>(xxhash32)); cout << '\n';
    std::get<0>(xxhash64)   = process<pride::hash64_t>(std::string("xxhash64"),   [](auto key, size_t len){ return pride::hash::xxhash64(key, len); },   std::get<2>(xxhash64)); cout << '\n';

    size_t precision = 3;
    std::get<1>(crc32)      = to_string_with_precision((static_cast<float>(std::get<0>(crc32).size())      / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(fasthash32) = to_string_with_precision((static_cast<float>(std::get<0>(fasthash32).size()) / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(fasthash64) = to_string_with_precision((static_cast<float>(std::get<0>(fasthash64).size()) / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(fnv1a32)    = to_string_with_precision((static_cast<float>(std::get<0>(fnv1a32).size())    / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(fnv1a64)    = to_string_with_precision((static_cast<float>(std::get<0>(fnv1a64).size())    / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(mm332)      = to_string_with_precision((static_cast<float>(std::get<0>(mm332).size())      / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(mm364)      = to_string_with_precision((static_cast<float>(std::get<0>(mm364).size())      / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(xxhash32)   = to_string_with_precision((static_cast<float>(std::get<0>(xxhash32).size())   / static_cast<float>(word_list.size())) * 100, precision) + " %";
    std::get<1>(xxhash64)   = to_string_with_precision((static_cast<float>(std::get<0>(xxhash64).size())   / static_cast<float>(word_list.size())) * 100, precision) + " %";

    size_t width = 18;
    size_t num_col = 4;
    std::string bar(width * num_col + (num_col + 1), '-');
    std::string underheader(width, '-');

    #define setl() std::setw(width) << std::left
    #define setr() std::setw(width) << std::right

    cout << bar << '\n';
    cout << '|' << setr() << "Hash Name"  << '|' << setl() << "Collistions"                  << '|' << setr() << "Percentage"            << '|' << setr() << "Average time (ns)"     << '|' << '\n';
    cout << '|' << setr() << underheader  << '|' << setl() << underheader                    << '|' << setr() << underheader             << '|' << setr() << underheader             << '|' << '\n';
    cout << '|' << setr() << "crc32"      << '|' << setl() << std::get<0>(crc32).size()      << '|' << setr() << std::get<1>(crc32)      << '|' << setr() << std::get<2>(crc32)      << '|' << '\n';
    cout << '|' << setr() << "fasthash32" << '|' << setl() << std::get<0>(fasthash32).size() << '|' << setr() << std::get<1>(fasthash32) << '|' << setr() << std::get<2>(fasthash32) << '|' << '\n';
    cout << '|' << setr() << "fasthash64" << '|' << setl() << std::get<0>(fasthash64).size() << '|' << setr() << std::get<1>(fasthash64) << '|' << setr() << std::get<2>(fasthash64) << '|' << '\n';
    cout << '|' << setr() << "fnv1a32"    << '|' << setl() << std::get<0>(fnv1a32).size()    << '|' << setr() << std::get<1>(fnv1a32)    << '|' << setr() << std::get<2>(fnv1a32)    << '|' << '\n';
    cout << '|' << setr() << "fnv1a64"    << '|' << setl() << std::get<0>(fnv1a64).size()    << '|' << setr() << std::get<1>(fnv1a64)    << '|' << setr() << std::get<2>(fnv1a64)    << '|' << '\n';
    cout << '|' << setr() << "mm332"      << '|' << setl() << std::get<0>(mm332).size()      << '|' << setr() << std::get<1>(mm332)      << '|' << setr() << std::get<2>(mm332)      << '|' << '\n';
    cout << '|' << setr() << "mm364"      << '|' << setl() << std::get<0>(mm364).size()      << '|' << setr() << std::get<1>(mm364)      << '|' << setr() << std::get<2>(mm364)      << '|' << '\n';
    cout << '|' << setr() << "xxhash32"   << '|' << setl() << std::get<0>(xxhash32).size()   << '|' << setr() << std::get<1>(xxhash32)   << '|' << setr() << std::get<2>(xxhash32)   << '|' << '\n';
    cout << '|' << setr() << "xxhash64"   << '|' << setl() << std::get<0>(xxhash64).size()   << '|' << setr() << std::get<1>(xxhash64)   << '|' << setr() << std::get<2>(xxhash64)   << '|' << '\n';
    cout << bar << '\n';

    return 0;
 }
