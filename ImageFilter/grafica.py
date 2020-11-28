from PIL import Image
import numpy as np
from numpy import *
import matplotlib.pyplot as plt
import os
import math
from PIL import Image
import numpy as np
from numpy import *
import matplotlib.pyplot as plt
import os
import math


def graficar():  
    contador=1
    while (contador<4) : 
        f = open(str(contador)+".txt", "r")
        x =(f.readline())
        print(x)
        y =(f.readline())
        plt.plot(x, y)
        contador=contador+1
        f.close()

    plt.xlabel('entry a')
    plt.ylabel('entry b')
    plt.show()

graficar()