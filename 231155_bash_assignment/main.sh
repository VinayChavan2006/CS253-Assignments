#!/bin/bash

# Check if exactly 3 arguments are provided
if [ "$#" -ne 3 ]; then
    echo "Usage: bash $0 logs.csv output.txt log_ts.txt"
    exit 1
fi

input_file="$1"
output_file="$2"
log_file="$3"

# Function to log timestamp
log_timestamp() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> "$log_file"
}

# Check if input file exists
if [ ! -f "$input_file" ]; then
    log_timestamp "Input file not detected"
    echo "Error: Input file does not exist"
    exit 1
fi

log_timestamp "Input file exists"

# Clear output file
> "$output_file"

# (a) Extract unique IP addresses
# Using awk to get first column, sort and uniq to get unique IPs
echo -e "Unique IP Adresses:" > "$output_file"
awk -F',' '{print $1}' "$input_file" | grep -v "IP" | sort | uniq >> "$output_file"
log_timestamp "Unique IP extraction completed"

# (b) Top 3 HTTP methods
echo -e "\nTop 3 HTTP Methods:" >> "$output_file"
awk -F',' '{print $3}' "$input_file" | grep -v "HTTP_method" | sort | uniq -c | sort -nr | head -3 | 
sed 's/^\s*\([0-9]*\)\s*\(.*\)/\2: \1 occurrences/' >> "$output_file" # for ex: 3 POST => POST: 3 occurances
log_timestamp "Top 3 HTTP methods identified"

# (c) Hourly request count
echo -e "\nHourly Request Counts:" >> "$output_file"

# Extract hour from timestamp, count occurrences
for hour in {0..23}; do
    # Format hour with leading zero
    hour_padded=$(printf "%02d" $hour)
    
    # Count lines matching this hour using grep
    # sed extracts the hour portion from timestamp
    count=$(sed 's/.* \([0-9][0-9]\):[0-9][0-9]:[0-9][0-9].*/\1/' "$input_file" | 
            grep "^$hour_padded$" | 
            awk 'END {print NR}')  # extract lines for a specific hour and get count by simply no. of rows
    
    # If no matches found, count will be empty, set to 0
    if [ -z "$count" ]; then
        count=0
    fi
    
    # Append result to output file
    echo "Hour $hour: $count requests" >> "$output_file"
done

log_timestamp "Hourly request count completed"

log_timestamp "Script execution completed"
echo "Processing completed. Results in $output_file, logs in $log_file"
