# TFTP Client

## Table of Contents

- [Introduction](#introduction)
- [Objectives](#objectives)
- [Prerequisites](#prerequisites)
- [Features](#features)
- [How to Run](#how-to-run)
- [Implementation Details](#implementation-details)

---

## Introduction

This project implements a client program capable of exchanging files using the Trivial File Transfer Protocol (TFTP). The TFTP protocol is widely used for tasks like network installations, diskless node operations, and firmware updates for network devices. The client is developed based on the RFC specifications and Wireshark captures.

This repository includes the code, documentation, and related materials for the project, developed by Kyle Dottin and Reem Khater.

---

## Objectives 

The goal of this project is to:

- Develop a functional TFTP client using the following RFC specifications:
  - RFC1350: TFTP v2
  - RFC2347: TFTP Option Extension
  - RFC2348: TFTP Blocksize Option
  - RFC2349: TFTP Timeout & Tsize Options
  - RFC7440: TFTP Windowsize Option
    
- Use Wireshark captures to analyze and debug TFTP packet exchanges.

- Implement two command-line tools for file transfer:
  - gettftp: To download files from a server.
  - puttftp: To upload files to a server.
 
---

## Prerequisites 

To work on this project, ensure the following tools and environments are available:

- Development Environment: Linux-based (e.g., Ubuntu 22.04 or similar).
- Compiler: GCC (GNU Compiler Collection).
- Network Tools:
  - Wireshark (for capturing and analyzing packets).
  - Netcat and Hexdump (for testing frame exchanges).
  - TFTP server (e.g., atftp package or Moodle TFTP server).
 
---


## Features

1. **Basic TFTP operations**:
   - Download files
     ```
     $ gettftp host file
     ```
   - Upload files:
     ```
     $ puttftp host file
     ```

2. **Domain name resolutions**: Uses `getaddrinfo` to resolve domain names to IP addresses, supporting both IPv4 and IPv6. 
   
3. **RFC Compliance**: Implementation adheres to TFTP RFCs, including support for extensions.

4. **Interactive Console Output**: provides feedback during operations.

5. **Socket Connection**:
   - Reserves a connection socket to the TFTP server on port `69` (default).
   - Iterates through all resolved IP addresses until a connection is successfully established.
   - Prints connection status messages for feedback.


---

## How to Run

1. Clone the repository.
```
git clone https://github.com/your-username/tftp-client.git
cd tftp-client
```
2. Compile the code:
```
gcc TFTPQ1.c -o gettftp
gcc TFTPQ2.c -o puttftp
```
3. Run the TFTP client for downloading a file:
 ```
./gettftp <server-hostname> <file-name> 
 ```
4. Run the TFTP client for uploading a file:
```
./puttftp <server-hostname> <file-name>
```

---

## Implementation Details 

### Question 1: Command-Line Arguments

- Implemented gettftp and puttftp commands to accept server hostname and file name as arguments.
- Provides interactive prompts and feedback during file transfers.
  ```
  Connecting to server...
  download of the file...
  ```

### Question 2: DNS Resolution 

- Utilizes `getaddrinfo` to resolve domain names to IP addresses.
- Supports both IPv4 and IPv6 using `AF_UNSPEC`.
- Iterates through all resolved IP addresses to attempt a connection.
- Prints the resolved IP address for verific.

### Question 3: Socket Connection

- function that reserves a connection socket to the TFTP server.
- Handles connection retries if the first address fails.
- Prints connection status updates ("Connected to the server.").

### Error Handling

- Displays errors for failed domain resolution using `gai_strerror`.
- Silently skips failed connection attempts and retries with the next resolved address.

---
