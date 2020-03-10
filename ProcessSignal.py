import serial
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import csv
from math import sqrt

arduinoData = serial.Serial('COM11', 57600)

entradas = []
ch1 = []
ch2 = []
ch3 = []
ch4 = []

def qmean(num):
    return sqrt(sum(n*n for n in num)/len(num))

def trasposeMatrix(M):
    """Calcula a transposta de uma matriz."""
    aux = []
    for j in range(len(M[0])):
        linha = []
        for i in range(len(M)):
            linha.append(M[i][j])
        aux.append(linha)
    return aux

while 1:
    u = 0
    usuario = input("Aperte Enter para começar a leitura")

    if usuario == "":
        arduinoData.write(b'1')

        u = arduinoData.readline()
        u = u.decode('utf-8')
        entradas = u.split(',')
        entradas.pop()

        for i in range(1, 12000, 4):
            ch1.append(int(entradas[i - 1]))
            ch2.append(int(entradas[i]))
            ch3.append(int(entradas[i + 1]))
            ch4.append(int(entradas[i + 2]))

        dados1 = [ch1, ch2, ch3, ch4]
        dados2 = [ch4, ch3, ch2, ch1]
        dados1 = trasposeMatrix(dados1)
        dados2 = trasposeMatrix(dados2)
        dados = [dados1, dados2]

        df = pd.DataFrame(data=dados)

        entradas = []
