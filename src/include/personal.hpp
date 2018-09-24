#ifndef PERSONAL
#define PERSONAL

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include <boost/container/small_vector.hpp>

namespace my
{

int get_opt_template(int argc, char **argv);

void print_usage(
    std::ostream &out,
    int exit_code,
    const std::string &program_name);

//------------------------------------------------------------------------------
template <typename T>
std::vector<T> split(
    const std::string &buffer,
    const char delimiter)
{
    std::stringstream ss(buffer);
    std::string word;
    std::vector<T> output;

    while (std::getline(ss, word, delimiter))
    {
        std::stringstream ws(word);
        T chunk;
        ws >> chunk;
        output.push_back(chunk);
    }

    return output;
}

template <>
std::vector<int> split<int>(
    const std::string &buffer,
    const char delimiter);

template <>
std::vector<float> split<float>(
    const std::string &buffer,
    const char delimiter);

//------------------------------------------------------------------------------
template <typename Sequence>
void print_seq(Sequence const &v)
{
    std::copy(v.cbegin(), v.cend(),
              std::ostream_iterator<typename Sequence::value_type>(std::cout, " "));
    std::cout << "\n";
}

//------------------------------------------------------------------------------
template <typename T>
using Compare = std::function<bool(T const &, T const &)>;

template <typename T>
using ItList = boost::container::small_vector<std::pair<T, T>, 2>;

template <typename RandomAccessIterator>
ItList<RandomAccessIterator>
quick_sort(RandomAccessIterator start, RandomAccessIterator end)
{
    ItList<RandomAccessIterator> next_iteration;
    // Bad quick_sort implementation
    auto pivot = start;
    auto i = start + 1;
    auto j = end;
    while (i != j)
    {
        if (*i >= *pivot)
        {
            std::swap(*i, *(j - 1));
            --j;
        }
        else
        {
            std::swap(*i, *pivot);
            ++i;
        }
    }

    if (start < pivot - 1)
    {
        next_iteration.push_back(
            make_pair(start, pivot));
    }
    if (pivot + 1 < end - 1)
    {
        next_iteration.push_back(
            make_pair(pivot + 1, end));
    }
    return next_iteration;
}

template <typename Sequence>
void sort(Sequence &s, Compare<typename Sequence::value_type> const &compare_func = std::less())
{
    using Seq_It = typename Sequence::iterator;
    using SortQueue = std::queue<std::future<ItList<Seq_It>>>;

    SortQueue sort_queue;
    sort_queue.push(
        std::async(
            quick_sort<Seq_It>,
            s.begin(),
            s.end()));

    while (!sort_queue.empty())
    {
        auto current = sort_queue.front().get();
        sort_queue.pop();
        for (auto &val : current)
        {
            sort_queue.push(
                std::async(
                    quick_sort<Seq_It>,
                    val.first,
                    val.second));
        }
    }
}
//------------------------------------------------------------------------------
template <typename Function, typename... Args>
void bench_func(Function const &f, Args &&... args)
{
    auto start = std::chrono::high_resolution_clock::now();
    f(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
}

} // namespace my

#endif