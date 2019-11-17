# k-center-in-C
Approximation algorithms for the vertex k-center problem implemented in C

# Approximation algorithms

main_small.c and main_medium.c are equivalent. However, main_small.c admits instances with up to 900 vertices, and main_medium.c admits instances with up to 4663 vertices. 

## Compile

```
$ gcc -o 'output' {code} -lm
```

|  Parameter |                                          Description                                          |
|----------|---------------------------------------------------------------------------------------------|
| `{code}` | (string) main_small.c or main_medium.c                              |

## Run

```
$ 'output' {algorithm} {instance} {n} {k} {seed} {instance_format}
```

## Where,

|  Parameter |                                          Description                                          |
|----------|---------------------------------------------------------------------------------------------|
| `{algorithm}` | (string) Algorithm: Gon, Gon+, HS, HS+, CDS, CDSh, or CDSh+                                    |
| `{instance}` | (string) Instance file path                                    |
| `{n}`    | (integer) Number of vertices  |
| `{k}`    | (integer) Number of centers   |
| `{seed}`    | (integer) Seed   |
| `{instance_format}`    | (string) Instance format: tsplib or orlib |

The code named main_big_bisection.c admits instances with up to 71009 vertices. Besides, instead of performing a binary search it performs a bisection search (for HS and CDSh algorithms), and ony admits instances in TSPLib format.

## Run

```
$ 'output' {algorithm} {instance} {n} {k} {seed}
```

## Where,

|  Parameter |                                          Description                                          |
|----------|---------------------------------------------------------------------------------------------|
| `{algorithm}` | (string) Algorithm: Gon, Gon+, HS, or CDSh                                    |
| `{instance}` | (string) Instance file path                                    |
| `{n}`    | (integer) Number of vertices  |
| `{k}`    | (integer) Number of centers   |
| `{seed}`    | (integer) Seed   |

# Exact algorithm
## Setup
### Install gurobipy:

Source: https://www.gurobi.com/gurobi-and-anaconda-for-windows/

#### Step one: Download and install Anaconda

Gurobi supports Python 2.7 and 3.7 for Windows. Please choose the version of Anaconda you wish to download (the download will start automatically):

Once the download is complete, click on it to run the installer.

#### Step two: Install Gurobi into Anaconda

The next step is to install the Gurobi package into Anaconda. You do this by first adding the Gurobi channel into your Anaconda platform and then installing the gurobi package from this channel.

From an Anaconda terminal issue the following command to add the Gurobi channel to your default search list:

```
$ conda config --add channels http://conda.anaconda.org/gurobi
```

Now issue the following command to install the Gurobi package:

```
$ conda install gurobi
```

You can remove the Gurobi package at any time by issuing the command:

```
$ conda remove gurobi
```

#### Step three: Install a Gurobi License

The third step is to install a Gurobi license (if you haven’t already done so).

You are now ready to use Gurobi from within Anaconda. Your next step is to launch either the Spyder IDE or Jupyter Notebook.

## Run the exact algorithm

To execute the exact algorithm (exact.py) run the following command on the Anaconda prompt:

```
$ python exact.py {instance} {n} {k} {instance_format}
```

## Where,

|  Parameter |                                          Description                                          |
|----------|---------------------------------------------------------------------------------------------|
| `{instance}` | (string) Instance file path                                    |
| `{n}`    | (integer) Number of vertices  |
| `{k}`    | (integer) Number of centers   |
| `{instance_format}`    | (string) Instance format: tsplib or orlib |
