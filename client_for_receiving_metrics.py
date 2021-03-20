import socket
import time


class ClientError(Exception):
    pass


class Client(ClientError):
    def __init__(self, ip_address, port, timeout=None):
        self.ip_address = ip_address
        self.port = port
        self.timeout = timeout

        self.sock = socket.socket()
        self.sock.connect((self.ip_address, self.port))

    def __del__(self):
        self.sock.close()

    @staticmethod
    def __to_dict__(metric_list):
        new_dict = {}
        for item in metric_list:
            list_item = item.split()
            if len(list_item) != 3:
                raise ClientError
            elif list_item[0] in new_dict:
                new_dict[list_item[0]].append((int(list_item[2]), float(list_item[1])))
            else:
                new_dict[list_item[0]] = [(int(list_item[2]), float(list_item[1]))]

        return {item: sorted(new_dict[item], key=lambda list1: list1[0]) for item in new_dict}

    def get(self, key):
        get_command = "get {}\n".format(key)
        self.sock.send(get_command.encode())
        result = self.sock.recv(1024)
        result = result.decode().split('\n')
        if result[0] == "ok":
            try:
                return self.__to_dict__(result[1:-2])
            except ValueError:
                raise ClientError
        else:
            raise ClientError

    def put(self, key, value, timestamp=None):
        if not timestamp:
            timestamp = int(time.time())
        put_command = "put {} {} {}\n".format(key, value, timestamp)
        self.sock.send(put_command.encode())
        result = self.sock.recv(1024)
        result = result.decode().split('\n')
        if result[0] == "ok" and len(result) == 3:
            pass
        else:
            raise ClientError
