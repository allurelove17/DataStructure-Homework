#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <limits>

double calculate_total_input_time(double n, double S, double m, double ts, double tl, double tt, int k) {
    if (k < 2) {
        return -1.0;
    }
    double num_passes = std::ceil(std::log(m) / std::log(k));
    double time_per_pass = (n * (k + 1.0) / S) * (ts + tl) + (n * tt);
    return num_passes * time_per_pass;
}

int main() {
    const double n = 200000.0;
    const double S = 2000.0;
    const double m = 64.0;
    const double ts = 0.080;
    const double tl = 0.020;
    const double tt = 0.001;

    std::ofstream csv_file("output_data.csv");
    if (!csv_file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    csv_file << "k,t_input\n";

    double min_time = std::numeric_limits<double>::max();
    int optimal_k = -1;

    for (int k = 2; k <= static_cast<int>(m); ++k) {
        double total_time = calculate_total_input_time(n, S, m, ts, tl, tt, k);
        csv_file << k << "," << std::fixed << std::setprecision(2) << total_time << "\n";

        if (total_time < min_time) {
            min_time = total_time;
            optimal_k = k;
        }
    }

    csv_file.close();

    std::cout << "Successfully generated output_data.csv" << std::endl;
    std::cout << "--- Analysis Summary ---" << std::endl;
    std::cout << "Optimal k: " << optimal_k << std::endl;
    std::cout << "Minimum t_input: " << std::fixed << std::setprecision(2) << min_time << " seconds." << std::endl;

    return 0;
}
