# Sushi biometrics

## Introduction

This repository was created in the context of the “INFO-F514 - Protocols, cryptanalysis and mathematical cryptology” course at ULB.

The goal of the project was to “expand our knowledge and understanding of Cryptography beyond what is covered in the regular lectures, in a direction of our choice”.

And so, our group chose to dive into the topic of Biometric Authentication using Homomorphic Encryptions. 

---

In short, the making of this repository helped us to better understand the TFHE library and also allowed us to analyse an biometric authentication protocol, for instance the THREATS protocol.

## **THREATS**

The **THREATS** protocol was presented in the “*Privacy-Preserving Biometric Matching Using Homomorphic Encryption*” paper presented at TrustCom 2021. As the paper’s name induces, it implements authentication based on the fully homomorphic encryption.

**THREATS** code is freely available on the following GitHub repository: [https://github.com/lab-incert/threats](https://github.com/lab-incert/threats)

## TFHE Library

**TFHE** Library, or **Fast Fully Homomorphic Encryption over the Torus**, is a library that implements “very fast gate-by-gate bootstrapping”.

It is available through the following page: [https://tfhe.github.io/tfhe/](https://tfhe.github.io/tfhe/)

## Project’s structure

This repository is organised into three directories.

### Code

The **headers** and **sources** directories are respectively the .hpp and .cpp files.

main.cpp contains the main code

client.cpp/hpp and server.cpp/hpp implement the client-server view of the protocol

circuits.cpp/hpp contain the underlying Boolean circuits

Please note that those circuits are a dull copy-paste of those implemented by THREATS’ authors. (minor a few *segmentation fault* fixes). The complete THREATS code can be found on their GitHub page as explained in the THREATS section above.

### Benchmarks

Saved banchmarks are available ithrough the **results** folder, that present the execution of THREATS original code, and this code exececution for the point of view of both an honest and a malicious user.

### Execution

In order to execute the code, simply execute the joined makefile with

```bash
# Either
make

# Or
make compile# compiles the code
make run # executes the code
```