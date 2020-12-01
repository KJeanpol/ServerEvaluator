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
import numbers

fig = plt.figure()

def grafica(tipo2,nombre):  
    contador=1
    while (contador<4) : 
        if(tipo2==2 or tipo2==3):
            f = open(str(contador)+"_"+str(tipo2)+".txt", "r")
            y =(f.readline())
            if(y==""):
                break        
            lista1 = ast.literal_eval(y)
            print(lista1)   
            lista2=[]
            contador2=1
            for i in lista1: 
                lista2.append(contador2)
                contador2=contador2+1
            print(lista2)
            plt.plot(lista2, lista1, label='Servidor'+str(contador))
            fig.suptitle(nombre)
        else:  
            if (tipo2==1 and contador==1):
                f = open(str(1)+"_"+str(tipo2)+".txt", "r")
                string=(f.readline())
                x =list(string.split(",")) 
                for i in range(0, len(x)): 
                    x[i] = int(x[i]) 
                if(x=="" or len(x)==0):
                    break
                lista1 = [x[0]]
                lista2 = x[1]
                plt.plot(lista1,lista2, 'ro', label='Servidor'+str(contador)) 
                fig.suptitle(nombre)
            if (tipo2==1 and contador==2):
                f = open(str(1)+"_"+str(tipo2)+".txt", "r")
                string=(f.readline())
                x =list(string.split(",")) 
                for i in range(0, len(x)): 
                    x[i] = int(x[i]) 
                if(x==" " or len(x)==0):
                    break
                lista1 = [x[0]]
                lista2 = x[2]
                plt.plot(lista1,lista2, 'bo',label='Servidor'+str(contador)) 
                fig.suptitle(nombre)
            else:
                f = open(str(1)+"_"+str(tipo2)+".txt", "r")
                string=(f.readline())
                x =list(string.split(",")) 
                for i in range(0, len(x)): 
                    x[i] = int(x[i]) 
                if(x=="" or len(x)==0):
                    break
                lista1 = [x[0]]
                lista2 = [x[-1]]
                plt.plot(lista1,lista2, 'go',label='Servidor'+str(contador)) 
                fig.suptitle(nombre)
 
        contador=contador+1
        f.close()
    
    plt.legend(loc='best')
    plt.show()
def graficar():
    grafica(1,"Cantidad de solicitudes vs Tiempo en ejecutar todas")
    grafica(2,"Cantidad de solicitudes vs Tiempo promedio por solicitud")
    grafica(3,"Cantidad de solicitudes vs Tiempo en aumento de solicitudes")

graficar()

