# k-center-in-C
Approximation algorithms for the vertex k-center problem implemented in C

# To run the exact algorithm
## Install gurobipy:

Source: https://www.gurobi.com/gurobi-and-anaconda-for-windows/

### Step one: Download and install Anaconda

Gurobi supports Python 2.7 and 3.7 for Windows. Please choose the version of Anaconda you wish to download (the download will start automatically):

Once the download is complete, click on it to run the installer.

### Step two: Install Gurobi into Anaconda

The next step is to install the Gurobi package into Anaconda. You do this by first adding the Gurobi channel into your Anaconda platform and then installing the gurobi package from this channel.

From an Anaconda terminal issue the following command to add the Gurobi channel to your default search list:

conda config --add channels http://conda.anaconda.org/gurobi

Now issue the following command to install the Gurobi package:

conda install gurobi

You can remove the Gurobi package at any time by issuing the command:

conda remove gurobi

### Step three: Install a Gurobi License

The third step is to install a Gurobi license (if you haven’t already done so).

You are now ready to use Gurobi from within Anaconda. Your next step is to launch either the Spyder IDE or Jupyter Notebook.
