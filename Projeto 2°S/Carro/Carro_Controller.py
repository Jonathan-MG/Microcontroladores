import tkinter as tk
import serial
import time
import sys

bluetooth = serial.Serial("/dev/rfcomm0",9600)
def inicio():
    #código para validar a quantidade de água
    string='N'
    bluetooth.write(string.encode('utf-8'))
    
inicio()

def throw_water():
    # Código para lançar água
    string='B'
    bluetooth.write(string.encode('utf-8'))

def go_right():
    # Código para virar à direita
    string='D'
    bluetooth.write(string.encode('utf-8'))

def go_forward():
    # Código para ir para a frente
    string='F'
    bluetooth.write(string.encode('utf-8'))

def go_left():
    # Código para ir para a esquerda
    string='E'
    bluetooth.write(string.encode('utf-8'))

def reverse():
    # Código para dar ré
    string='R'
    bluetooth.write(string.encode('utf-8'))

def stop():
    # Código para parar
    string='P'
    bluetooth.write(string.encode('utf-8'))
    
def exit_app():
    #Código para sair do aplicativo
    sys.exit(0)
    
def center():
    #Código para sair do aplicativo
    string='C'
    bluetooth.write(string.encode('utf-8'))
    


# Crie uma janela
window = tk.Tk()
window.title("Controle Carrinho")
window.geometry("800x480")  # Ajustando o tamanho da janela
#window.attributes('-fullscreen',True)

# Cores personalizadas
bg_color = "#474747"  # Cor de fundo da janela
button_color = "#dec800"  # Cor de fundo dos botões

# Configurando a cor de fundo da janela
window.configure(bg=bg_color)

# Criando botões personalizados
btn_throw_water = tk.Button(window, text="💧 Água", bg='#3cd4fa', relief="solid", width=12, height=3, font=("Helvetica", 14, "bold"), command=throw_water, borderwidth=0, highlightthickness=0)
btn_stop = tk.Button(window, text="STOP", bg="#c21818", relief="solid", width=6, height=3, font=("Segoe UI Semibold", 14), command=stop, borderwidth=0, highlightthickness=0)
btn_exit = tk.Button(window, text="EXIT", bg="#c21818", relief="solid", width=6, height=2, font=("Segoe UI Semibold", 14), command=exit_app, borderwidth=0, highlightthickness=0)

# Posicionando o botão da gota na parte superior da janela com espaçamento de 15px lateral e 25px superior.
btn_throw_water.grid(row=6, column=0, sticky="w", padx=15, pady=(150,10))
btn_exit.grid(row=0, column=0, sticky="ew", padx=15, pady=25)
window.columnconfigure(0, weight=1)  # Centraliza horizontalmente a coluna 0

# Calculando a largura disponível para as setas
available_width = window.winfo_width() - 40

# Definindo as direções das setas
btn_direction_forward = tk.Button(window, text="↑", bg=button_color, relief="solid", width=6, height=3, font=("Segoe UI Semibold", 14, "bold"), command=go_forward, borderwidth=0, highlightthickness=0)
btn_direction_reverse = tk.Button(window, text="↓", bg=button_color, relief="solid", width=6, height=3, font=("Segoe UI Semibold", 14,"bold"), command=reverse, borderwidth=0, highlightthickness=0)
btn_direction_left = tk.Button(window, text="←", bg=button_color, relief="solid", width=6, height=3, font=("Segoe UI Semibold", 14, "bold"), command=go_left, borderwidth=0, highlightthickness=0)
btn_direction_right = tk.Button(window, text="→", bg=button_color, relief="solid", width=6, height=3, font=("Segoe UI Semibold", 14, "bold"), command=go_right, borderwidth=0, highlightthickness=0)
btn_direction_center = tk.Button(window, text="O", bg=button_color, relief="solid", width=6, height=3, font=("Segoe UI Semibold", 14, "bold"), command=center, borderwidth=0, highlightthickness=0)

# Organizando as setas
btn_direction_forward.grid(row=6, column=0, padx=(0,15), pady=(150,10), sticky="e")
btn_direction_reverse.grid(row=7, column=0, padx=(0,15), pady=10, sticky="e")
btn_direction_left.grid(row=7, column=0, padx=(15,275), pady=10, sticky="w")  # Alinhado à direita
btn_direction_right.grid(row=7, column=0, padx=(200,275), pady=10, sticky="w")  # Alinhado à esquerda
btn_direction_center.grid(row=7, column=0, padx=(100,275), pady=10, sticky="w")
# Posicionando o botão "PARE"
btn_stop.grid(row=7, column=0, padx=(15,100), pady=10, sticky="e")

# Iniciando o loop de eventos da GUI
nivel = []
char = ""
while True:
    window.update()
    if bluetooth.in_waiting > 0:
        char = str(bluetooth.read()).replace("b'","").replace("'","")
        if char == 'F':
            valor = "".join(nivel)
            nivel = []
            medicao = int(valor)
            print(f'Medicao: {medicao}')
        else:
           nivel.append(char)
    #print(char)
    valor = ""
    window.update()
    
    
    