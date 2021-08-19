#include<vector>
#include<map>
#include<array>
#include<cmath>
#include<iostream>
#include"id3.hpp"

// interface for recursively building the tree
void build_id3(id3& tree, std::vector<std::vector<double>>& dataset){
    grow_id3(tree, 0, dataset);
}

// recursively builds the tree.
// - leaf nodes use the feature vector for storing the class
//   in order to separate leaf nodes from decision nodes,
//   the class is stored as -(majority_class + 1)
void grow_id3(id3& tree, unsigned current_depth, std::vector<std::vector<double>>& dataset){
    if(dataset.size() >= tree.min_samples && current_depth < tree.max_depth && !is_pure(dataset)){
        unsigned node_index = tree.feature_indicies.size();
        const auto[best_index, best_threshold] = get_split_details(dataset);
        auto datasets = split(dataset, best_index, best_threshold);
        
        tree.feature_indicies.push_back(best_index);
        tree.jumps.resize(node_index);
        tree.feature_thresholds.push_back(best_threshold);

        grow_id3(tree, current_depth + 1, datasets[0]);
        tree.jumps[node_index] = tree.feature_indicies.size();
        grow_id3(tree, current_depth + 1, datasets[1]);
    }else{
        unsigned majority_class = get_majority_class(dataset);
        unsigned node_index = tree.feature_indicies.size();
        tree.jumps.resize(node_index);
        tree.jumps[node_index] = 0;
        tree.feature_indicies.push_back( -(majority_class + 1) );
        tree.feature_thresholds.push_back(0);
    }
}

// sets the values of best_index and best_threshold
// so that the entropy loss / the information gain
// on the dataset is maximized
std::tuple<unsigned, double> get_split_details(std::vector<std::vector<double>>& dataset){
    unsigned best_index = 0;
    double best_threshold = 0;
    double e_c = entropy(dataset);
    double max_gain = 0;
    std::array<std::vector<std::vector<double>>,2> datasets;
    for(unsigned index_feature = 0; index_feature < dataset[0].size() - 1; index_feature++){
        for(unsigned index_member = 0; index_member < dataset.size(); index_member++){
            double threshold = dataset[index_member][index_feature];
            datasets = split(dataset, index_feature, threshold);
            double e_l = entropy(datasets[0]);
            double e_r = entropy(datasets[1]);
            double w_l = datasets[0].size()/dataset.size();
            double w_r = datasets[1].size()/dataset.size();
            if(max_gain < e_c - (w_l*e_l + w_r*e_r) ){
                best_index = index_feature;
                best_threshold = threshold;
            }
        }
    }
    return std::make_tuple(best_index, best_threshold);
}

double entropy(std::vector<std::vector<double>>& dataset){
    unsigned size_dataset = dataset.size();
    if(size_dataset == 0) return 0;

    unsigned size_datapoint = dataset[0].size();
    double entropy;
    std::map<double,unsigned> classes;
    
    for(auto& datapoint : dataset){
        classes[datapoint[size_datapoint-1]]++;
    }
    for(auto& cls : classes){
        unsigned size_subset = cls.second;
        double p_cls = (double)size_subset/size_dataset;
        entropy += -p_cls * log(p_cls);
    }
    return entropy;
}

unsigned get_majority_class(std::vector<std::vector<double>>& dataset){
    double majority_class = 0;
    unsigned majority_size = 0;
    std::map<double, unsigned> classes;
    for(auto& datapoint : dataset){
        classes[datapoint[datapoint.size()-1]] += 1;
    }
    for(auto& cls : classes){
        if(cls.second > majority_size){
            majority_size = cls.second;
            majority_class = cls.first;
        }
    }
    return (unsigned) majority_class;
}

double calculate_accuracy(id3& tree, std::vector<std::vector<double>>& dataset){
    unsigned label_index = dataset[0].size()-1;
    unsigned num_total = dataset.size();
    unsigned num_correct = 0;
    for(auto& datapoint : dataset){
        if(datapoint[label_index] == query_id3(tree, datapoint)) num_correct++;
    }
    return (double)num_correct/num_total;
}

unsigned query_id3(id3& tree, std::vector<double>& datapoint){
    unsigned node_index = 0;
    int feature_index = 0;
    double feature_threshold = 0;
    
    while(feature_index >= 0){
        feature_index = tree.feature_indicies[node_index];
        feature_threshold = tree.feature_thresholds[node_index];
        node_index = datapoint[feature_index] >= feature_threshold ? node_index+1 : tree.jumps[node_index];
    }
    return -(feature_index+1);
}

// split the dataset into new datasets for
// the left and the right nodes. Datapoints
// with a feature valuer lower than the threshold
// are assigned to the right node
std::array<std::vector<std::vector<double>>, 2> split(std::vector<std::vector<double>>& dataset, unsigned feature_index, double feature_threshold){
    std::array<std::vector<std::vector<double>>, 2> datasets;
    std::vector<std::vector<double>> dataset_left;
    std::vector<std::vector<double>> dataset_right;
    for(std::vector<double> datapoint : dataset){
        if (datapoint[feature_index] >= feature_threshold) dataset_left.push_back(datapoint);
        else dataset_right.push_back(datapoint);
    }
    datasets[0] = dataset_left;
    datasets[1] = dataset_right;
    return datasets;
}

bool is_pure(std::vector<std::vector<double>>& dataset){
    unsigned size_dataset = dataset.size();
    if(size_dataset==0) return true;
    unsigned size_datapoint = dataset[0].size();
    double first_class = dataset[0][size_datapoint-1];
    for(unsigned i = 1; i < size_dataset; i++){
        if(first_class != dataset[i][size_datapoint-1]) return false;
    }
    return true;
}