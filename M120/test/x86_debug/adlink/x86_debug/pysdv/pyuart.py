'''
Created on Oct 11, 2017

@author: qxiong06
'''
import logging
import serial
import threading


class PyUart(object):
    '''
    classdocs
    '''

    def __init__(self, port_name, baudrate=115200, timeout=1, logger=None):
        '''
        Constructor
        '''
        if not logger:
            self.logger = logging.getLogger("UART")
        else:
            self.logger = logger
        try:
            #No need to close as Serial will close it automaticlly when the object is released.
            self.serial = serial.Serial(port_name,baudrate=baudrate,timeout=timeout)
        except:
            self.logger.error("Open uart %s fail!"%(port_name))
            raise

    def print_response(self,data):
        for e in data:
            self.logger.info(e.decode())
            #print(e.decode())

    def read_print(self,do_print=None):
        try:
            self.serial.timeout = 1
            data = self.serial.readlines()
            self.print_response(data)
            return data
        except BaseException as err:
            self.logger.error(str(err))

    def send_data(self,data,read_response=True):
        try:
            if isinstance(data,str):
                data=data.encode()
            self.serial.write(data)
            self.serial.flush()
            if read_response:
                return self.read_print(True)
            else:
                return None
        except BaseException as err:
            self.logger.error(str(err))
            return None

"""
Copy from pyserial3.0 as the pyserial 2.7 version don't have this.
"""
class Protocol(object):
    """\
    Protocol as used by the ReaderThread. This base class provides empty
    implementations of all methods.
    """

    def connection_made(self, transport):
        """Called when reader thread is started"""

    def data_received(self, data):
        """Called with snippets received from the serial port"""

    def connection_lost(self, exc):
        """\
        Called when the serial port is closed or the reader loop terminated
        otherwise.
        """
        if isinstance(exc, Exception):
            raise exc

    def timeout_check(self):
        """
        Called when read timeout.
        It maybe normal case.
        """

"""
Copy from pyserial3.0 as the pyserial 2.7 version don't have this.
update the logger
"""
class OPReaderThread(threading.Thread):
    """\
    Implement a serial port read loop and dispatch to a Protocol instance (like
    the asyncio.Protocol) but do it with threads.

    Calls to close() will close the serial port but it is also possible to just
    stop() this thread and continue the serial port instance otherwise.
    """

    def __init__(self, serial_instance, protocol, serial_timeout = 1,logger=None):
        """\
        Initialize thread.

        Note that the serial_instance' timeout is set to serial_timeout.
        Other settings are not changed.
        """
        super(OPReaderThread, self).__init__()
        self.daemon = True
        self.serial = serial_instance
        #self.protocol_factory = protocol_factory
        self.alive = True
        self._lock = threading.Lock()
        self._connection_made = threading.Event()
        self.protocol = protocol
        ##
        self.serial_timeout = serial_timeout
        if not logger:
            self.logger = OPLogManager().get_logger("UART")
        else:
            self.logger = logger

    def stop(self):
        """Stop the reader thread"""
        self.alive = False
        if hasattr(self.serial, 'cancel_read'):
            self.serial.cancel_read()
        self.join(2)

    def run(self):
        """Reader loop"""
        #if not hasattr(self.serial, 'cancel_read'):
        self.serial.timeout = self.serial_timeout
        #self.protocol = self.protocol_factory()
        try:
            self.protocol.connection_made(self)
        except Exception as e:
            self.alive = False
            self.protocol.connection_lost(e)
            self._connection_made.set()
            return
        error = None
        self._connection_made.set()
        while self.alive and self.serial.is_open:
            try:
                # read all that is there or wait for one byte (blocking)
                data = self.serial.read(self.serial.in_waiting or 1)
            except serial.SerialException as e:
                # probably some I/O problem such as disconnected USB serial
                # adapters -> exit
                error = e
                break
            else:
                if data:
                    # make a separated try-except for called used code
                    try:
                        self.protocol.data_received(data)
                    except Exception as e:
                        error = e
                        break
                else:
                    #if no data, must be timeout
                    #Do we need to break
                    if self.protocol.timeout_check():
                        break
                    else:
                        continue
        self.alive = False
        self.protocol.connection_lost(error)
        self.protocol = None

    def write(self, data):
        """Thread safe writing (uses lock)"""
        with self._lock:
            self.serial.write(data)

    def close(self):
        """Close the serial port and exit reader thread (uses lock)"""
        # use the lock to let other threads finish writing
        with self._lock:
            # first stop reading, so that closing can be done on idle port
            self.stop()
            #We don't want the port to be closed as it is not opened by us.
            #self.serial.close()

    def connect(self):
        """
        Wait until connection is set up and return the transport and protocol
        instances.
        """
        if self.alive:
            self._connection_made.wait()
            if not self.alive:
                raise RuntimeError('connection_lost already called')
            return (self, self.protocol)
        else:
            raise RuntimeError('already stopped')

    # - -  context manager, returns protocol

    def __enter__(self):
        """\
        Enter context handler. May raise RuntimeError in case the connection
        could not be created.
        """
        self.start()
        self._connection_made.wait()
        if not self.alive:
            raise RuntimeError('connection_lost already called')
        return self.protocol

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Leave context: close port"""
        self.close()


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# test
if __name__ == '__main__':
    # pylint: disable=wrong-import-position
    import sys
    import time
    import traceback

    #~ PORT = 'spy:///dev/ttyUSB0'
    PORT = 'loop://'
    class PrintLines(object):
        TERMINATOR = b'\r\n'
        ENCODING = 'utf-8'
        UNICODE_HANDLING = 'replace'
        def connection_made(self, transport):
            self.transport = transport
            sys.stdout.write('port opened\n')
            #self.write_line('hello world')

        def handle_line(self, data):
            sys.stdout.write('line received: {!r}\n'.format(data))

        def timeout_check(self):
            sys.stdout.write('timeout check\n')

        def connection_lost(self, exc):
            if exc:
                traceback.print_exc(exc)
            sys.stdout.write('port closed\n')

    ser = serial.serial_for_url(PORT, baudrate=115200, timeout=1)
    with OPReaderThread(ser, PrintLines) as protocol:
        #protocol.write_line('hello')
        time.sleep(2)

    # alternative usage
    ser = serial.serial_for_url(PORT, baudrate=115200, timeout=1)
    t = OPReaderThread(ser, PrintLines)
    t.start()
    transport, protocol = t.connect()
    #protocol.write_line('hello')
    time.sleep(2)
    t.close()




