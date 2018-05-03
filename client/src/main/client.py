import socket
import argparse

PORT = 60085

def get_parser():
	parser = argparse.ArgumentParser(description="Make call to po node")
	parser.add_argument("method", choices=["PUSH","PULL"])
	parser.add_argument("--to", dest="to",help="destination ip")
	parser.add_argument("--msg", dest="msg",help="message contents")

	return parser

def main():
	args = get_parser().parse_args()
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(("localhost",PORT))

	if args.method == "PULL":
		sock.send("PULL")
		resp = sock.recv(512)
		if resp == "FAIL":
			print "Pull failed"
		else:
			num_msgs = int(resp[8:])
			for _i in range(num_msgs):
				single_msg = sock.recv(2048)
				print single_msg

	elif args.method == "PUSH":
		request_str = "PUSH " + "IP:" + args.to + " MSG:" + args.msg
		sock.send(request_str)
		resp = sock.recv(2048)
		print "Response %s" % (resp)

if __name__ == "__main__":
	main()