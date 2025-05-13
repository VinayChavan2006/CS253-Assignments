# Shivam's Data Science Expt.

import random
import numpy as np
import matplotlib.pyplot as plt


def generate_dataset(N,x_min, x_max):
    """Generates a dataset of N random points X,Y"""

    # Generate n uniformly distributed random numbers between x_min, x_max
    X = np.random.uniform(low=x_min,high=x_max,size=N)

    # List of functions
    functions = [np.sin, np.cos, np.tan, lambda x:x**2, lambda x:x**3]
    if x_min > 0:
        functions.append(np.log)

    # random constants
    A, B, C, D, E, F = np.random.randn(6)

    f1, f2, f3 = np.random.choice(functions, 3, replace=False)

    # Generate Y from chosen functions 
    Y = A*f1(B*X) + C*f2(D*X) + E* f3(F*X)
    return X, Y

def plot_scatter(X,Y):
    """Creates a scatter plot for X v/s Y"""
    plt.figure(figsize = (8,6))
    plt.scatter(X, Y, color = 'blue', alpha = 0.5)
    plt.title('Scatter Plot of X vs Y')
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.show()

def plot_histogram(X):
    """Plots a histogram of X"""
    plt.figure(figsize = (8,6))
    plt.hist(X, bins = 'auto',color = 'red', alpha = 0.4)
    plt.title('Histogram of X')
    plt.xlabel('X')
    plt.ylabel('Frequency')
    plt.show()

def plot_box(Y):
    """Plots the box plot for Y"""
    plt.figure(figsize = (8,6))
    box = plt.boxplot(Y, vert = False, patch_artist=True,boxprops=dict(facecolor='lightblue', color='blue', alpha = 0.4))
    plt.title('Box plot of Y')
    plt.xlabel('Y')


    # Customize the whiskers and caps
    for whisker in box['whiskers']:
        whisker.set(color='blue', linewidth=1.5, linestyle='--',alpha = 0.4)
    for cap in box['caps']:
        cap.set(color='blue', linewidth=1.5, alpha = 0.4)
    
    # Customize the median line
    for median in box['medians']:
        median.set(color='red', linewidth=2, alpha = 0.4)

    plt.show()

def plot_line(X, Y):
    """Line plot for sorted X v/s Y"""
    sorted_X = np.sort(X)
    sorted_Y = np.sort(Y)

    plt.figure(figsize =(8,6))
    plt.plot(sorted_X, sorted_Y, color = 'green', alpha = 0.4,label = 'Y vs X: Sorted')
    plt.title('Line plot of sorted X vs Y')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.legend()
    plt.show()

def set_random_seed(seed):
    """Sets random seed for reproducibility"""
    np.random.seed(seed)
    random.seed(seed)


def main():
    """Handles input and the dataset generation and plotting."""
    try:
        N = int(input("Enter number of data points (N): "))
        x_min = float(input("Enter minimum X value: "))
        x_max = float(input("Enter maximum X value: "))
        
        # Set random seed
        set_random_seed(23)
        
        # Generate dataset
        X, Y = generate_dataset(N, x_min, x_max)
        
        # Generate plots
        plot_scatter(X, Y)
        plot_histogram(X)
        plot_box(Y)
        plot_line(X, Y)
        
    except ValueError as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
    
