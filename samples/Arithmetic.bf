# Add adjacent cells. Store result in second cell.
# b = (b + a)
# a = 0
[->+<]

# Subtract adjacent cells. Store result in second cell.
# b = (b - a)
# a = 0
[->-<]

# Copy a cell into the cell to its right. Requires two cells to the right of
# the current cell.
# b = a
[->+>+<<] # clone into two cells to right
>>[-<<+>>]<< # move third cell into first
