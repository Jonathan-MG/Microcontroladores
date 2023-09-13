import tkinter as tk

def throw_water():
    # Código para lançar água
    pass

def turn_right():
    # Código para virar à direita
    pass

def go_forward():
    # Código para ir para a frente
    pass

def go_left():
    # Código para ir para a esquerda
    pass

def reverse():
    # Código para dar ré
    pass

def stop():
    # Código para parar
    pass

# Crie uma janela
window = tk.Tk()
window.title("Controle Batmóvel")
#window.minsize(1200,600)

# Crie botões com cores e formas personalizadas
button_color = "white"  # Cor de fundo dos botões

btn_throw_water = tk.Button(window, text="💧", bg='blue', relief="solid", width=10, height=4, font=("Helvetica", 14), command=throw_water)
btn_turn_left = tk.Button(window, text="←", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=go_left)
btn_go_forward = tk.Button(window, text="↑", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=go_forward)
btn_turn_right = tk.Button(window, text="→", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=turn_right)
btn_reverse = tk.Button(window, text="↓", bg=button_color, relief="solid", width=10, height=4, font=("Helvetica", 14), command=reverse)
btn_stop = tk.Button(window, text="STOP", bg="red", relief="solid", width=10, height=4, font=("Helvetica", 14), command=stop)
c0 = tk.Label(window, text="     \n      ")
c7 = tk.Label(window, text="     \n      ")
c3 = tk.Label(window, text="     \n                          ")
c5 = tk.Label(window, text="     \n                          ")

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

# Ajustando a janela para 800x480, tamanho da tela 
window.geometry("800x480")

# Inicie o loop de eventos da GUI
window.mainloop()
