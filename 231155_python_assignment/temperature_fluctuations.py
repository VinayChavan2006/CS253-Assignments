# Analysing Temperature fluctuations
import statistics

# TODO: Handle Edge Cases
def analyze_temperatures(temperatures):
    """Computes the mean, median, standard deviation, and sample variance of the temperatures data"""

    # if temperatures is empty list then display a message
    if len(temperatures) < 1:
        print("Empty Dataset")
        return
    if not all(isinstance(t, (int, float)) for t in temperatures):
        return "Error: All temperatures must be numbers."
    try:
        # mean and median of data
        mean = statistics.mean(temperatures)
        median = statistics.median(temperatures)

        # Handle standard deviation and variance for single data point
        if len(temperatures) == 1:
            stdev = 0
            variance = 0
        else:
            stdev = statistics.stdev(temperatures)
            variance = statistics.variance(temperatures)
        print(f'mean: {mean}\nmedian: {median}\nstandard deviation: {stdev}\nvariance: {variance}')
        return [mean,median,stdev,variance]
    except Exception as e:
        return f"Error: {e}"
    
if __name__ == '__main__':
    temperatures = input("Enter the data (space seperated) of temperatures: ")
    temperatures = list(map(float,temperatures.split()))
    analyze_temperatures(temperatures)

