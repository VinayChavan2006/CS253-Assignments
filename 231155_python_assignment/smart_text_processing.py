# Smart Text processing
def validate_input(n,s):
    """validates the input n,s"""
    if n <= 0 or not isinstance(n, int):
        raise ValueError("Invalid input: Registration number should be an integer, greater than 0")
    if not(len(s) > 0 ) or not(isinstance(s, str)):
        raise ValueError("Invalid input: String should not be empty")
    

def process_string(n,s):
    """Process String: if n is even reverse else  capiltalize vowels and lower the consonants"""
    if n%2 == 0:
        # reverse string
        return s[::-1]
    else:
        s_new = ""
        vowels = ['a','e','i','o','u']
        # capitalize vowels and lower the consonants
        for i in range(len(s)):
            if s[i] in vowels:
                s_new += s[i].upper()
            else:
                s_new += s[i].lower()
        return s_new

def count_set_bits(n):
    """Counts the number of 1's in binary form of n"""
    binary_str = bin(n)
    count = 0
    # count the number of ones in binary form string of n
    for char in binary_str:
        if char == '1':
            count += 1
    return count

def extract_substrings(s,k):
    """Extracts the substrings of length k from s"""

    # if k is greater than the string length then return empty list
    if k > len(s):
        return []
    
    # iterate over string s and append each substring of length k
    substrings = []
    for i in range(len(s)-k+1):
        substrings.append(s[i:i+k])
    return substrings

def sort_or_reverse(n,s,substrings):
    """Sorts substrings lexicographically if n&len(s) is 0, else in reverse order"""
    if n&len(s) == 0:
        # sort the substrings if n&len(s) is 0
        return sorted(substrings)
    else:
        # reverse the substrings
        return substrings[::-1]

def main(n,s):
    """Handles the incoming inputs and applies the functions"""
    validate_input(n,s)
    processed_str = process_string(n,s)
    k = count_set_bits(n)
    substrings = extract_substrings(processed_str,k)
    result = sort_or_reverse(n,s,substrings)
    for res in result:
        print(res,end=' ')

if __name__ == '__main__':
    try:
        id = int(input('Enter your registration number: '))
        if(id <= 0):
            raise ValueError("Registration number must be positive")
        content = input("Enter content string: ")
        main(id,content)
    except Exception as e:
        print(e)
    
