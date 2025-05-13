# Sum of Prime : Efficiently using Sieve of Eratosthenes
import math

def sieve(limit):
    """Generates primes upto limit using Sieve of Eratosthenes"""
    isPrime = [True] * (limit + 1)
    isPrime[0] = isPrime[1] = False
    for i in range(2, int(limit ** 0.5) + 1):
        if isPrime[i]:
            for j in range(i*i, limit + 1, i):
                isPrime[j] = False
    return [i for i in range(limit+1) if isPrime[i]]

def sum_of_primes(n):
    """Computes the sum of first n primes using Sieve of Eratosthenes."""

    # check for a valid input for n
    if not isinstance(n, int) or n<=0:
        return 0
    
    if n == 1:
        limit = 2
    else:
        try:
            log_n = math.log(n)
            log_log_n = math.log(log_n)
            limit = int(n* (log_n + log_log_n)) + 1
        except ValueError:
            # for small n
            limit = max(100, int(n * 4.0))
    
    # Ensure limit is sufficient for small n
    limit = max(limit, 10)

    # Get primes
    primes = sieve(limit)

    # Sum of first n primes
    if len(primes) < n:
        return 0
    else:
        return sum(primes[:n])

if __name__ == '__main__':
    try:
        n = int(input("Enter a number: "))
        if(n > 0):
            sum = sum_of_primes(n)
            print(sum)
        else:
            print('The number must be positive integer')
    except Exception as e:
        print(e)
    

