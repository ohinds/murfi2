from collections import namedtuple
import os
import socket
from time import sleep

import threading
import SocketServer

import .external_image

SocketServer.TCPServer.allow_reuse_address = True

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):
    def __init__(self, callback, infoclient, *args, **keys):
            self.callback = callback
            self.infoclient = infoclient
            SocketServer.BaseRequestHandler.__init__(self, *args, **keys)

    def handle(self):
        self.callback(self.infoclient, self.request)
        '''
        cur_thread = threading.current_thread()
        response = "{}: {}".format(cur_thread.name, data)
        self.request.sendall(response)
        '''

def handler_factory(callback, infoclient):
    def createHandler(*args, **keys):
        return ThreadedTCPRequestHandler(callback, infoclient,  *args, **keys)
    return createHandler

def processDataCallback(infoclient, sock):
    infoclient.process_data(sock)

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

class ImageReceiver(object):

    def __init__(self, host=None, port=None, save_location=None):
        self.host = host
        self.port = port

        self._is_running = None
        self._server = None
        self.datastore = []
        self.imagestore = []
        self.save_location = save_location

        self.ei = ExternalImage("ExternalImageHeader")

    def stop(self):
        self._server.shutdown()
        self._is_running = None
        self._server = None
        print "image receiver stopped"

    def start(self):
        self._startserver()

    def check(self):
        if not self._is_running:
            raise RuntimeError('Server is not running')
        return self.datastore

    def _startserver(self):
        if self._is_running:
            raise RuntimeError('Server already running')

        server = ThreadedTCPServer((self.host, self.port),
                                   handler_factory(processDataCallback, self))
        ip, port = server.server_address
        print "image receiver running at %s on port %d" % (ip, port)
        # Start a thread with the server -- that thread will then start one
        # more thread for each request
        server_thread = threading.Thread(target=server.serve_forever)
        # Exit the server thread when the main thread terminates
        server_thread.daemon = True
        server_thread.start()
        self._is_running = True
        self._server = server

    def process_data(self, sock):
        in_bytes = sock.recv(1024)
        if '<rt:done>' in in_bytes:
            self.stop()
            return
        new_ei = self.ei.process_data(in_bytes, sock)
        if new_ei:
            if isinstance(new_ei, nb.Nifti1Image) and new_ei not in self.imagestore:
                self.imagestore.append(new_ei)
                index = len(self.imagestore) - 1
                uid = self.datastore[index].seriesUID
                filename = os.path.join(self.save_location,
                                        'img-%s-%04d.nii.gz' % (uid, index))
                new_ei.to_filename(filename)
                print "Saved to %s" % filename
            elif new_ei not in self.datastore:
                self.datastore.append(new_ei)
        else:
            self.stop()

def receive(save_location):
    receiver = ImageReceiver('localhost', 15000, save_location=save_location)
    receiver.start()
    while(receiver._is_running):
        sleep(1)

def main(argv):
    pass

if __name__ == "__main__":
    sys.exit(main(sys.argv))
