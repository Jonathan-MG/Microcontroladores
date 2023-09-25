import tkinter as tk
import serial
import time

bluetooth = serial.Serial("/dev/rfcomm0",9600)

def throw_water():
    # Código para lançar água
    #string='B'
    string='L'
    bluetooth.write(string.encode('utf-8'))

def turn_right():
    # Código para virar à direita
    #string='D'
    string='L'
    bluetooth.write(string.encode('utf-8'))

def go_forward():
    # Código para ir para a frente
    #string='F'
    string='L'
    bluetooth.write(string.encode('utf-8'))

def go_left():
    # Código para ir para a esquerda
    #string='E'
    string='L'
    bluetooth.write(string.encode('utf-8'))

def reverse():
    # Código para dar ré
    #string='R'
    string='L'
    bluetooth.write(string.encode('utf-8'))

def stop():
    # Código para parar
    #string='P'
    string='D'
    bluetooth.write(string.encode('utf-8'))

# Crie uma janela
window = tk.Tk()
window.title("Controle Batmóvel")
#window.minsize(1200,600)

# Crie botões com cores e formas personalizadas
button_color = "white"  # Cor de fundo dos botões

btn_throw_water = tk.Button(window, text="Água", bg='blue', relief="solid", width=10, height=4, font=("Helvetica", 14), command=throw_water)
btn_turn_left = tk.Button(window, text="←", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=go_left)
btn_go_forward = tk.Button(window, text="↑", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=go_forward)
btn_turn_right = tk.Button(window, text="→", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=turn_right)
btn_reverse = tk.Button(window, text="↓", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=reverse)
btn_stop = tk.Button(window, text="STOP", bg="red", relief="solid", width=10, height=4, font=("Helvetica", 14), command=stop)
c0 = tk.Label(window, text="     \n      ")
c7 = tk.Label(window, text="     \n      ")
c3 = tk.Label(window, text="     \n                    ")
c5 = tk.Label(window, text="     \n                    ")

# Arrange os botões usando o grid layout
btn_throw_water.grid(row=2, column=4)
btn_turn_left.grid(row=1, column=1)
btn_go_forward.grid(row=1, column=7)
btn_turn_right.grid(row=1, column=2)
btn_reverse.grid(row=2, column=7)
btn_stop.grid(row=2, column=6)
c0.grid(row=0, column=0)
c7.grid(row=3,column=8)
c3.grid(row=0,column=3)
c5.grid(row=0,column=5)

# Inicie o loop de eventos da GUI
window.mainloop()



