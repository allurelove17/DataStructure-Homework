import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

def read_csv_file(filename):
    """
    Read CSV file and return a pandas DataFrame.
    """
    try:
        df = pd.read_csv(filename, skipinitialspace=True)
        print(f"Successfully loaded {filename}")
        return df
    except Exception as e:
        print(f"Error reading {filename}: {e}")
        return None

def generate_time_complexity_chart(df, output_filename="time_complexity.png", case_type="Worst Case"):
    """
    Generate time complexity chart for all 5 sorting algorithms
    """
    # Get input sizes
    sizes = df['Size'].values
    
    # Set up the plot with a clean, professional style
    plt.figure(figsize=(14, 10))
    
    # Define algorithms with consistent colors and markers
    algorithms = [
        ("Insertion_Time(ms)", "orange", "o-", "Insertion Sort (O(n²))"),
        ("Quick_Time(ms)", "red", "s-", "Quick Sort MedianOfThree (O(n log n))"),
        ("Merge_Time(ms)", "purple", "D-", "Merge Sort (O(n log n))"),
        ("Heap_Time(ms)", "magenta", "P-", "Heap Sort (O(n log n))"),
        ("Composite_Time(ms)", "blue", "X-", "Composite Sort")
    ]
    
    # Plot each algorithm
    for col_name, color, style, label in algorithms:
        if col_name in df.columns:
            plt.plot(sizes, df[col_name], style, color=color, linewidth=2, markersize=8, label=label)
    
    # Customize the plot to match the provided image
    plt.title(f'Sorting Algorithm Time Complexity - {case_type} (Linear Scale)', fontsize=20)
    plt.xlabel('Input Size (n)', fontsize=16)
    plt.ylabel('Time (ms)', fontsize=16)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=14)
    
    # Use the exact values from the data for the x-ticks
    plt.xticks(sizes)
    
    # Add gridlines matching the provided image
    plt.grid(True, which='both', linestyle='--', alpha=0.7)
    
    # Apply tight layout to make better use of space
    plt.tight_layout()
    
    # Save the plot
    plt.savefig(output_filename, dpi=300)
    print(f"Time complexity chart saved as {output_filename}")
    
    return plt

def generate_space_complexity_chart(df, output_filename="space_complexity.png", case_type="Worst Case"):
    """
    Generate space complexity chart for all 5 sorting algorithms
    
    Note: This function looks for Memory columns in KB format first, and if not found,
    it will look for Memory columns in B format and convert them to KB.
    """
    # Get input sizes
    sizes = df['Size'].values
    
    # Set up the plot with a clean, professional style
    plt.figure(figsize=(14, 10))
    
    # Define column mappings with both KB and B format
    memory_columns = [
        # Column name in KB format, column name in B format, color, style, label
        ("Insertion_Memory(KB)", "Insertion_Memory(B)", "orange", "o-", "Insertion Sort (O(1))"),
        ("Quick_Memory(KB)", "Quick_Memory(B)", "red", "s-", "Quick Sort (O(log n))"),
        ("Merge_Memory(KB)", "Merge_Memory(B)", "purple", "D-", "Merge Sort (O(n))"),
        ("Heap_Memory(KB)", "Heap_Memory(B)", "magenta", "P-", "Heap Sort (O(1))"),
        ("Composite_Memory(KB)", "Composite_Memory(B)", "blue", "X-", "Composite Sort")
    ]
    
    # Plot each algorithm
    for kb_col, b_col, color, style, label in memory_columns:
        # First try KB format
        if kb_col in df.columns:
            plt.plot(sizes, df[kb_col], style, color=color, linewidth=2, markersize=8, label=label)
        # Then try B format and convert to KB
        elif b_col in df.columns:
            memory_kb = df[b_col] / 1024  # Convert bytes to KB
            plt.plot(sizes, memory_kb, style, color=color, linewidth=2, markersize=8, label=label)
        else:
            print(f"Warning: Neither {kb_col} nor {b_col} found in CSV")
    
    # Customize the plot to match the provided image
    plt.title(f'Sorting Algorithm Space Complexity - {case_type} (Linear Scale)', fontsize=20)
    plt.xlabel('Input Size (n)', fontsize=16)
    plt.ylabel('Memory (KB)', fontsize=16)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=14)
    
    # Use the exact values from the data for the x-ticks
    plt.xticks(sizes)
    
    # Add gridlines matching the provided image
    plt.grid(True, which='both', linestyle='--', alpha=0.7)
    
    # Apply tight layout to make better use of space
    plt.tight_layout()
    
    # Save the plot
    plt.savefig(output_filename, dpi=300)
    print(f"Space complexity chart saved as {output_filename}")
    
    return plt

def main():
    """
    Main function to read CSVs and generate four graphs:
    1. Worst-case time complexity
    2. Worst-case space complexity
    3. Average-case time complexity
    4. Average-case space complexity
    """
    print("Starting sorting algorithm visualization...")
    
    # Set the default font size for all plots
    plt.rcParams.update({'font.size': 12})
    
    # Define file paths
    worst_case_file = "sorting_worstcase.csv"
    average_case_file = "sorting_averagecase.csv"
    
    # Check if files exist
    if not os.path.exists(worst_case_file):
        print(f"File {worst_case_file} not found.")
        worst_case_file = input("Enter the worst case CSV filename: ")
    
    if not os.path.exists(average_case_file):
        print(f"File {average_case_file} not found.")
        average_case_file = input("Enter the average case CSV filename: ")
    
    # Read the CSV files
    worst_df = read_csv_file(worst_case_file)
    avg_df = read_csv_file(average_case_file)
    
    # Generate charts based on available data
    if worst_df is not None:
        print(f"Successfully loaded worst case data with {len(worst_df)} rows and {len(worst_df.columns)} columns.")
        
        # Generate worst case charts
        generate_time_complexity_chart(worst_df, "worst_time_complexity_chart.png", "Worst Case")
        generate_space_complexity_chart(worst_df, "worst_space_complexity_chart.png", "Worst Case")
        
        print("Worst case charts generated successfully.")
    else:
        print("Failed to load worst case CSV file.")
    
    if avg_df is not None:
        print(f"Successfully loaded average case data with {len(avg_df)} rows and {len(avg_df.columns)} columns.")
        
        # Generate average case charts
        generate_time_complexity_chart(avg_df, "average_time_complexity_chart.png", "Average Case")
        generate_space_complexity_chart(avg_df, "average_space_complexity_chart.png", "Average Case")
        
        print("Average case charts generated successfully.")
    else:
        print("Failed to load average case CSV file.")
    
    # Summary
    print("\nChart generation complete. The following files have been created:")
    if worst_df is not None:
        print("1. worst_time_complexity_chart.png")
        print("2. worst_space_complexity_chart.png")
    if avg_df is not None:
        print("3. average_time_complexity_chart.png")
        print("4. average_space_complexity_chart.png")

if __name__ == "__main__":
    main()