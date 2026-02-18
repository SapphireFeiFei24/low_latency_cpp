# Network
## The 7 OSI Model Layers (Top to Bottom):
* Layer 7: Application Layer: Interacts directly with software applications to provide network services (e.g., HTTP, FTP, SMTP, DNS).
* Layer 6: Presentation Layer: Formats, encrypts, and compresses data for the application layer, ensuring compatibility (e.g., SSL/TLS, ASCII, JPEG).
* Layer 5: Session Layer: Manages, maintains, and terminates connections between applications on different devices (e.g., SYN/ACK, API sessions).
* Layer 4: **Transport Layer**: Handles end-to-end communication, flow control, and error correction (e.g., TCP, UDP, port numbers).
* Layer 3: **Network Layer**: Manages logical addressing (IP addresses) and routing data packets across networks (e.g., Routers).
* Layer 2: Data Link Layer: Handles **node-to-node** data transfer, physical addressing (MAC addresses), and error detection (e.g., Switches, Bridges, Ethernet).
* Layer 1: Physical Layer: Transmits **raw bitstreams** over physical media (e.g., cables, hubs, NICs, optical signals).
![img.png](img.png)
## TCP/IP 5 Layer model
![img_1.png](img_1.png)

## TCP Handshakes
> 3 Handshakes to connect, 4 Handshakes to disconnect \
> * Reason for 4 handshakes for disconnection: After the client send FIN, the server can still send more data, when it's done sending data, it'll send a FIN back to client.
![img_2.png](img_2.png)

## Data Pack/Unpack
### 1. Packing: The Sending Process
Before data leaves your device, it goes through a process of encapsulation, where more information is added at each layer of the network stack.
* Segmentation (**TCP**): The original file is broken into smaller chunks. 
* Packet Structuring: Each packet is given **a header and a trailer**. 
* Header: Contains the source IP address, destination IP address, sequence number (for reordering), and port numbers. 
* Payload: The actual portion of the data being sent. 
* Trailer: Contains error-checking information (e.g., checksum) to ensure the data is not corrupted during transit. 
* Encapsulation: The packet is placed inside a frame (layer 2) to prepare it for transmission over local hardware (Ethernet or Wi-Fi). 
* Transmission: The data is converted into physical signals (light or electrical pulses) and sent across the network.
2. Routing: Moving Across the Internet 
* Packets often travel independently to their destination. 
* Independent Paths: Each packet may take a different route through routers based on network traffic and efficiency. 
* Routers: Routers check the destination IP address in the header and forward the packet to the next, most efficient, "hop".
3. Unpacking: The Receiving Process 
Once packets reach their destination, the process is reversed to reassemble the original data.
* Reception & Verification: The receiving network interface checks the packet for errors using the trailer. 
* Decapsulation: The headers are stripped off, moving up the protocol stack, and the data is inspected. 
* Reassembly (TCP): The receiving device uses the sequence numbers in the headers to put the packets back in the correct order.
* Handling Errors: If a packet is lost or corrupted, TCP requests a retransmission of that specific packet, not the entire file.

## DNS // TODO