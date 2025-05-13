# Solving the System of Linear Equations
import numpy as np

def solve_linear_system(A, B):
    """Solves the system of linear equations AX = B using numpy."""
    try:
        x = np.linalg.solve(A, B)
        return x
    except np.linalg.LinAlgError as err:
        return f"Error: {err}"

if __name__ == '__main__':
    # A = [[2,3,4],[1,2,3],[2,3,4]]
    # B = [1,2,3]
    # Input matrix A
    rows = int(input("Enter the number of rows in matrix A , n = : "))
    print("Enter the matrix A row by row (space-separated values):")
    A = []
    for _ in range(rows):
        row = list(map(float, input().split()))
        A.append(row)
    A = np.array(A)

    # Input vector B
    print("Enter the vector B (space-separated values):")
    B = list(map(float, input().split()))
    B = np.array(B)

    # Solve the system
    soln = solve_linear_system(A, B)
    print("Solution:", soln)