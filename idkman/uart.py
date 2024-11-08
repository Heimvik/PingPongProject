import serial
import struct
import time
import threading


# UART communication settings
UART_PORT = '/dev/ttyACM0'  # Replace with your UART port (e.g., COM3 on Windows)
BAUD_RATE = 9600

# Define protocol constants
STX = 0x02
ETX = 0x03

def send_messages(uart):
    while True:
        payload_content = b'\xAA\xBB\xCC\xDD'  # Example payload data

        # Calculate the length of the payload
        payload_length = len(payload_content)
        len_msb = (payload_length >> 8) & 0xFF  # Most significant byte
        len_lsb = payload_length & 0xFF         # Least significant byte

        # Construct the payload
        example_payload = b'\x02' + bytes([len_msb, len_lsb]) + payload_content + b'\x03'
        uart.send_message(example_payload)
        time.sleep(5)

class UartComm:
    def __init__(self, port, baud_rate):
        # Initialize the serial port
        self.ser = serial.Serial(port, baud_rate, timeout=1)
        self.state = 'WAIT_FOR_STX'
        self.payload_length = 0
        self.payload = bytearray()
        self.payload_index = 0

    def send_message(self, payload):
        """
        Send a message over UART in the defined format.
        Format: [STX][LEN_MSB][LEN_LSB][PAYLOAD][ETX]
        """
        # Calculate the payload length
        length = len(payload)
        if length > 0xFFFF:
            raise ValueError("Payload is too large.")

        # Construct the message
        message = bytearray([STX])
        message.extend(struct.pack('>H', length))  # 2-byte length (MSB, LSB)
        message.extend(payload)
        message.append(ETX)

        # Send the message over UART
        self.ser.write(message)
        print(f"Sent: {message.hex()}")

    def read_message(self):
        """
        Read incoming messages from UART and handle them.
        Uses a state machine to parse the incoming message format.
        """
        while True:
            try:
                byte = self.ser.read(1)
            except serial.SerialException:
                print("Error reading from UART.")
                pass
            try:
                print(byte.decode('utf-8'), end='')
            except UnicodeDecodeError:
                pass
            if byte:
                self.process_byte(byte[0])

    def process_byte(self, byte):
        """
        Process a single byte using the state machine.
        """
        if self.state == 'WAIT_FOR_STX':
            if byte == STX:
                self.state = 'READ_LEN_MSB'
                self.payload.clear()
                self.payload_length = 0
                self.payload_index = 0

        elif self.state == 'READ_LEN_MSB':
            self.payload_length = byte << 8  # Store MSB of the length
            self.state = 'READ_LEN_LSB'

        elif self.state == 'READ_LEN_LSB':
            self.payload_length |= byte  # Store LSB of the length
            if self.payload_length > 0:
                self.state = 'READ_PAYLOAD'
            else:
                self.state = 'WAIT_FOR_ETX'

        elif self.state == 'READ_PAYLOAD':
            self.payload.append(byte)
            self.payload_index += 1
            if self.payload_index >= self.payload_length:
                self.state = 'WAIT_FOR_ETX'

        elif self.state == 'WAIT_FOR_ETX':
            if byte == ETX:
                # Valid packet received
                self.handle_packet(self.payload)
            else:
                print("ERR: Invalid packet format.")
            # Reset state machine
            self.state = 'WAIT_FOR_STX'

    def handle_packet(self, payload):
        """
        Handle a valid packet payload.
        """
        print(f"Received valid packet with length: {len(payload)}")
        print(f"Payload data: {payload.hex()}")

    def close(self):
        """
        Close the UART connection.
        """
        if self.ser.is_open:
            self.ser.close()

if __name__ == "__main__":
    uart = UartComm(UART_PORT, BAUD_RATE)

    try:
        # Example usage: send and receive

        # Construct the payload
        #send_thread = threading.Thread(target=send_messages, args=(uart,))
        #send_thread.start()

        # Start reading messages
        print("Listening for incoming messages...")
        uart.read_message()

    except KeyboardInterrupt:
        print("Program terminated.")
    finally:
        uart.close()
