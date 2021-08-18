#include<iostream>
#include<vector>
#include"id3.hpp"

int main(){
    unsigned max_depth = 5;
    unsigned min_samples = 2;
    id3 tree = {max_depth, min_samples};

    // last element of the datapoint is the class to which it belongs
    std::vector<std::vector<double>> dataset = {
        {04.80, 0.605, 546, 21.0, 14.0, 0},
        {14.99, 0.444, 359, 20.0, 13.0, 0},
        {24.90, 0.478, 314, 21.0, 15.0, 0},
        {04.90, 0.279, 255, 19.0, 12.5, 0},
        {39.90, 1.279, 608, 24.5, 18.0, 1},
        {80.00, 1.518, 883, 24.0, 17.0, 1},
        {29.95, 1.644,1288, 24.0, 17.0, 1},
        {19.33, 0.613, 396, 22.0, 14.0, 2},
        {12.50, 0.344, 287, 19.5, 12.0, 2},
        {09.99, 0.518, 685, 18.5, 12.5, 2},
        {09.95, 0.497, 445, 19.0, 12.5, 2}
    };

    build_id3(tree, dataset);
    for(auto& datapoint : dataset){
        printf("Predicted: %i\n", query_id3(tree, datapoint));
    }
    printf("Accuracy: %lf\n", calculate_accuracy(tree, dataset));
    return 0;
}