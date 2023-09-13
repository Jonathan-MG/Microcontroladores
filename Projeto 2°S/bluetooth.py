import bluetooth

server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
server_sock.bind(("", bluetooth.PORT_ANY))
server_sock.listen(1)

port = server_sock.getsockname()[1]

bluetooth.advertise_service(server_sock, "MyService", service_id=port)

print(f"Aguardando conexão Bluetooth na porta {port}...")

client_sock, client_info = server_sock.accept()
print(f"Conexão estabelecida com {client_info}")

try:
    while True:
        data = client_sock.recv(1024)
        if not data:
            break
        print(f"Recebido: {data}")
        # Aqui você pode responder à mensagem como desejado.
except Exception as e:
    print(f"Erro: {e}")

client_sock.close()
server_sock.close()

###################################### TESTE BLUETOOTH ####################
import serial
import time

bluetooth = serial.Serial("/dev/rfcomm10",9600)

while True:
    a=input("enter: ")
    string='X{0}'.format(a)
