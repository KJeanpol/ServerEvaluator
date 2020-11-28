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
import ast


def graficar():  
    contador=1
    while (contador<4) : 
        f = open(str(contador)+".txt", "r")
        x =(f.readline())
        lista1 = ast.literal_eval(x)
        print(lista1[0])
        y =(f.readline())
        lista2 = ast.literal_eval(y)
        plt.plot(lista1, lista2)
        contador=contador+1
        f.close()

    plt.xlabel('entry a')
    plt.ylabel('entry b')
    plt.show()

graficar()