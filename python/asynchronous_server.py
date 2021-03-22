import asyncio

def put_serv(map_server, server_name, value, timestamp):
    if server_name in map_server:
        map_server[server_name].append((value, timestamp.strip()))
    else:
        map_server[server_name] = [(value, timestamp.strip())]

    return 'ok\n\n'


def get_serv(map_server, server_name):
    metric_list = []
    if server_name == '*':
        for item in map_server:
            for item_list in map_server[item]:
                strings = str(item + " " + item_list[0] + " " + item_list[1])
                metric_list.append(strings)
    else:
        for item_list in map_server[server_name]:
            strings = str(server_name + " " + item_list[0] + " " + item_list[1])
            metric_list.append(strings)

    return "\n".join(metric_list)


def data_validation(split_list):
    if split_list[0] == 'get' and len(split_list) == 2:
        return True
    elif split_list[0] == 'put' and len(split_list) == 4:
        float(split_list[2])
        int(split_list[3])
        return True
    else:
        raise ValueError


class ClientServerProtocol(asyncio.Protocol):
    def __init__(self):
        self.map_server = {}

    def process_data(self, string):
        result_string = string.strip().split(" ")
        try:
            if data_validation(result_string) and result_string[0] == "put":
                return put_serv(self.map_server, result_string[1], result_string[2], result_string[3])
            elif data_validation(result_string):
                return "ok\n" + get_serv(self.map_server, result_string[1]) + "\n\n"
        except ValueError:
            return "error\nwrong command\n\n"

    def connection_made(self, transport):
        self.transport = transport

    def data_received(self, data):
        resp = self.process_data(data.decode())
        self.transport.write(resp.encode())


loop = asyncio.get_event_loop()
coro = loop.create_server(
    ClientServerProtocol,
    '127.0.0.1', 8888
)

server = loop.run_until_complete(coro)

try:
    loop.run_forever()
except KeyboardInterrupt:
    pass

server.close()
loop.run_until_complete(server.wait_closed())
loop.close()

