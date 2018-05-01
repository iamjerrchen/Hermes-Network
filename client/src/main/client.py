
import socket

PORT = 80085
def main():
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(("localhost",PORT))

	while True:
		sock.send("Hello")
		resp = sock.recv(2048)
		print "Response %s" % (resp)

if __name__ == "__main__":
	main()