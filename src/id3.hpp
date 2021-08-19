#include<vector>
#include<array>
#include <tuple> 

#ifndef ID3_HPP
#define ID3_HPP

typedef struct{
    unsigned max_depth;
    unsigned min_samples;
    std::vector<int> feature_indicies;
    std::vector<double> feature_thresholds;
    std::vector<int> jumps;
}id3;

void build_id3(id3& tree, std::vector<std::vector<double>>& dataset);
void grow_id3(id3& tree, unsigned current_depth, std::vector<std::vector<double>>& dataset);
std::tuple<unsigned, double> get_split_details(std::vector<std::vector<double>>& dataset);
unsigned query_id3(id3& tree, std::vector<double>& datapoint);
unsigned get_majority_class(std::vector<std::vector<double>>& dataset);
double entropy(std::vector<std::vector<double>>& dataset);
double calculate_accuracy(id3& tree, std::vector<std::vector<double>>& dataset);
bool is_pure(std::vector<std::vector<double>>& dataset);



#endif