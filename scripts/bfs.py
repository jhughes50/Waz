"""
    Jason Hughes
    June 2024
    A binary breadth first search
"""
import numpy as np

class Pair:
    def __init__(self, x, y):
        self.x = x
        self.y = y

class BinaryBreadthFirstSearch:

    def is_safe(self, mat, visited, x, y):
        return (x >= 0 and x < len(mat) and y >= 0 and y < len(mat[0]) and mat[x][y] == 1 and (not visited[x][y]))

    def search(self, binary_mat, visited, i, j, x, y, min_dist, dist):
        if (i == x and j == y):
            min_dist = min(dist, min_dist)
            return min_dist
        
        visited[i][j] = True

        # go to the bottom cell
        if (self.is_safe(mat, visited, i + 1, j)):
            min_dist = search(mat, visited, i + 1, j, x, y, min_dist, dist + 1)
     
        # go to the right cell
        if (self.is_safe(mat, visited, i, j + 1)):
            min_dist = search(mat, visited, i, j + 1, x, y, min_dist, dist + 1)
     
        # go to the top cell
        if (self.is_safe(mat, visited, i - 1, j)):
            min_dist = search(mat, visited, i - 1, j, x, y, min_dist, dist + 1)
     
        # go to the left cell
        if (self.is_safe(mat, visited, i, j - 1)):
            min_dist = search(mat, visited, i, j - 1, x, y, min_dist, dist + 1)

        visited[i][j] = False
        return min_dist

    def get_path_length(self, binary_mat, src, dest):
        if (len(binary_mat) == 0 or binary_mat[src.x][src.y] == 0 or mat[dest.x][dest.y] ==0):
            return -1

        row = len(binary_mat)
        col = len(binary_mat[0])

        visited = []
        for i in range(row):
            visited.append([None for _ in range(col)])

        dist = sys.maxsize
        dist = search(binary_mat, visited, src.x, src.y, dest.x, dest.y, dist, 0)

        if (dist != sys.maxsize):
            return dist
        return -1
                    
## TODO: change index_map and binary_map to numpy arrays
