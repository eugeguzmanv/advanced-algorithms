# Initialize n to 1
n = 1

# Start an infinite loop that will break once the condition is met
while True:
    # Calculate the expression's numerator
    numerator = 101 * n
    
    # Check if the numerator is perfectly divisible by 101
    if numerator % 1020096 == 1:
        # If it is, we've found our number.
        # Print the value of n and the result of the operation.
        print(n)
        
        # Break out of the loop
        break
    
    # If not, increment n and continue to the next iteration
    n += 1