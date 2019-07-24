import sys

def run(output_file):
    global n
    out_file = open(output_file, 'w')
    out_file.write('from gurobipy import *\n')
    out_file.write('import heapq\n')
    out_file.write('import math\n')
    out_file.write('def heap_sort(items):\n')
    out_file.write('    heapq.heapify(items)\n')
    out_file.write('    items[:] = [heapq.heappop(items) for i in range(len(items))]\n')
    out_file.write('    return items\n')
    
    out_file.write('def createGraph(input_file):\n')
    out_file.write('    global n, m , k, matrix, ordered_sizes\n')
    out_file.write('    n = ' + n +'\n')
    out_file.write('    f = open(input_file, "r")\n')
    out_file.write('    string = f.readline()\n')
    out_file.write('    string = string.split()\n')
    out_file.write('    m = int(string[0])\n')
    out_file.write('    matrix = []\n')
    out_file.write('    for i in range(0,n):\n')
    out_file.write('        list = []\n')
    out_file.write('        for j in range(0,n):\n')
    out_file.write('            list.append(float("inf"))\n')
    out_file.write('        matrix.append(list)\n')
    
    out_file.write('    positions = []\n')
    out_file.write('    for i in range(0, m):\n')
    out_file.write('        string = f.readline()\n')
    out_file.write('        string = string.split()\n')
    out_file.write('        temp_position = []\n')
    out_file.write('        temp_position.append(int(string[0])-1)\n')
    out_file.write('        temp_position.append(float(string[1]))\n')
    out_file.write('        temp_position.append(float(string[2]))\n')
    out_file.write('        positions.append(temp_position)\n')
    
    out_file.write('    for i in range(0, n):\n')
    out_file.write('        for j in range(0, n):\n')
    out_file.write('            dist_temp = math.sqrt(((positions[i][1] - positions[j][1]) * (positions[i][1] - positions[j][1])) + ((positions[i][2] - positions[j][2]) * (positions[i][2] - positions[j][2])))\n')
    out_file.write('            matrix[i][j] = dist_temp\n')
    out_file.write('            matrix[j][i] = dist_temp\n')
    out_file.write('    f.close()\n')
    
    out_file.write('    for i in range(0, n):\n')
    out_file.write('        matrix[i][i] = 0\n')
    
    
    #out_file.write('    for i in range(0, n):\n')
    #out_file.write('        for j in range(0, n):\n')
    #out_file.write('            for l in range(0, n):\n')
    #out_file.write('                if matrix[i][j] == float("inf") or matrix[i][l] == float("inf"):\n')
    #out_file.write('                    cost = float("inf")\n')
    #out_file.write('                else:\n')
    #out_file.write('                    cost = matrix[i][j] + matrix[i][l]\n')
    #out_file.write('                if cost < matrix[j][l]:\n')
    #out_file.write('                    matrix[j][l] = cost\n')
    #out_file.write('                    \n')
    
    out_file.write('    ordered_sizes = []\n')
    out_file.write('    for i in range(0, n):\n')
    out_file.write('        for j in range(i, n):\n')
    out_file.write('            ordered_sizes.append(matrix[i][j])\n')
    out_file.write('    ordered_sizes = heap_sort(ordered_sizes)\n')
    out_file.write('def run(mid_value):\n')
    out_file.write('    global prunedMatrix\n')
    out_file.write('    prunedMatrix = []\n')
    out_file.write('    for i in range(0,n):\n')
    out_file.write('        list = []\n')
    out_file.write('        for j in range(0,n):\n')
    out_file.write('            list.append(float(0))\n')
    out_file.write('        prunedMatrix.append(list)\n')
    out_file.write('    for i in range(0,n):\n')
    out_file.write('        for j in range(0,n):\n')
    out_file.write('            if matrix[i][j] <= mid_value:\n')
    out_file.write('                prunedMatrix[i][j] = 1\n')
    out_file.write('    try:\n')
    out_file.write('        global m\n')
    out_file.write('        m = Model("mip1")\n')
    #out_file.write('        m.Params.MIPGap = 0\n')
    #out_file.write('        m.Params.MIPGapAbs = 0\n')
    #out_file.write('        m.Params.OptimalityTol = 0.000000001\n')
    #out_file.write('        m.Params.Presolve = 2\n')
    #out_file.write('        m.Params.Heuristics = 0.05\n')
    #out_file.write('        m.Params.TuneCriterion = 0\n')
    out_file.write('        a_ij = 0\n')
    
    n = int(n)
    for i in range(0, n):
        out_file.write('        x' + str(i+1) + ' = m.addVar(vtype=GRB.BINARY, name="x' + str(i+1) + '")\n')
    string = '        m.setObjective(x1'
    for i in range(1, n):
        string = string + ' + x' + str(i+1)
    string = string + ', GRB.MINIMIZE)'
    out_file.write(string + '\n')
    for i in range(0, n):
        string = ''
        for j in range(0, n):
            if j == 0:
                string = string + 'prunedMatrix[' + str(i) + '][' + str(j) + '] * ' + 'x' + str(j+1)
            else:
                string = string + ' + prunedMatrix[' + str(i) + '][' + str(j) + '] * ' + 'x' + str(j+1)
            if j == n - 1:
                string = string + ' >= 1'
        string = '        m.addConstr(' + string + ', "c' + str(i) + '")'
        out_file.write(string + '\n')
        
    #out_file.write('        m.setParam(GRB.Param.MIPGap, 0.00000000)\n')
    out_file.write('        m.optimize()\n')
    out_file.write('        runtime = m.Runtime\n')
    out_file.write('        print("The run time is %f" % runtime)\n')
    out_file.write('        global total_runtime\n')
    out_file.write('        total_runtime = total_runtime + runtime\n')
    out_file.write('        print("The total run time is %f" % total_runtime)\n')
    out_file.write('        global centers\n')
    out_file.write('        global num_centers\n')
    out_file.write('        num_centers = 0\n')
    out_file.write('        for v in m.getVars():\n')
    out_file.write('            if(v.x == 1.0):\n')
    out_file.write('                num_centers = num_centers + 1\n')
    out_file.write('        print("Obj:", m.objVal)\n')
    out_file.write('        print("number of centers: ", str(num_centers))\n')
    out_file.write('        centers = []\n')
    out_file.write('        i = 1\n')
    out_file.write('        for v in m.getVars():\n')
    out_file.write('            if(v.x == 1.0):\n')
    out_file.write('                centers.append(i - 1)\n')
    out_file.write('            i = i + 1\n')
    out_file.write('        print(centers)\n')
    out_file.write('        global max_dist\n')
    out_file.write('        max_dist = 0\n')
    out_file.write('        for i in range(0, n):\n')
    out_file.write('            min_dist = float("inf")\n')
    out_file.write('            for c in centers:\n')
    out_file.write('                if matrix[i][c] <= min_dist:\n')
    out_file.write('                    min_dist = matrix[i][c]\n')
    out_file.write('            if min_dist > max_dist:\n')
    out_file.write('                max_dist = min_dist\n')
    out_file.write('        print("r(C): ", max_dist)\n')
    out_file.write('    except GurobiError:\n')
    out_file.write('        print("Error reported")\n')
    
    out_file.write('def CDS():\n')
    out_file.write('    best_known_size = float("inf")\n')
    out_file.write('    global total_runtime\n')
    out_file.write('    total_runtime = 0\n')
    out_file.write('    upper = len(ordered_sizes) - 1\n')
    out_file.write('    lower = 0\n')
    out_file.write('    mid_history = []\n')
    out_file.write('    b = []\n')
    #out_file.write('    mid = math.ceil(lower + ((upper - lower)/2))\n')
    out_file.write('    not_done = True\n')
    out_file.write('    d = {}\n')
    out_file.write('    while not_done:\n')
    out_file.write('        mid = math.ceil(lower + ((upper - lower)/2))\n')
    out_file.write('        mid_value = ordered_sizes[int(mid)]\n')
    out_file.write('        if mid == upper:\n')
    out_file.write('            not_done = False\n')
    out_file.write('        if mid_value in d:\n')
    out_file.write('            if d[mid_value] == "true":\n')
    out_file.write('                upper = mid\n')
    out_file.write('            else:\n')
    out_file.write('                lower = mid\n')
    out_file.write('        else:\n')
    out_file.write('            run(mid_value)\n')
    out_file.write('            if num_centers <= k:\n')
    #out_file.write('            mid = ordered_sizes.index(max_dist)\n')
    out_file.write('                upper = mid\n')
    out_file.write('                d[mid_value] = "true"\n')
    out_file.write('                mid_history.append(max_dist)\n')
    out_file.write('            else:\n')
    out_file.write('                lower = mid\n')
    out_file.write('                d[mid_value] = "false"\n')
    out_file.write('            b.append(mid_value)\n')
    out_file.write('    print("best solution found (OPT): ")\n')
    out_file.write('    print(repr(min(mid_history)))\n')
    out_file.write('    print(mid_history)\n')
    out_file.write('    print(b)\n')
    
    out_file.write('if __name__ == "__main__":\n')
    out_file.write('    if len(sys.argv) != 3:\n')
    out_file.write('        print ("Wrong number of arguments")\n')
    out_file.write('        print ("tsp2lp input_file_path k")\n')
    out_file.write('        sys.exit()\n')
    out_file.write('    input_file  = sys.argv[1]\n')
    out_file.write('    k = int(sys.argv[2])\n')
    out_file.write('    createGraph(input_file)\n')
    out_file.write('    CDS()\n')
    out_file.close()
    
    
        
if __name__ == '__main__':
    if len(sys.argv) != 3:
        print ("Wrong number of arguments")
        print ("createpy output_file_path n")
        sys.exit()
    output_file  = sys.argv[1]
    n  = sys.argv[2]
    run(output_file)