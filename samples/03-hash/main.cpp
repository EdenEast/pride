
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
    pride::hash64_t hash;
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

bool cmd_option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char*    [])
{
    using namespace pride;
    using std::cout;

    if (cmd_option_exists(argv, argv + argc, "-f"))
        std::cout.setf(std::ios_base::unitbuf);

    read_word_list();
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

    size_t width = 18;
    std::string bar(width * 2 + 3, '-');

    cout << bar << '\n';
    cout << '|' << std::setw(width) << std::right << "crc32"      << '|' << std::setw(width) << std::left << crc32_result.size()      << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "fasthash32" << '|' << std::setw(width) << std::left << fasthash32_result.size() << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "fasthash64" << '|' << std::setw(width) << std::left << fasthash64_result.size() << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "fnv1a32"    << '|' << std::setw(width) << std::left << fnv1a32_result.size()    << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "fnv1a64"    << '|' << std::setw(width) << std::left << fnv1a64_result.size()    << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "mm332"      << '|' << std::setw(width) << std::left << mm332_result.size()      << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "mm364"      << '|' << std::setw(width) << std::left << mm364_result.size()      << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "xxhash32"   << '|' << std::setw(width) << std::left << xxhash32_result.size()   << '|' << '\n';
    cout << '|' << std::setw(width) << std::right << "xxhash64"   << '|' << std::setw(width) << std::left << xxhash64_result.size()   << '|' << '\n';
    cout << bar << '\n';
}
