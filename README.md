# TFTP Client

##Table of contents
-[Introduction](#introduction)
-[How to run](#how-to-tun)

---

## Introduction

In this project, we create a client program capable of exchanging files using the Trivial File Transfer Protocol (TFTP). We develop a TFTP client using RFC specifications and Wireshark captures. TFTP is commonly used for network installation of operating systems (PXE protocol), diskless node operations, and firmware updates for network equipment (routers, IoT, etc.).

This repository contains the code, documentation, and report for the project, completed by Kyle DOTTIN and Reem KHATER. 

---

## Tasks accomplished

We create two clients to be used from the command line:

- One for downloading a file from the server:
```
$ gettftp host file
```
- One for uploading a file to the server:
```
$ puttftp host file
```

---

## How to Run

1. Clone the repository.
2. Compile the code:
   ```
   gcc enseash.c -o enseash
   ```
3. Run the microshell:
   ```
   ./enseash
   ```

---
