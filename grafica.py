from PIL import Image
import numpy as np
from numpy import *
import matplotlib.pyplot as plt
import os
import math
import ast


def graficar(tipo2):  
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
            plt.plot(lista2, lista1)
        else:  
            if (tipo2==1):
                f = open(str(contador)+"_"+str(tipo2)+".txt", "r")
                string=(f.readline())
                x =list(string.split(",")) 
                if(x==""):
                    break
                lista1 = [x[0]]
                lista2 = [x[1]]
                plt.plot(lista1, lista2)
            if (tipo2==2):
                f = open(str(contador)+"_"+str(tipo2)+".txt", "r")
                string=(f.readline())
                x =list(string.split(",")) 
                if(x==""):
                    break
                lista1 = [x[0]]
                lista2 = [x[1]]
                plt.plot(lista1, lista2)
            else:
                f = open(str(contador)+"_"+str(tipo2)+".txt", "r")
                string=(f.readline())
                x =list(string.split(",")) 
                if(x==""):
                    break
                lista1 = [x[0]]
                lista2 = [x[1]]
                plt.plot(lista1, lista2)
 
        contador=contador+1
        f.close()

    plt.xlabel('X')
    plt.ylabel('Y')
    plt.show()

graficar(2)