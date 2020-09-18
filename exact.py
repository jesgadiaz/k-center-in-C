from gurobipy import *
import math
import numpy as np
import heapq

def heap_sort(items):
    heapq.heapify(items)
    items[:] = [heapq.heappop(items) for i in range(len(items))]
    return items

def createGraph(input_file, instance_format):
    global n, m , k, matrix, ordered_sizes
    if instance_format == 'orlib':
        f = open(input_file, "r")
        matrix = []
        for i in range(0,n):
            list = []
            for j in range(0,n):
                list.append(float("inf"))
            matrix.append(list)
        m = sum(1 for line in open(input_file))
        #with open(input_file, "r") as f:  
        for i in range(0, m):
            string = f.readline()
            string = string.split()
            if string is not "EOF":
                v1 = int(string[0]) - 1
                v2 = int(string[1]) - 1
                weight = int(string[2])
                matrix[v1][v2] = weight
                matrix[v2][v1] = weight
        f.close()
        
        for i in range(0, n):
            matrix[i][i] = 0
        for i in range(0, n):
            #print(i)
            for j in range(0, n):
                for l in range(0, n):
                    if matrix[i][j] == float("inf") or matrix[i][l] == float("inf"):
                        cost = float("inf")
                    else:
                        cost = matrix[i][j] + matrix[i][l]
                    if cost < matrix[j][l]:
                        matrix[j][l] = cost
                        
        ordered_sizes = []
        for i in range(0, n):
            for j in range(i, n):
                ordered_sizes.append(matrix[i][j])
        ordered_sizes = heap_sort(ordered_sizes)
    elif instance_format == 'tsplib':
        f = open(input_file, "r")
        m = n
        matrix = []
        for i in range(0,n):
            list = []
            for j in range(0,n):
                list.append(float("inf"))
            matrix.append(list)
        positions = []
        for i in range(0, m):
            string = f.readline()
            string = string.split()
            temp_position = []
            temp_position.append(int(string[0])-1)
            temp_position.append(float(string[1]))
            temp_position.append(float(string[2]))
            positions.append(temp_position)
        for i in range(0, n):
            for j in range(0, n):
                dist_temp = math.sqrt(((positions[i][1] - positions[j][1]) * (positions[i][1] - positions[j][1])) + ((positions[i][2] - positions[j][2]) * (positions[i][2] - positions[j][2])))
                matrix[i][j] = dist_temp
                matrix[j][i] = dist_temp
        f.close()
        for i in range(0, n):
            matrix[i][i] = 0
            
        ordered_sizes = []
        for i in range(0, n):
            for j in range(i, n):
                ordered_sizes.append(matrix[i][j])
        ordered_sizes = heap_sort(ordered_sizes)
    
        
    
def run(r):
    prunedMatrix = []
    for i in range(0,n):
        list = []
        for j in range(0,n):
            list.append(float(0))
        prunedMatrix.append(list)
    for i in range(0,n):
        for j in range(0,n):
            if matrix[i][j] <= r:
                prunedMatrix[i][j] = 1
    
    try:
        global m, num_centers, runtime, cap
        m = Model("mip1")
        
        m.params.BestObjStop = k
        
        y = []
        for i in range(n):
            y.append(0)
        
        for i in range(n):
            y[i] = m.addVar(vtype=GRB.BINARY, name="y%s" % str(i+1))
        
        
        m.setObjective(sum(y), GRB.MINIMIZE)
        
        temp_list = np.array(prunedMatrix).T.tolist()
        
        for i in range(n):
            m.addConstr(sum(np.multiply(temp_list[i], y).tolist()) >= 1)
        
        m.optimize()
        runtime = m.Runtime
        print("The run time is %f" % runtime)
        print("Obj:", m.objVal)
            
        num_centers = m.objVal
        
    except GurobiError:
        print("Error reported")
        
        
def binarySearch():
    global total_runtime, k, runtime, num_centers
    total_runtime = 0
    not_done = True
    upper = len(ordered_sizes) - 1
    lower = 0
    best_solution_size = float("inf")
    while not_done:
        mid = math.ceil(lower + ((upper - lower)/2))
        mid_value = ordered_sizes[int(mid)]
        if mid == upper:
            not_done = False
        else:
            run(mid_value)
            total_runtime = total_runtime + runtime
            if num_centers <= k:
                upper = mid
                if mid_value <= best_solution_size:
                    best_solution_size = mid_value
            else:
                lower = mid
    print("solution size: " + str(best_solution_size))
    print("total runtime: " + str(total_runtime))
    
if __name__ == "__main__":
    if len(sys.argv) != 5:
        print ("Wrong number of arguments")
        print ("exact input_file_path n k instance_format")
        sys.exit()
    input_file  = sys.argv[1]
    n = int(sys.argv[2])
    k = int(sys.argv[3])
    instance_format = sys.argv[4]
    createGraph(input_file, instance_format)
    binarySearch()
